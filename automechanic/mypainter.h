#ifndef MYPAINTER_H
#define MYPAINTER_H
#include <QString>

class mypainter
{
public:
    mypainter();
    void drawCar(int width, int height, const QString& selectedZone);
};

#endif // MYPAINTER_H