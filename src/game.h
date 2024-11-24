#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QKeyEvent>

class Game : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent *event) override;

signals:
    void backToMenu();
};
