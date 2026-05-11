#pragma once
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit MyGLWidget(QWidget *parent = nullptr);

signals:
    // Сигнал, который отправляется при клике на определенную зону
    void zoneClicked(const QString& zoneName);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Перехват кликов мыши
    void mousePressEvent(QMouseEvent *event) override;
private:
        QString selectedZone;
};