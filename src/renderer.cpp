#include "renderer.h"

BlocksRenderer::BlocksRenderer(QWidget* parent, Camera* camera) : QOpenGLWidget(parent) {
    m_renderContext.camera = camera;
}

void BlocksRenderer::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
}

void BlocksRenderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_renderContext.camera->setAspectRatio(w / h);
}

void BlocksRenderer::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if (m_isTransparent) {
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //     glDepthMask(GL_FALSE);
    // } else {
    //     glDisable(GL_BLEND);
    //     glDepthMask(GL_TRUE);
    // }

    update();
}



