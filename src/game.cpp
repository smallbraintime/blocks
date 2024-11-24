#include "game.h"

#include <QApplication>

Game::Game(QWidget *parent) : QOpenGLWidget(parent) {}

void Game::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Game::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Game::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glEnd();
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        emit backToMenu();
    } else {
        QWidget::keyPressEvent(event);
    }
}
