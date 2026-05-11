#include "myglwidget.h"
#include "mypainter.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions();
}

void MyGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MyGLWidget::paintGL() {
    // Темно-синий фон
    glClearColor(0.15f, 0.2f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Рисуем машину через чистый OpenGL
    mypainter painter;
    painter.drawCar(width(), height(), selectedZone);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        float x = (float)event->pos().x() / width();
        float y = (float)event->pos().y() / height();

        QString lastZone = selectedZone;

        // 1. КАПОТ (Правая часть экрана)
        if (x > 0.65 && x < 0.95 && y > 0.3 && y < 0.8) {
            // Делим по горизонтали
            if (y < 0.55) {
                selectedZone = "Двигатель"; // Верх (ГРМ, Фильтры)
            } else {
                selectedZone = "Электрика"; // Низ (Генератор, Фары)
            }
        }
        else if (x > 0.05 && x <= 0.65 && y > 0.3 && y < 0.8) {
            selectedZone = "Ходовая часть";
        }

        if (selectedZone != lastZone) {
            emit zoneClicked(selectedZone);
            update(); // Запускает paintGL() и перерисовывает полигоны
        }
    }
}