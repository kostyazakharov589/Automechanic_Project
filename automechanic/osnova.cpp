#include "osnova.h"
#include "ui_osnova.h"
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include "myglwidget.h"
#include <QUrl>
#include <QDesktopServices>
#include <QProgressBar>
#include <databasemanager.h>

osnova::osnova(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::osnova)
{
    ui->setupUi(this);

    /////////////////////////////////////////////////////////////////////////////
    if (dbManager.connectToDataBase()) {//здесь мы реализуем логику подключения к бд и вывод на второй странице
        qDebug() << "База данных успешно подключена";

        ui->profileSelector->addItems(dbManager.getSavedProfiles());//выводим подключённые жесонные профили


        QStringList makeList = dbManager.getCarMakes(); //подключаем марки и выводим их
        QCompleter *makeCompleter = new QCompleter(makeList, this);//создаём ядро из профилей
        makeCompleter->setCaseSensitivity(Qt::CaseInsensitive);//сделать все буквы однострочными
        ui->makeInput->setCompleter(makeCompleter);//прикручиваем к полю ядро


        QCompleter *modelCompleter = new QCompleter(this);// для моделей
        modelCompleter->setCaseSensitivity(Qt::CaseInsensitive);//сделать все буквы однострочными
        ui->modelInput->setCompleter(modelCompleter);//прикручиваем к полю ядро

        ui->modelInput->setEnabled(false); //всё заблокировано
        ui->yearInput->setEnabled(false);
        ui->mileageInput->setEnabled(false);
        ui->btnNext->setEnabled(false);


        connect(ui->makeInput, &QLineEdit::textChanged, this, [this, modelCompleter]() { //реализация ввода марки с удалением и подключением и прочим
            ui->modelInput->clear(); // Стираем старую модель
            ui->yearInput->clear();  // Стираем старые года

            // СБРОС: блокируем нижние поля, если марку стерли или изменили
            ui->modelInput->setEnabled(false);
            ui->yearInput->setEnabled(false);
            ui->mileageInput->setEnabled(false);
            ui->btnNext->setEnabled(false);

            QString currentMake = ui->makeInput->text(); //ввод
            QStringList models = dbManager.getCarModels(currentMake); //вывод моделей машин

            if (!models.isEmpty()) { // Если модели найдены, то разблокируем поле
                modelCompleter->setModel(new QStringListModel(models, modelCompleter)); // Загружаем найденные модели в подсказки
                ui->modelInput->setEnabled(true);
            }
        });

        connect(ui->modelInput, &QLineEdit::textChanged, this, [this]() {//ищем модель после ввода марки и то же самое, что и в предыдущем с поиском и удалением
            QString currentMake = ui->makeInput->text();//ищем год выпуска после ввода модели
            QString currentModel = ui->modelInput->text();

            const QList<int> years = dbManager.getCarYears(currentMake, currentModel);//подключаем список из годов
            ui->yearInput->clear();


            ui->yearInput->setEnabled(false);//если модель поменялась, то блокируем, то что ниже
            ui->mileageInput->setEnabled(false);
            ui->btnNext->setEnabled(false);

            if (!years.isEmpty()) { //проверяем, что список годов не пуст
                for (int y : years) { //перебираем все готовые года, которые отдала база
                    ui->yearInput->addItem(QString::number(y));//выводим года
                }

                // Разблокируем последние элементы
                ui->yearInput->setEnabled(true);
                ui->mileageInput->setEnabled(true);
                ui->btnNext->setEnabled(true);
            }
        });
    }
    ///////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////
    ui->detailsBox->setVisible(false);
    ui->replaceBox->setVisible(false);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->btnAddNewCar, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->profileSelector, &QComboBox::activated, this, [this](int) {
        QString selectedName = ui->profileSelector->currentText();
        loadProfile(selectedName); // подгружаем наши расчёты в память
        calculateFinalWear();
        ui->openGLWidget->update();
        ui->stackedWidget->setCurrentIndex(4); // А потом показываем результат
    });
    //

    //2 страница
    connect(ui->btnBackToCar, &QPushButton::clicked, this, [this]() {
        //prepareSeasonsPage(); вроде можно убрать
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->btnNext, &QPushButton::clicked, this, [this]() {
        prepareSeasonsPage();//вывод большой функции с сезонами
        ui->stackedWidget->setCurrentIndex(2);
    });
    //

    //3 страница
    connect(ui->btnBackToCar2, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->radioRefine, &QRadioButton::toggled, this, [this](bool checked) {
        ui->detailsBox->setVisible(checked);
    });

    connect(ui->btnCalculate, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        prepareReplacementsPage();
    });
    connect(ui->btnGenerateTimeline, &QPushButton::clicked, this, &osnova::generateTimeline);

    connect(ui->radioSkip, &QRadioButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        prepareReplacementsPage();
    });
    //

    //4 страница
    connect(ui->btnBackToSeasons, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(ui->radioReplaceYes, &QRadioButton::toggled, this, [this](bool checked) {
        ui->replaceBox->setVisible(checked);
    });
    connect(ui->btnAddReplacement, &QPushButton::clicked, this, [this](){
        QString part = ui->cmbReplacedPart->currentText();
        QString season = ui->cmbReplaceSeason->currentText();
        int year = ui->cmbReplaceYear->currentText().toInt();

        if (part.isEmpty() || season.isEmpty() || year == 0) {//если какое-то поле пустое, то не выводим замену
            return;
        }

        ReplacedPart rp;
        rp.partName = part;
        rp.season = season;
        rp.year = year;
        replacedPartsList.append(rp);//сохранение в память

        QString displayText = QString("%1 — замена: %2 %3 г.").arg(part, season, QString::number(year));//вывод замены на экран
        ui->listReplacements->addItem(displayText);

        qDebug() << "Успешно добавлено:" << displayText;
    });
    connect(ui->radioReplaceSkip, &QRadioButton::clicked, this, [this]() {//две одинаковых штуки для сохранения в жесоне при переходе на 5 страницу
        ui->stackedWidget->setCurrentIndex(4);
        QString make = ui->makeInput->text();
        QString model = ui->modelInput->text();
        int year = ui->yearInput->currentText().toInt();
        int mileage = ui->mileageInput->value();//превращаем введённое в нормальные типы данных для жесона

        saveProfileToJson(make, model, year, mileage);//сохраняем в жесон


        ui->profileSelector->clear();//динамически обновляем список
        ui->profileSelector->addItems(dbManager.getSavedProfiles());//добавляем в список наших профиоей
    });

    connect(ui->btnGoToResults, &QPushButton::clicked, this, [this]() {//две одинаковых штуки для сохранения в жесоне при переходе на 5 страницу
        ui->stackedWidget->setCurrentIndex(4);
        calculateFinalWear();
        QString make = ui->makeInput->text();
        QString model = ui->modelInput->text();
        int year = ui->yearInput->currentText().toInt();
        int mileage = ui->mileageInput->value();//превращаем введённое в нормальные типы данных для жесона

        saveProfileToJson(make, model, year, mileage);//превращаем введённое в нормальные типы данных для жесона

        ui->profileSelector->clear();//динамически обновляем список
        ui->profileSelector->addItems(dbManager.getSavedProfiles());//добавляем в список наших профиоей

    });
    //

    //5 страница
    connect(ui->btnBackToHistory, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        prepareReplacementsPage();
    });

    connect(ui->btnRestart, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->openGLWidget, &MyGLWidget::zoneClicked, this, [this](const QString& zoneName) {
        if (finalResults.isEmpty()) {
            QMessageBox::warning(this, "Пусто", "Сначала выполните расчет износа!");
            return;
        }

        // очищаем список под экраном
        ui->listWidgetDetails->clear();

        // определяем какие категории запчастей искать в этой зоне
        QStringList targetCategories;
        if (zoneName == "Двигатель") {
            targetCategories << "Двигатель и ГРМ" << "Охлаждение" << "Фильтры";
        } else if (zoneName == "Ходовая часть") {
            targetCategories = {"Тормозная система", "Подвеска и рулевое", "Трансмиссия"};
        } else {
            targetCategories << "Электрика и прочее";
        }

        //формируем список для вывода
        bool partsFound = false;
        for (const PartResult& res : finalResults) {
            // Проверяем, входит ли деталь в одну из категорий выбранной зоны
            // (Для этого можно создать вспомогательную функцию или просто проверить по списку)
            if (isPartInZone(res.partName, targetCategories)) {
                partsFound = true;

                // Создаем элемент списка
                QString status = QString("%1: %2%").arg(res.partName, QString::number(res.wearPercent));
                QListWidgetItem* item = new QListWidgetItem(status);

                // Цветовая индикация согласно математике шага 3
                if (res.wearPercent >= 80) item->setForeground(Qt::red);
                else if (res.wearPercent >= 50) item->setForeground(QColor(255, 165, 0)); // Оранжевый

                ui->listWidgetDetails->addItem(item);
            }
        }

        if (!partsFound) {
            ui->listWidgetDetails->addItem("В этой зоне все детали в норме.");
        }
});
connect(ui->listWidgetDetails, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        QString partName = item->text().split(":").first();
        QString searchQuery = ui->makeInput->text() + " " + ui->modelInput->text() + " " + partName;
        QDesktopServices::openUrl(QUrl("https://www.avito.ru/all/zapchasti_i_aksessuary?q=" + QUrl::toPercentEncoding(searchQuery)));
});
///////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////

}

osnova::~osnova()
{
    delete ui;
}
bool osnova::isPartInZone(const QString& partName, const QStringList& categories) {//проверяем деталь на соотв. списку
    static QMap<QString, QStringList> catalogMap = {
        {"Фильтры", {"Масляный фильтр", "Воздушный фильтр", "Салонный фильтр", "Топливный фильтр"}},
        {"Тормозная система", {"Колодки передние", "Колодки задние", "Диски тормозные передние", "Суппорт передний"}},
        {"Подвеска и рулевое", {"Амортизатор передний", "Пружина передняя", "Стойка стабилизатора", "Рулевой наконечник"}},
        {"Двигатель и ГРМ", {"Ремень ГРМ", "Водяная помпа", "Свеча зажигания", "Ролик натяжителя"}},
        {"Охлаждение", {"Радиатор охлаждения", "Антифриз", "Термостат", "Патрубок радиатора"}},
        {"Трансмиссия", {"Сцепление", "Масло трансмиссионное", "ШРУС наружный"}},
        {"Электрика и прочее", {"Аккумулятор", "Генератор", "Стартер", "Лампа ближнего света"}}
    };

    for (const QString& cat : categories) {
        if (catalogMap[cat].contains(partName)) return true;
    }
    return false;
}
void osnova::saveProfileToJson(const QString &make, const QString &model, int year, int mileage)//реализация логичики функции сохранения профиля в жесон
{
    QJsonObject obj;
    obj["make"] = make;
    obj["model"] = model;
    obj["year"] = year;
    obj["mileage"] = mileage;//сохраняем базовый профиль
    double avgKm = calculateEffectiveMileage();//тут идёт отдельный подсчёт среднего пробега в неведённых

    obj["startMonthIdx"] = ui->cmbStartMonth->currentIndex();
    obj["endMonthIdx"] = ui->cmbEndMonth->currentIndex();
    obj["startYear"] = ui->spinStartYear->currentText().toInt();
    obj["endYear"] = ui->spinEndYear->currentText().toInt();//сохраняем начало и конец использования нашей машины
    //это всё сохранём в жесон
    QJsonArray replacementsArray;
    for (const ReplacedPart &rp : replacedPartsList) {
        QJsonObject repObj;
        repObj["partName"] = rp.partName;
        repObj["year"] = rp.year;
        repObj["season"] = rp.season;
        replacementsArray.append(repObj);
    }
    obj["replacements"] = replacementsArray;
    QJsonArray timelineArray; // Создаем массив для истории

    for (const SeasonEntry &entry : generatedSeasons) {//т.к. у нас пробег по сезонам генерится от пользователя, то перебираем по колву введённого
        QJsonObject seasonObj;
        seasonObj["year"] = entry.year;
        seasonObj["season"] = entry.seasonName;
        int val = entry.spinKm->value();
        int finalKm = val;
        if (val == 0) {
            finalKm = (int)avgKm;//для среднего арифм, пока не доделоано
        }
        seasonObj["km"] = finalKm;
        seasonObj["road_k"] = entry.cmbRoad->currentText(); //собираем сезоны

        timelineArray.append(seasonObj); // Добавляем сезон в историю
    }

    obj["timeline"] = timelineArray; //прикрепляем историю к профилю

    QJsonDocument doc(obj);
    QDir().mkpath("profiles");//используется для создания всей цепочки вложенных директорий по указанному пути

    QString fileName = QString("%1-%2(%3).json").arg(make, model, QString::number(year));//сохраняем
    QFile file("profiles/" + fileName);//сам репозиторий, поменять!

    if (file.open(QIODevice::WriteOnly)) {//открываем и записываем в жесон
        file.write(doc.toJson());
        file.close();
        qDebug() << "Профиль успешно сохранен со всеми сезонами в " << fileName;
    } else {
        qDebug() << "Ошибка! Не удалось создать файл " << fileName;//проверка на всякий
    }
}

void osnova::prepareSeasonsPage() {//подготовка страницы с сезонами года
    int carYear = ui->yearInput->currentText().toInt();//берём год выпуска машины, выбранную юзером

    int currentYear = QDate::currentDate().year();//какой текущий год?

    ui->spinStartYear->clear();//очищаем от старых данных
    ui->spinEndYear->clear();

    for (int y = carYear; y <= currentYear; ++y) {//создаём списки с годами владения
        ui->spinStartYear->addItem(QString::number(y));
        ui->spinEndYear->addItem(QString::number(y));//начало и конец соответственно
    }
}


QStringList getActiveSeasonsForYear(int currentYear, int startYear, int startMonth, int endYear, int endMonth) {//для сезонов года большой кулист
    QStringList activeSeasons;//подключаем сезоны


    int mStart = (currentYear == startYear) ? startMonth : 1; //если год первый, то начинаем с месяца покупки, иначе с января
    int mEnd   = (currentYear == endYear)   ? endMonth   : 12; //если год последний, то заканчиваем месяцем продажи, иначе декабрем

    auto hasMonth = [&](int m) {
        return m >= mStart && m <= mEnd;
    };//проверяем на соотвествие месяцам

    if (hasMonth(1) || hasMonth(2) || hasMonth(12)) activeSeasons << "Зима";//присваиеваем сезоны в зависимости от номера месяца в списке
    if (hasMonth(3) || hasMonth(4) || hasMonth(5))  activeSeasons << "Весна";
    if (hasMonth(6) || hasMonth(7) || hasMonth(8))  activeSeasons << "Лето";
    if (hasMonth(9) || hasMonth(10) || hasMonth(11)) activeSeasons << "Осень";

    return activeSeasons;
}
void osnova::generateTimeline()//генерируем сезоны исходя из введенных дат владения
{

    QLayoutItem *child;
    while ((child = ui->verticalLayout_2->takeAt(0)) != nullptr) {//для очищения виджетов, хз
        if (child->widget())
            delete child->widget();
        delete child;
    }
    generatedSeasons.clear();//очищаем от предыдущей генерации

    // Берем Месяцы и Года из интерфейса
    int startMonth = ui->cmbStartMonth->currentIndex() + 1;
    int endMonth   = ui->cmbEndMonth->currentIndex() + 1;
    int startYear  = ui->spinStartYear->currentText().toInt();
    int endYear    = ui->spinEndYear->currentText().toInt();


    if (endYear < startYear || (endYear == startYear && endMonth < startMonth)) {
        qDebug() << "Ошибка: Период конца указан раньше периода начала!";//защита на всякий случай
        return;
    }

    QStringList roadTypes = {"Город", "Трасса", "Грунтовые", "Бездорожье", "Гараж"};//типы дорог

    for (int y = startYear; y <= endYear; ++y) {//генерация блоков по годам. начинаем со старта и до конца

        QStringList activeSeasons = getActiveSeasonsForYear(y, startYear, startMonth, endYear, endMonth);//получаем список из введённых данных
        if (activeSeasons.isEmpty()) continue;

        QGroupBox *yearBox = new QGroupBox("                                                             " + QString::number(y) + " год", this);//создаём виджет из года и сезона
        QVBoxLayout *yearLayout = new QVBoxLayout(yearBox);

        for (const QString &seasonName : activeSeasons) {
            QWidget *rowWidget = new QWidget(yearBox);//создаём виджеты для отображения годов
            QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(5, 5, 5, 5);//задаём отступы

            QLabel *lblSeason = new QLabel(seasonName, rowWidget);//выгрузка сезонов
            lblSeason->setMinimumWidth(60);//минимальная ширина

            QSpinBox *spinKm = new QSpinBox(rowWidget);//выгрузка км
            spinKm->setMaximum(500000);//максимальный
            spinKm->setSuffix(" км");//ввод км

            QComboBox *cmbRoad = new QComboBox(rowWidget);//создаём комбобокс для ввода дорог
            cmbRoad->addItems(roadTypes);
            rowLayout->addWidget(lblSeason);
            rowLayout->addWidget(spinKm);
            rowLayout->addWidget(cmbRoad);
            yearLayout->addWidget(rowWidget);//добавляем виджеты по дорогам и прочим для ввода

            SeasonEntry entry;
            entry.year = y;
            entry.seasonName = seasonName;
            entry.spinKm = spinKm;
            entry.cmbRoad = cmbRoad;
            generatedSeasons.append(entry); // Сохраняем для JSON
        }

        ui->verticalLayout_2->addWidget(yearBox);//боксы годов
    }

    ui->verticalLayout_2->addStretch();//занимает всё пространтсва, для норм отображения можно и менять
}

void osnova::loadProfile(const QString &profileName) {//загружаем профили машин
    QFile file("profiles/" + profileName + ".json");//берём название профиля из сохранённого типа названий
    if (!file.open(QIODevice::ReadOnly)) return;//перепроверка
    QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();//открываем на чтение
    file.close();//закрываем

    ui->makeInput->setText(obj["make"].toString());//заполняем всё на первой странице
    ui->modelInput->setText(obj["model"].toString());//модель
    ui->yearInput->clear();//добавляем год чтобы ничего не ломалось
    ui->yearInput->addItem(QString::number(obj["year"].toInt()));
    ui->yearInput->setCurrentIndex(0);
    ui->mileageInput->setValue(obj["mileage"].toInt());
    prepareSeasonsPage();//списки на странице сезонов

    ui->cmbStartMonth->setCurrentIndex(obj["startMonthIdx"].toInt());//восстанавливаем всё из жесона на странице сезонов
    ui->cmbEndMonth->setCurrentIndex(obj["endMonthIdx"].toInt());
    ui->spinStartYear->setCurrentText(QString::number(obj["startYear"].toInt()));
    ui->spinEndYear->setCurrentText(QString::number(obj["endYear"].toInt()));

    generateTimeline();//отрисовывем наши сезоны

    QJsonArray timeline = obj["timeline"].toArray();//извлекаем массив из нашего файла
    for (int i = 0; i < timeline.size() && i < generatedSeasons.size(); ++i) {//делаем генерация по кол-ву введённого
        QJsonObject sObj = timeline[i].toObject();
        generatedSeasons[i].spinKm->setValue(sObj["km"].toInt());//км
        generatedSeasons[i].cmbRoad->setCurrentText(sObj["road_k"].toString());//тип дороги
    }
    replacedPartsList.clear();
    replacedPartsList.clear();
    ui->listReplacements->clear();
    QJsonArray replacementsArray = obj["replacements"].toArray();
    for (int i = 0; i < replacementsArray.size(); ++i) {
        QJsonObject repObj = replacementsArray[i].toObject();
        ReplacedPart rp;
        rp.partName = repObj["partName"].toString();
        rp.year = repObj["year"].toInt();
        rp.season = repObj["season"].toString();
        replacedPartsList.append(rp);

        ui->listReplacements->addItem(QString("%1 — замена: %2 %3 г.")
                                          .arg(rp.partName, rp.season, QString::number(rp.year)));
    }
}

double osnova::calculateEffectiveMileage() {//функция подсчёта среднего пробега по сезонам
    int totalBaseMileage = ui->mileageInput->value();
    double totalUserEntered = 0;
    int emptySeasonsCount = 0;

    for (const SeasonEntry &entry : generatedSeasons) { //подсчитываем то, что пользователь уже ввёл
        int val = entry.spinKm->value();
        if (val > 0) {
            totalUserEntered += val;

        }
        else {
            emptySeasonsCount++;
        }
    }

    double distributedKm = 0;
    if (emptySeasonsCount > 0 && totalBaseMileage > totalUserEntered) {
        distributedKm = (double)(totalBaseMileage - totalUserEntered) / emptySeasonsCount; //вычисление среднего пробега для невведённых полей
    }

    return distributedKm; //возвращаем
}
void osnova::prepareReplacementsPage() {
    ui->cmbReplaceYear->clear();
    ui->cmbReplacedPart->clear();
    ui->cmbReplaceSeason->clear(); // Очищаем сезоны

    ui->cmbReplaceSeason->addItems({"Зима", "Весна", "Лето", "Осень"});//добавляем сезоны кодом

    int startYear = ui->spinStartYear->currentText().toInt();//года
    int endYear = ui->spinEndYear->currentText().toInt();
    for(int y = startYear; y <= endYear; ++y) {
        ui->cmbReplaceYear->addItem(QString::number(y));
    }

    QString make = ui->makeInput->text();//выгружаем детали из базы
    QString model = ui->modelInput->text();
    QMap<QString, int> carParts = dbManager.getCarPartsResource(make, model);//сама база
    ui->cmbReplacedPart->addItems(carParts.keys());

    ui->listReplacements->clear();//защита от дубликатов
    for (const ReplacedPart &rp : replacedPartsList) {
        ui->listReplacements->addItem(QString("%1 — замена: %2 %3 г.")
                                          .arg(rp.partName, rp.season, QString::number(rp.year)));//для вывода
    }
}

void osnova::calculateFinalWear() { //отображение износа машин
    if (!ui->resultsLayout->layout()) {//защита от вылетов, если слоя нет, то он создаётся сам
        ui->resultsLayout->setLayout(new QVBoxLayout());
    }

    QLayoutItem *child;
    if (ui->resultsLayout->layout()) {
        while ((child = ui->resultsLayout->layout()->takeAt(0)) != nullptr) {//очищаем старые полоски
            if (child->widget()) delete child->widget();
            delete child;
        }
    }

    QList<CalcSeasonData> history;//вот тут добавление сезонов, а далее замен
    for (const auto &entry : generatedSeasons) {
        history.append({entry.year, entry.seasonName, entry.spinKm->value(), entry.cmbRoad->currentText()});
    }

    QList<CalcReplaceData> replacements;
    for (const auto &rp : replacedPartsList) {
        replacements.append({rp.partName, rp.year, rp.season});
    }

    WearCalculator calculator; //вызов калькулятора

    QMap<QString, int> catalog = dbManager.getCarPartsResource(ui->makeInput->text(), ui->modelInput->text());//подгружаем детали из бд
    calculator.setCatalog(catalog);//наш каталог

    finalResults = calculator.calculate(ui->mileageInput->value(), history, replacements);//сохранём в опенгл

    for (const auto& res : finalResults) { //отрисовываем прогресс износа
        QWidget *row = new QWidget();
        QHBoxLayout *hLay = new QHBoxLayout(row);

        QLabel *lbl = new QLabel(res.partName);
        lbl->setFixedWidth(200);

        QProgressBar *pb = new QProgressBar();
        pb->setValue(res.wearPercent);

        if (res.wearPercent > 80) pb->setStyleSheet("QProgressBar::chunk { background-color: #ff4d4d; }");//красный
        else if (res.wearPercent > 50) pb->setStyleSheet("QProgressBar::chunk { background-color: #ffa500; }");//жёлтый
        else pb->setStyleSheet("QProgressBar::chunk { background-color: #2ecc71; }");//зелёный

        hLay->addWidget(lbl);
        hLay->addWidget(pb);//добавляем виджеты
        ui->resultsLayout->layout()->addWidget(row);//выводим в лейлаут
    }
    static_cast<QVBoxLayout*>(ui->resultsLayout->layout())->addStretch();//для бокса лейлаутов
}