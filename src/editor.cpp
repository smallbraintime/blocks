#include "editor.h"

#include <QApplication>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

Editor::Editor(QWidget* parent) : QWidget(parent), m_camera({20.0f, 20.0f, 20.0f}), m_cameraController{&m_camera, {5.0f, 5.0f, 5.0f}, 10.0f}, m_renderer{new BlocksRenderer(this, &m_camera, &m_pointedBlock)} {
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    m_renderer->setFormat(format);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_renderer);

    setFocusPolicy(Qt::StrongFocus);
    cursor().setPos(mapToGlobal(rect().center()));
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled, false);

    resetBlocks();

    connect(m_renderer, &BlocksRenderer::glInitialized, this, &Editor::onGlInitialized);
}

void Editor::setColor() {
    m_currentColor = QColorDialog::getColor(Qt::white, this, "Choose color", QColorDialog::DontUseNativeDialog);
}

bool Editor::openProject() {
    m_filename = QFileDialog::getOpenFileName(this, "Open .blks File", "", "BLKS Files (*.blks)");
    if (!m_filename.isEmpty()) {
        QFile file(m_filename);
        if (file.open(QIODevice::ReadOnly)) {
            file.read(reinterpret_cast<char*>(m_blocks.data()), m_blocks.size() * sizeof(Color));
            return true;
        } else {
            QMessageBox::critical(this, "Error", "Failed to open the file.");
        }
    }

    return false;
}

void Editor::saveProject() {
    if (m_filename.isEmpty()) {
        m_filename = QFileDialog::getSaveFileName(this, "Save As", "", "BLKS Files (*.blks)");
        if (m_filename.isEmpty()) {
            return;
        }
    }

    QFile file(m_filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(reinterpret_cast<char*>(m_blocks.data()), m_blocks.size() * sizeof(Color));
    }
}

void Editor::resetBlocks() {
    m_blocks.fill({255, 255, 255, 0}, CHUNK_SIZE);
    for (size_t z = 0; z < VECTOR_SIZE; ++z) {
        for (size_t x = 0; x < VECTOR_SIZE; ++x) {
            size_t index = x + 0 * VECTOR_SIZE + z * VECTOR_SIZE * VECTOR_SIZE;
            m_blocks[index].a = 1;
        }
    }
}

void Editor::mouseMoveEvent(QMouseEvent *event) {
    if (pointBlock(event)) return;

    QPoint center = mapToGlobal(rect().center());
    QPointF mousePos = event->globalPosition();

    if (m_firstMouse) {
        m_lastx = mousePos.x();
        m_lasty = mousePos.y();
        m_firstMouse = false;
        QCursor::setPos(center);
        return;
    }

    float xoffset = mousePos.x() - m_lastx;
    float yoffset = m_lasty - mousePos.y();

    m_lastx = mousePos.x();
    m_lasty = mousePos.y();

    m_cameraController.addYaw(xoffset);
    m_cameraController.addPitch(yoffset);

    QCursor::setPos(center);

    m_lastx = center.x();
    m_lasty = center.y();
}

void Editor::mousePressEvent(QMouseEvent *event) {
    setFocus();

    if (event->button() == Qt::MiddleButton) {
        m_shouldMove = true;
        m_firstMouse = true;
        QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
        QPoint center = mapToGlobal(rect().center());
        QCursor::setPos(center);
        m_lastx = center.x();
        m_lasty = center.y();
        return;
    }

    if (m_pointedBlock != -1) {
        if (event->button() == Qt::RightButton) m_editorMode = EditorMode::Deleting;

        if (event->button() == Qt::LeftButton) {
            switch (m_editorMode) {
            case EditorMode::Creating: {
                m_blocks[m_pointedBlock] = m_currentColor;
            } break;
            case EditorMode::Deleting: {
                m_blocks[m_pointedBlock].a = 0;
            } break;
            }
        }
    }
}

void Editor::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        m_shouldMove = false;
        QApplication::restoreOverrideCursor();
    }

    if (event->button() == Qt::RightButton) m_editorMode = EditorMode::Creating;
}

void Editor::wheelEvent(QWheelEvent *event) {
    float delta = event->angleDelta().y() / 120.0f;
    m_cameraController.setRadius(qBound(10.0f, m_cameraController.radius() - delta, 100.0f));
}

void Editor::onGlInitialized() {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        m_renderer->setBuffer(m_blocks);
    });
    timer->start(16);
}

bool Editor::pointBlock(QMouseEvent *event) {
    if (!m_shouldMove) {
        QPointF mousePos = event->position();

        float ndcX = 2.0f * mousePos.x() / width() - 1.0f;
        float ndcY = 1.0f - 2.0f * mousePos.y() / height();

        QMatrix4x4 viewProjection = m_camera.projection() * m_camera.view();
        QMatrix4x4 invViewProjection = viewProjection.inverted();

        QVector4D nearPoint(ndcX, ndcY, -1.0f, 1.0f);
        QVector4D farPoint(ndcX, ndcY, 1.0f, 1.0f);

        QVector4D nearWorld = invViewProjection * nearPoint;
        QVector4D farWorld = invViewProjection * farPoint;

        nearWorld /= nearWorld.w();
        farWorld /= farWorld.w();

        QVector3D origin = nearWorld.toVector3D();
        QVector3D direction = (farWorld.toVector3D() - origin).normalized();

        int voxelX = int(floor(origin.x()));
        int voxelY = int(floor(origin.y()));
        int voxelZ = int(floor(origin.z()));

        float deltaDistX = (direction.x() == 0) ? 1e30f : abs(1.0f / direction.x());
        float deltaDistY = (direction.y() == 0) ? 1e30f : abs(1.0f / direction.y());
        float deltaDistZ = (direction.z() == 0) ? 1e30f : abs(1.0f / direction.z());

        float sideDistX, sideDistY, sideDistZ;
        int stepX, stepY, stepZ;

        if (direction.x() < 0) {
            stepX = -1;
            sideDistX = (origin.x() - voxelX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (voxelX + 1.0f - origin.x()) * deltaDistX;
        }
        if (direction.y() < 0) {
            stepY = -1;
            sideDistY = (origin.y() - voxelY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (voxelY + 1.0f - origin.y()) * deltaDistY;
        }
        if (direction.z() < 0) {
            stepZ = -1;
            sideDistZ = (origin.z() - voxelZ) * deltaDistZ;
        } else {
            stepZ = 1;
            sideDistZ = ((voxelZ + 1.0f) - origin.z()) * deltaDistZ;
        }

        bool hit = false;
        int maxSteps = 100;
        int lastValidAdjacentBlock = -1;

        auto hasAdjacentBlock = [&](int x, int y, int z) -> bool {
            int offsets[][3] = {{-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}, {0,0,-1}, {0,0,1}};

            for (auto& offset : offsets) {
                int nx = x + offset[0];
                int ny = y + offset[1];
                int nz = z + offset[2];

                if (nx >= 0 && nx < VECTOR_SIZE &&
                    ny >= 0 && ny < VECTOR_SIZE &&
                    nz >= 0 && nz < VECTOR_SIZE) {

                    int neighborIndex = nx + VECTOR_SIZE * (ny + nz * VECTOR_SIZE);
                    if (m_blocks[neighborIndex].a > 0) {
                        return true;
                    }
                }
            }
            return false;
        };

        auto isEdgeBlock = [&](int x, int y, int z) -> bool {
            return (x == 0 || x == VECTOR_SIZE - 1 ||
                    y == 0 || y == VECTOR_SIZE - 1 ||
                    z == 0 || z == VECTOR_SIZE - 1);
        };

        for (int i = 0; i < maxSteps && !hit; ++i) {
            if (voxelX >= 0 && voxelY >= 0 && voxelZ >= 0 &&
                voxelX < VECTOR_SIZE && voxelY < VECTOR_SIZE && voxelZ < VECTOR_SIZE) {

                int index = voxelX + VECTOR_SIZE * (voxelY + voxelZ * VECTOR_SIZE);
                Color color = m_blocks[index];

                switch (m_editorMode) {
                case EditorMode::Creating: {
                    if (hasAdjacentBlock(voxelX, voxelY, voxelZ) || isEdgeBlock(voxelX, voxelY, voxelZ)) {
                        if (color.a > 0) goto loop;
                        lastValidAdjacentBlock = index;
                    }
                } break;
                case EditorMode::Deleting: {
                    if (color.a > 0) {
                        m_pointedBlock = index;
                        hit = true;
                        goto loop;
                    }
                } break;
                }
            }

            if (sideDistX < sideDistY && sideDistX < sideDistZ) {
                sideDistX += deltaDistX;
                voxelX += stepX;
            } else if (sideDistY < sideDistZ) {
                sideDistY += deltaDistY;
                voxelY += stepY;
            } else {
                sideDistZ += deltaDistZ;
                voxelZ += stepZ;
            }
        }
        loop:

        if (!hit && lastValidAdjacentBlock != -1) {
            m_pointedBlock = lastValidAdjacentBlock;
            hit = true;
        }

        if (!hit) {
            m_pointedBlock = -1;
        }

        return true;
    } else {
        m_pointedBlock = -1;
    }

    return false;
}

