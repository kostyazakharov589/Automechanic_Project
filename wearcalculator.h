#ifndef WEARCALCULATOR_H
#define WEARCALCULATOR_H
#include <QString>
#include <QList>
#include <QMap>

struct CalcSeasonData {//для сезонов структура
    int year;
    QString season;
    double km;
    QString roadType;
};

struct CalcReplaceData {//структуура для калькулятора замен
    QString partName;
    int year;
    QString season;
};

struct PartResult {//результат по запчастям
    QString partName;
    int wearPercent;
};
struct PredictionData {//для прогноза будущего износа
    QString partName;
    int daysLeft;
    double remainingKm;
    int wearPercent;
};
class WearCalculator {
public:
    WearCalculator();
    void setCatalog(const QMap<QString, int>& catalog);//загрузка каталога
    QList<PartResult> calculate(int baseMileage, const QList<CalcSeasonData>& history, const QList<CalcReplaceData>& replacements);//подсчёт для калькултяора
    QList<PredictionData> predictService(const QList<PartResult>& results, double dailyKm);//расчёт будущего износа
private:
    QMap<QString, int> partCatalog; //каталог из частей
    QMap<QString, double> roadCoefs; //дорожные коэфы
    QMap<QString, double> seasonCoefs; //сезонные коэфы
    double calculateDistributedKm(int baseMileage, const QList<CalcSeasonData>& history);//для подсчёта функции
};


#endif // WEARCALCULATOR_H



