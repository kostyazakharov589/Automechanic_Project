#include "mypainter.h"
#include <QtOpenGL>
#include <cmath>

mypainter::mypainter() {}


// Функция рисует круг через веер треугольников
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);//избавляет от артефактов
    glVertex2f(cx, cy); // Центральная точка веера
    for (int ii = 0; ii <= num_segments; ii++) { // ВАЖНО: <= чтобы замкнуть последнюю точку с первой
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

void mypainter::drawCar(int width, int height, const QString& selectedZone) {
    auto setColor = [&](const QString& targetZone, float r, float g, float b) {
        if (selectedZone == targetZone) glColor3f(1.0f, 0.8f, 0.0f); // активный желтый
        else glColor3f(r, g, b); // обычный
    };

    // сглаживание
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    setColor("Прочее", 0.6f, 0.62f, 0.65f); // применяем подсветку прочего ко всему кузову
    glBegin(GL_POLYGON);
    glVertex2f(-0.60f, -0.22f); // низ багажника
    glVertex2f(-0.64f,  0.05f); // край багажника
    glVertex2f(-0.45f,  0.08f); // начало задней стойки

    // плавная крыша сзади
    for(float t = 0; t <= 1.0f; t += 0.1f) {
        float x = -0.45f + 0.26f * t;
        float y =  0.08f + 0.27f * sin(t * 1.5708f);
        glVertex2f(x, y);
    }

    glVertex2f( 0.08f,  0.35f); // верх лобового
    glVertex2f( 0.34f,  0.08f); // низ лобового
    glVertex2f( 0.34f, -0.22f); // низ кузова
    glEnd(); // закрыли полигон кузова!

    glColor3f(0.18f, 0.2f, 0.25f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.41f, 0.11f); // нижний левый угол

    // изгиб заднего стекла
    for(float t = 0; t <= 1.0f; t += 0.1f) {
        float x = -0.41f + 0.22f * t;
        float y =  0.11f + 0.21f * sin(t * 1.5708f);
        glVertex2f(x, y);
    }

    glVertex2f(0.06f, 0.32f); // верх лобового
    glVertex2f(0.29f, 0.11f); // низ лобового
    glEnd();

    setColor("Прочее", 0.6f, 0.62f, 0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.06f, 0.11f);
    glVertex2f(-0.01f, 0.11f);
    glVertex2f(-0.01f, 0.33f);
    glVertex2f(-0.06f, 0.33f);
    glEnd();

    // блик на лобовом стекле
    glColor4f(1.0f, 1.0f, 1.0f, 0.15f);
    glBegin(GL_POLYGON);
    glVertex2f(0.06f, 0.25f);
    glVertex2f(0.10f, 0.25f);
    glVertex2f(0.19f, 0.15f);
    glVertex2f(0.15f, 0.15f);
    glEnd();

    // полоса на дверях
    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.60f, 0.0f);   glVertex2f(0.33f, 0.0f);
    glVertex2f(0.32f, -0.015f); glVertex2f(-0.56f, -0.015f);
    glEnd();

    setColor("Двигатель", 0.55f, 0.57f, 0.60f);
    glBegin(GL_POLYGON);
    glVertex2f(0.338f, 0.082f); // стык с лобовым стеклом
    glVertex2f(0.64f, -0.02f);  // нос
    glVertex2f(0.64f, -0.12f);  // низ капота
    glVertex2f(0.338f, -0.12f); // стык с дверью
    glEnd();

    // декоративная линия капота
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(0.34f, 0.08f); glVertex2f(0.64f, -0.02f);
    glEnd();

    setColor("Электрика", 0.50f, 0.52f, 0.55f);
    glBegin(GL_POLYGON);
    glVertex2f(0.338f, -0.119f); // стык с капотом
    glVertex2f(0.64f, -0.119f);
    glVertex2f(0.62f, -0.22f);   // низ бампера
    glVertex2f(0.338f, -0.22f);  // стык с кузовом снизу
    glEnd();

    // фара
    if (selectedZone == "Электрика") glColor3f(1.0f, 1.0f, 0.6f);
    else glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(0.56f, -0.02f); glVertex2f(0.64f, -0.02f);
    glVertex2f(0.62f, -0.08f); glVertex2f(0.55f, -0.08f);
    glEnd();

    setColor("Ходовая часть", 0.2f, 0.2f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.41f, -0.18f); glVertex2f(0.41f, -0.18f);
    glVertex2f(0.41f, -0.22f);  glVertex2f(-0.41f, -0.22f);
    glEnd();

    // арки колес
    glColor3f(0.05f, 0.05f, 0.05f);
    drawCircle(-0.41f, -0.22f, 0.16f, 40);
    drawCircle( 0.41f, -0.22f, 0.16f, 40);

    // резина
    glColor3f(0.12f, 0.12f, 0.12f);
    drawCircle(-0.41f, -0.22f, 0.14f, 40);
    drawCircle( 0.41f, -0.22f, 0.14f, 40);

    // диски
    if (selectedZone == "Ходовая часть") glColor3f(1.0f, 0.8f, 0.0f);
    else glColor3f(0.6f, 0.65f, 0.7f);
    drawCircle(-0.41f, -0.22f, 0.09f, 30);
    drawCircle( 0.41f, -0.22f, 0.09f, 30);

    // гайка
    glColor3f(0.2f, 0.2f, 0.2f);
    drawCircle(-0.41f, -0.22f, 0.03f, 15);
    drawCircle( 0.41f, -0.22f, 0.03f, 15);

    // блик
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    drawCircle(-0.38f, -0.19f, 0.025f, 15);
    drawCircle( 0.44f, -0.19f, 0.025f, 15);
}