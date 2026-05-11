#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QStringList>

QStringList getCarMakes();

class databasemanager
{
public:
    databasemanager();//коснтуркетор
    ~databasemanager();//деструктор
    bool connectToDataBase();//подключение к бд
    QStringList getCarModels(const QString &make);
    QList<int> getCarYears(const QString &make, const QString &model);//получаем года машин
    QStringList getSavedProfiles();//профили
    QStringList getCarMakes();//получение
    QMap<QString, int> getCarPartsResource(const QString &make, const QString &model);//забираем детали машин
private:
    bool createTable();//создаём таблицу

};

#endif
