#ifndef WEARCALCULATOR_H
#define WEARCALCULATOR_H
#include <QString>
#include <QList>
#include <QMap>

struct CalcSeasonData {
    int year;
    QString season;
    int km;
    QString roadType;
};

struct CalcReplaceData {
    QString partName;
    int year;
    QString season;
};

struct PartResult {
    QString partName;
    int wearPercent;
};

class WearCalculator {
public:
    WearCalculator();
    void setCatalog(const QMap<QString, int>& catalog);//загрузка каталога
    QList<PartResult> calculate(int baseMileage, const QList<CalcSeasonData>& history, const QList<CalcReplaceData>& replacements);//подсчёт для калькултяора
    
private:
    QMap<QString, int> partCatalog; //каталог из частей
    QMap<QString, double> roadCoefs; //дорожные коэфы
    QMap<QString, double> seasonCoefs; //сезонные коэфы
    double calculateDistributedKm(int baseMileage, const QList<CalcSeasonData>& history);//для подсчёта функции
};


#endif // WEARCALCULATOR_H



