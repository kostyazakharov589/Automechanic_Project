#ifndef OSNOVA_H
#define OSNOVA_H
#include <QMainWindow>
#include <QMainWindow>
#include "databasemanager.h"
#include <QList>
#include <QSpinBox>
#include <QComboBox>
#include "wearcalculator.h"
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui {
class osnova;
}
QT_END_NAMESPACE
struct SeasonEntry {//структура для разбивки по сезонам
    int year;
    QString seasonName;
    QDoubleSpinBox* spinKm;  //указатели на км и тип дороги
    QComboBox* cmbRoad;
};
struct ReplacedPart {//структура для замены
    QString partName;
    int year;
    QString season;
};
class osnova : public QMainWindow
{
    Q_OBJECT

public:
    explicit osnova(QWidget *parent = nullptr);
    ~osnova() override;

private:
    Ui::osnova *ui;
    databasemanager dbManager;//подключаем дб машин
    void saveProfileToJson(const QString &make, const QString &model, int year, int mileage); //жесон профилди
    void prepareSeasonsPage();//сезоны на 3 странице
    QList<SeasonEntry> generatedSeasons;//лист из сезонов
    void generateTimeline();//для подсчёта дат и сезонов
    void loadProfile(const QString &profileName);//загрузка профилей машин
    double calculateEffectiveMileage();//подсчёт пробега
    QList<ReplacedPart> replacedPartsList;//лист детайлей для замены
    void prepareReplacementsPage();//ф для замены деталей
    void calculateFinalWear();//подсчёт замен
    QList<PartResult> finalResults;//opengl
    bool isPartInZone(const QString& partName, const QStringList& categories);//роль маршрутизатора между зоной клика по 3D-модели автомобиля и конкретной запчастью из рассчитанного списка
    void exportReport();//делаем репорт по нашей машине
    void deleteSelectedProfile();//ф для удаления профилей машинок
    void showTOPrediction(); // рассчёт даты прогнозируемого ремонта
    void openMapsForService(); //открываем поиск сто в картах
};
#endif // OSNOVA_H
