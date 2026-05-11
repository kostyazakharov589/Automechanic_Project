#include "mypainter.h"
#include <QtOpenGL>
#include <cmath>

mypainter::mypainter() {}

// Функция рисует закрашенный круг (GL_POLYGON вместо GL_LINE_LOOP)
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

void mypainter::drawCar(int width, int height, const QString& selectedZone) {
    // Вспомогательная лямбда: если зона выбрана - красим в желтый, иначе в базовый
    auto setColor = [&](const QString& targetZone, float r, float g, float b) {
        if (selectedZone == targetZone) {
            glColor3f(1.0f, 0.8f, 0.0f); // Ярко-желтый (Подсветка)
        } else {
            glColor3f(r, g, b); // Базовый цвет детали
        }
    };

    // 1. САЛОН И БАГАЖНИК (Общий осмотр)
    setColor("Общий осмотр", 0.6f, 0.65f, 0.7f); // Серо-голубой кузов
    glBegin(GL_POLYGON);
    glVertex2f(-0.85f, -0.1f); // Низ багажника
    glVertex2f(-0.85f,  0.1f); // Верх багажника
    glVertex2f(-0.5f,   0.1f); // Начало заднего стекла
    glVertex2f(-0.3f,   0.4f); // Задняя часть крыши
    glVertex2f( 0.2f,   0.4f); // Передняя часть крыши
    glVertex2f( 0.5f,   0.1f); // Низ лобового стекла
    glVertex2f( 0.5f,  -0.1f); // Низ двери
    glEnd();

    // 2. ДВИГАТЕЛЬ (Верх капота)
    setColor("Двигатель", 0.5f, 0.55f, 0.6f); // Чуть темнее кузова
    glBegin(GL_POLYGON);
    glVertex2f(0.5f,   0.1f);
    glVertex2f(0.85f,  0.0f);
    glVertex2f(0.85f, -0.05f); // Линия разреза
    glVertex2f(0.5f,  -0.05f);
    glEnd();

    // 3. ЭЛЕКТРИКА (Низ капота и бампер)
    setColor("Электрика", 0.4f, 0.45f, 0.5f); // Еще темнее
    glBegin(GL_POLYGON);
    glVertex2f(0.5f,  -0.05f); // Линия разреза
    glVertex2f(0.85f, -0.05f);
    glVertex2f(0.85f, -0.2f);
    glVertex2f(0.5f,  -0.2f);
    glEnd();

    // Дорисовываем низ кузова к Общему осмотру
    setColor("Общий осмотр", 0.6f, 0.65f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.85f, -0.1f);
    glVertex2f( 0.5f,  -0.1f);
    glVertex2f( 0.5f,  -0.2f);
    glVertex2f(-0.85f, -0.2f);
    glEnd();

    // 4. ХОДОВАЯ ЧАСТЬ (Колеса)
    setColor("Ходовая часть", 0.15f, 0.15f, 0.15f); // Темно-серые шины
    drawCircle(-0.55f, -0.2f, 0.15f, 30); // Задняя шина
    drawCircle( 0.55f, -0.2f, 0.15f, 30); // Передняя шина

    // Диски (внутри шин)
    if (selectedZone == "Ходовая часть") glColor3f(1.0f, 0.9f, 0.3f);
    else glColor3f(0.8f, 0.8f, 0.8f); // Серебристые

    drawCircle(-0.55f, -0.2f, 0.08f, 20); // Задний диск
    drawCircle( 0.55f, -0.2f, 0.08f, 20); // Передний диск
}