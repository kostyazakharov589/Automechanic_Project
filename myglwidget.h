#pragma once
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit MyGLWidget(QWidget *parent = nullptr);

signals:
    void zoneClicked(const QString& zoneName);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString selectedZone;
};