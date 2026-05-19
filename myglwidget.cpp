#include "myglwidget.h"
#include "mypainter.h"
#include <QCursor>
#include <QToolTip>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setMouseTracking(true); // Включаем отслеживание движения мыши
}

void MyGLWidget::initializeGL() {//для корректной работы сглаживания
    initializeOpenGLFunctions();
    glClearColor(0.12f, 0.12f, 0.15f, 1.0f); //цвет фона (темно-синий графит)
    glEnable(GL_BLEND); //включаем прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH); //включаем сглаживание линий
}
void MyGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mypainter painter;
    painter.drawCar(width(), height(), selectedZone);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        float x = (float)event->pos().x() / width();
        float y = (float)event->pos().y() / height();

        QString lastZone = selectedZone;

        if (y > 0.65 && y < 0.95 && x > 0.2 && x < 0.8) {
            selectedZone = "Ходовая часть";
        }
        else if (x > 0.60 && x < 0.85) { // морда
            if (y > 0.35 && y < 0.53) {
                selectedZone = "Двигатель";
            }
            else if (y >= 0.53 && y < 0.75) {
                selectedZone = "Электрика";
            }
        }
        else if (x > 0.15 && x <= 0.60 && y > 0.25 && y < 0.65) {
            selectedZone = "Прочее";
        }
        else {
            selectedZone = ""; //сброс при клике в пустоту
        }

        if (selectedZone != lastZone) {
            emit zoneClicked(selectedZone);
            update(); //отображение жёлтого цвета
        }
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    float x = (float)event->pos().x() / width();
    float y = (float)event->pos().y() / height();

    // Определяем зону под курсором для смены курсора
    bool onZone = false;

    if ((x > 0.65 && x < 0.95 && y > 0.3 && y < 0.8) ||
        (x > 0.40 && x < 0.70 && y > 0.65 && y < 0.85) ||
        (x > 0.20 && x < 0.50 && y > 0.65 && y < 0.85) ||
        (x > 0.05 && x <= 0.65 && y > 0.3 && y < 0.8)) {
        onZone = true;
    }

    setCursor(onZone ? QCursor(Qt::PointingHandCursor) : QCursor(Qt::ArrowCursor));
}

void MyGLWidget::leaveEvent(QEvent *event) {
    setCursor(QCursor(Qt::ArrowCursor));
    QOpenGLWidget::leaveEvent(event);
}