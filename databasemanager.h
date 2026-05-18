#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QStringList>


class databasemanager
{
public:
    databasemanager();//коснтуркетор
    ~databasemanager();//деструктор
    bool connectToDataBase();//подключение к бд
    QStringList getCarModels(const QString &make);
    QList<int> getCarYears(const QString &make, const QString &model);//получаем года машин
    QStringList getSavedProfiles(const QString &username);//профили + юзер
    QStringList getCarMakes();//получение
    QMap<QString, int> getCarPartsResource(const QString &make, const QString &model);//забираем детали машин
    bool registerUser(const QString &username, const QString &password); //логин и регистрация
    bool loginUser(const QString &username, const QString &password);
private:
    bool createTable();//создаём таблицу

};

#endif
