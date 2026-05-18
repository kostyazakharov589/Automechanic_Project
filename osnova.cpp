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
#include <utility>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>

osnova::osnova(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::osnova)
{
    ui->setupUi(this);
    this->setWindowTitle("АРМ Автомеханика");
    this->setWindowIcon(QIcon(":/logooo.png"));
    /////////////////////////////////////////////////////////////////////////////
    if (dbManager.connectToDataBase()) {//здесь мы реализуем логику подключения к бд и вывод на второй странице
        qDebug() << "База данных успешно подключена";
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
        replacedPartsList.clear();//очистка всего старого
        generatedSeasons.clear();
        finalResults.clear();
        ui->listReplacements->clear();
        ui->makeInput->clear();
        ui->modelInput->clear();
        ui->yearInput->clear();
        ui->mileageInput->setValue(0);
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->profileSelector, &QComboBox::activated, this, [this](int) {
        QString selectedName = ui->profileSelector->currentText();
        loadProfile(selectedName); // подгружаем наши расчёты в память
        calculateFinalWear();
        ui->openGLWidget->update();
        ui->stackedWidget->setCurrentIndex(4); // а потом показываем результат
    });
    //
    connect(ui->btnDeleteProfile, &QPushButton::clicked, this, &osnova::deleteSelectedProfile);{
    };
    //2 страница
    connect(ui->btnBackToCar, &QPushButton::clicked, this, [this]() {
        //prepareSeasonsPage(); можно убрать
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
        double totalBaseMileage = ui->mileageInput->value(); // наше доёбле
        double totalEntered = 0.0;
        int emptyCount = 0;

        for (const auto &entry : std::as_const(generatedSeasons)) {//проверка по введённом + чтобы от утечки избавиться используем блок неявного копирования контейнера черещ as_const
            double val = entry.spinKm->value();
            if (val > 0.001) {
                totalEntered += val;
            } else {
                emptyCount++;
            }
        }

        if (totalEntered > totalBaseMileage + 0.01) {//если больше и через .arg
            QMessageBox::warning(this, "Ошибка пробега",
                                 QString("Введенный пробег по сезонам (%1 км) превышает общий пробег автомобиля (%2 км)!")
                                     .arg(totalEntered, 0, 'f', 2)
                                     .arg(totalBaseMileage, 0, 'f', 2));
            return;
        }

        if (emptyCount == 0 && totalEntered < totalBaseMileage - 0.01) {//если меньше и через тоже арг
            QMessageBox::warning(this, "Ошибка пробега",
                                 QString("Сумма по всем сезонам (%1 км) меньше базового пробега (%2 км)!\nОставьте часть сезонов с '0' для автораспределения.")
                                     .arg(totalEntered, 0, 'f', 2)
                                     .arg(totalBaseMileage, 0, 'f', 2));
            return;
        }

        ui->stackedWidget->setCurrentIndex(3);//некст
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
        ui->profileSelector->addItems(dbManager.getSavedProfiles(currentUser));//добавляем в список наших профиоей
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
        ui->profileSelector->addItems(dbManager.getSavedProfiles(currentUser));//добавляем в список наших профиоей

    });
    //

    //5 страница
    connect(ui->btnBackToHistory, &QPushButton::clicked, this, [this]() {// это назад
        ui->stackedWidget->setCurrentIndex(3);
        prepareReplacementsPage();
    });
    connect(ui->btnMaps, &QPushButton::clicked, this, &osnova::openMapsForService);//кнопка с картами
    connect(ui->btnPredict, &QPushButton::clicked, this, &osnova::showTOPrediction);{};//кнопка с прогнозом
    connect(ui->btnRestart, &QPushButton::clicked, this, [this]() {//кнопка в начало
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->openGLWidget, &MyGLWidget::zoneClicked, this, [this](const QString& zoneName) { //вот здесь отрисовка блоков
        if (finalResults.isEmpty()) {
            QMessageBox::warning(this, "Пусто", "Сначала выполните расчет износа!");
            return;
        }

        ui->listWidgetDetails->clear();
        ui->lblZoneName->setText("Зона осмотра: " + zoneName);//отображение названия зоны
        QStringList targetCategories; //здесь разделяем по категориям

        if (zoneName == "Двигатель") {//тут категории из изпартинзона
            targetCategories << "Двигатель и ГРМ" << "Охлаждение" << "Фильтры";
        } else if (zoneName == "Ходовая часть") {
            targetCategories << "Тормозная система" << "Подвеска и рулевое" << "Трансмиссия";
        } else if (zoneName == "Электрика") {
            targetCategories << "Электрика и прочее";
        } else if (zoneName == "Прочее") {
            targetCategories << "Прочее";
        }

        bool partsFound = false;

        for (const auto& res : std::as_const(finalResults)) { //отрисовка блоков
            if (isPartInZone(res.partName, targetCategories)) {
                partsFound = true;

                QListWidgetItem* item = new QListWidgetItem(ui->listWidgetDetails);
                item->setData(Qt::UserRole, res.partName); //прячем оригинальное имя для двойного клика

                QWidget *rowWidget = new QWidget();
                QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
                rowLayout->setContentsMargins(10, 5, 10, 5); //отступы

                QString icon; //иконочки
                if (res.wearPercent >= 80) icon = "🟥";
                else if (res.wearPercent >= 50) icon = "🟨";
                else icon = "🟩";

                QLabel *lblIcon = new QLabel(icon); //иконка
                lblIcon->setFont(QFont("Bold", 14));

                QVBoxLayout *textLayout = new QVBoxLayout(); //износ и прочее
                QLabel *lblName = new QLabel(res.partName);
                lblName->setStyleSheet("font-weight: bold; font-size: 13px; color: #FFFFFF;");

                textLayout->addWidget(lblName);
                textLayout->setSpacing(2);

                QLabel *lblHint = new QLabel("🔍");
                lblHint->setStyleSheet("color: #95a5a6; font-size: 10px;");

                rowLayout->addWidget(lblIcon);
                rowLayout->addLayout(textLayout);
                rowLayout->addStretch();
                rowLayout->addWidget(lblHint);

                item->setSizeHint(rowWidget->sizeHint());
                ui->listWidgetDetails->setItemWidget(item, rowWidget);
            }
        }

        if (!partsFound) {//проверка
            ui->listWidgetDetails->addItem("Нет деталей в зоне");
        }
    });
    connect(ui->listWidgetDetails, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {//вот здесь у нас заключение
        // достаём имя детали из скрытой памяти
        QString partName = item->data(Qt::UserRole).toString();

        int wear = 0;
        for (const auto &res : std::as_const(finalResults)) { // поиск деталей без копирования массива
            if (res.partName == partName) {
                wear = res.wearPercent;
                break;
            }
        }
        // формируем текст заключения
        QString status, color, recommendation;
        if (wear >= 80) {
            status = "Серьёзный износ!";
            color = "#e74c3c";
            recommendation = "Требуется срочная замена. Дальнейшая эксплуатация узла небезопасна и может привести к каскадной поломке смежных систем.";
        } else if (wear >= 50) {
            status = "Средний износ";
            color = "#e67e22";
            recommendation = "Ресурс детали подходит к концу. Рекомендуется запланировать замену при следующем плановом ТО.";
        } else {
            status = "Незначительный износ";
            color = "#27ae60";
            recommendation = "Деталь полностью исправна. Вмешательство не требуется.";
        }
        // отчётик в html
        QString htmlReport = QString(
                                 "<h3 style='color:#FFFFFF;'>Диагностическая карта узла</h3><hr>"
                                 "<p><b>Наименование:</b> %1</p>"
                                 "<p><b>Остаточный ресурс:</b> %2%</p>"
                                 "<p><b>Текущее состояние:</b> <span style='color:%3; font-weight:bold;'>%4</span></p>"
                                 "<p><b>Вердикт системы:</b> %5</p><hr>"
                                 "<p><i>Выберите площадку для поиска запчасти:</i></p>"
                                 ).arg(partName, QString::number(100 - wear), color, status, recommendation);

        // само окно
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(partName);
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(htmlReport);

        QPushButton *btnAvito = msgBox.addButton("Искать на Авито", QMessageBox::ActionRole);
        QPushButton *btnDrom = msgBox.addButton("Искать на Drom.ru", QMessageBox::ActionRole);
        msgBox.addButton("Закрыть", QMessageBox::RejectRole);

        msgBox.exec();//фигня, но без неё не работает

        //обработка и поиск
        QString make = ui->makeInput->text();
        QString model = ui->modelInput->text();
        QString searchQuery = make + " " + model + " " + partName;

        if (msgBox.clickedButton() == btnAvito) {
            QDesktopServices::openUrl(QUrl("https://www.avito.ru/all/zapchasti_i_aksessuary?q=" + QUrl::toPercentEncoding(searchQuery)));
        } else if (msgBox.clickedButton() == btnDrom) {
            QDesktopServices::openUrl(QUrl("https://baza.drom.ru/sell_spare_parts/?query=" + QUrl::toPercentEncoding(searchQuery)));
        }
    });
    connect(ui->btnExportReport, &QPushButton::clicked, this, &osnova::exportReport);{//репорт
    };
///////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////

}

osnova::~osnova()
{
    delete ui;
}
bool osnova::isPartInZone(const QString& partName, const QStringList& categories) {//делим наши детальки на зоны
    static QMap<QString, QStringList> catalogMap = {
        {"Фильтры", {"Масляный фильтр", "Воздушный фильтр", "Салонный фильтр", "Топливный фильтр"}},//первое у нас название, а второе что в неё входит
        {"Тормозная система", {"Колодки передние", "Колодки задние", "Диски тормозные передние", "Суппорт передний"}},
        {"Подвеска и рулевое", {"Амортизатор передний", "Пружина передняя", "Стойка стабилизатора", "Рулевой наконечник"}},
        {"Двигатель и ГРМ", {"Ремень ГРМ", "Водяная помпа", "Свеча зажигания", "Ролик натяжителя"}},
        {"Охлаждение", {"Радиатор охлаждения", "Антифриз", "Термостат", "Патрубок радиатора"}},
        {"Трансмиссия", {"Сцепление", "Масло трансмиссионное", "ШРУС наружный"}},
        {"Электрика и прочее", {"Аккумулятор", "Генератор", "Стартер", "Лампа ближнего света"}}
    };
    if (categories.contains("Прочее")) {//здесь все остальные детали
        // пробегаемся по всем известным спискам деталей
        for (const QStringList& knownParts : catalogMap.values()) {
            if (knownParts.contains(partName)) {
                return false; // если есть в известных, то не прочее
            }
        }
        return true; // тут понятно
    }

    for (const QString& cat : categories) {//проверка
        if (catalogMap[cat].contains(partName)) return true;//соответствует - да?
    }
    return false;
}
void osnova::saveProfileToJson(const QString &make, const QString &model, int year, int mileage)//реализация логики функции сохранения профиля в жесон
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
    for (const auto &rp : std::as_const(replacedPartsList)) {
        QJsonObject repObj;
        repObj["partName"] = rp.partName;
        repObj["year"] = rp.year;
        repObj["season"] = rp.season;
        replacementsArray.append(repObj);
    }
    obj["replacements"] = replacementsArray;
    QJsonArray timelineArray; // Создаем массив для истории

    for (const auto &entry : std::as_const(generatedSeasons)) {//т.к. у нас пробег по сезонам генерится от пользователя, то перебираем по колву введённого
        QJsonObject seasonObj;
        seasonObj["year"] = entry.year;
        seasonObj["season"] = entry.seasonName;
        int val = entry.spinKm->value();
        int finalKm = val;
        if (val == 0 && entry.cmbRoad->currentText() != "Гараж") {
            finalKm = (int)avgKm;//среднее арифм.
        }
        seasonObj["km"] = finalKm;
        seasonObj["road_k"] = entry.cmbRoad->currentText(); //собираем сезоны

        timelineArray.append(seasonObj); // Добавляем сезон в историю
    }

    obj["timeline"] = timelineArray; //прикрепляем историю к профилю

    QJsonDocument doc(obj);
    QDir().mkpath("profiles");//используется для создания всей цепочки вложенных директорий по указанному пути

    QString fileName = QString("%1-%2(%3).json").arg(make, model, QString::number(year));//сохраняем
    QString filePath = "profiles/" + currentUser + "/" + fileName;//профиля машин
    QFile file(filePath);

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
void osnova::generateTimeline() //генерируем сезоны исходя из введенных дат владения
{
    QLayoutItem *child;
    while ((child = ui->verticalLayout_2->takeAt(0)) != nullptr) { //для очищения виджетов, хз
        if (child->widget())
            delete child->widget(); //удаляем сам виджет из памяти
        delete child; //удаляем элемент слоя
    }
    generatedSeasons.clear(); //очищаем от предыдущей генерации

    // Берем Месяцы и Года из интерфейса
    int startMonth = ui->cmbStartMonth->currentIndex() + 1;
    int endMonth   = ui->cmbEndMonth->currentIndex() + 1;
    int startYear  = ui->spinStartYear->currentText().toInt();
    int endYear    = ui->spinEndYear->currentText().toInt();

    if (endYear < startYear || (endYear == startYear && endMonth < startMonth)) {
        qDebug() << "Ошибка: Период конца указан раньше периода начала!"; //защита на всякий случай
        return;
    }

    QStringList roadTypes = {"Город", "Трасса", "Грунтовые", "Бездорожье", "Гараж"}; //типы дорог

    for (int y = startYear; y <= endYear; ++y) { //генерация блоков по годам. начинаем со старта и до конца

        QStringList activeSeasons = getActiveSeasonsForYear(y, startYear, startMonth, endYear, endMonth); //получаем список из введённых данных
        if (activeSeasons.isEmpty()) continue;

        QGroupBox *yearBox = new QGroupBox("                                                            " + QString::number(y) + " год"); //создаём виджет из года и сезона

        QVBoxLayout *yearLayout = new QVBoxLayout(); //создаем вертикальный слой
        yearBox->setLayout(yearLayout); //назначаем слой боксу

        for (const auto &seasonName : std::as_const(activeSeasons)) { //блокируем копирование контейнера
            QWidget *rowWidget = new QWidget(yearBox); //создаём виджеты для отображения годов
            QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(5, 5, 5, 5); //задаём отступы

            QLabel *lblSeason = new QLabel(seasonName, rowWidget); //выгрузка сезонов
            lblSeason->setMinimumWidth(60); //минимальная ширина

            QDoubleSpinBox* spinKm = new QDoubleSpinBox(); //выгрузка км
            spinKm->setRange(0, 1000000); //задаем диапазон
            spinKm->setMaximum(1000000.00); //максимальный
            spinKm->setDecimals(2); //d

            QComboBox *cmbRoad = new QComboBox(rowWidget); //создаём комбобокс для ввода дорог
            cmbRoad->addItems(roadTypes);

            rowLayout->addWidget(lblSeason);
            rowLayout->addWidget(spinKm);
            rowLayout->addWidget(cmbRoad);

            yearLayout->addWidget(rowWidget); //добавляем виджеты по дорогам и прочим для ввода

            SeasonEntry entry;
            entry.year = y;
            entry.seasonName = seasonName;
            entry.spinKm = spinKm;
            entry.cmbRoad = cmbRoad;
            generatedSeasons.append(entry); // Сохраняем для JSON
        }

        ui->verticalLayout_2->addWidget(yearBox); //боксы годов
    }

    ui->verticalLayout_2->addStretch(); //занимает всё пространтсва, для норм отображения можно и менять
}

void osnova::loadProfile(const QString &profileName) {//загружаем профили машин
    QFile file("profiles/" + currentUser + "/" + profileName + ".json");//берём название профиля из сохранённого типа названий
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
    int totalBaseMileage = ui->mileageInput->value();//это с первой страничке, не путать!
    double totalUserEntered = 0;//всё введённое юзером
    int emptySeasonsCount = 0;//пустые сезоны
    for (const auto &entry : std::as_const(generatedSeasons)) { // подсчитываем то, что пользователь уже ввёл
        int val = entry.spinKm->value();
        if (val > 0) {
            totalUserEntered += val;
        }
        else if (entry.cmbRoad->currentText() != "Гараж") {//если гараж, то пустое
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
    for (const auto &rp : std::as_const(replacedPartsList)) {//делаем такие контейнеры через ас_конст, т.к. в других просто утечка памяти происходит
        ui->listReplacements->addItem(QString("%1 — замена: %2 %3 г.")
                                          .arg(rp.partName, rp.season, QString::number(rp.year)));//для вывода
    }
}

void osnova::calculateFinalWear() { //отображение износа машин
    if (!ui->resultsLayout->layout()) { //защита от вылетов, если слоя нет, то он создаётся сам
        ui->resultsLayout->setLayout(new QVBoxLayout());
    }

    QLayoutItem *child;
    if (ui->resultsLayout->layout()) {
        while ((child = ui->resultsLayout->layout()->takeAt(0)) != nullptr) { //очищаем старые полоски
            if (child->widget()) delete child->widget();
            delete child;
        }
    }

    QList<CalcSeasonData> history; //вот тут добавление сезонов, а далее замен
    for (const auto &entry : std::as_const(generatedSeasons)) { //блокируем копирование контейнера
        history.append({entry.year, entry.seasonName, entry.spinKm->value(), entry.cmbRoad->currentText()});
    }

    QList<CalcReplaceData> replacements;
    for (const auto &rp : std::as_const(replacedPartsList)) { //блокируем копирование контейнера
        replacements.append({rp.partName, rp.year, rp.season});
    }

    WearCalculator calculator; //вызов калькулятора

    QMap<QString, int> catalog = dbManager.getCarPartsResource(ui->makeInput->text(), ui->modelInput->text()); //подгружаем детали из бд
    calculator.setCatalog(catalog); //наш каталог

    finalResults = calculator.calculate(ui->mileageInput->value(), history, replacements); //сохранём в опенгл

    for (const auto& res : std::as_const(finalResults)) { //отрисовываем прогресс износа (без лишних копий)
        QWidget *row = new QWidget();
        QHBoxLayout *hLay = new QHBoxLayout(row);
        hLay->setContentsMargins(5, 5, 5, 5); //отступы

        QLabel *lbl = new QLabel(res.partName);
        lbl->setFixedWidth(200);

        QFont font = lbl->font(); //шрифт
        font.setPointSize(10);
        font.setBold(true);
        lbl->setFont(font);

        QProgressBar *pb = new QProgressBar();
        pb->setValue(res.wearPercent);
        pb->setFixedHeight(22); //высота в 22

        QString colorStart, colorEnd;//хотбар

        if (res.wearPercent >= 80) {
            colorStart = "#ff6b6b"; colorEnd = "#ee5253"; //красный
        } else if (res.wearPercent >= 50) {
            colorStart = "#feca57"; colorEnd = "#ff9f43"; //жёлтый
        } else {
            colorStart = "#1dd1a1"; colorEnd = "#10ac84"; //грин
        }

        int radius = (res.wearPercent > 0 && res.wearPercent < 100) ? 4 : 6;//для фикса квадратов в полоске

        QString chunkStyle = QString( //делаем через градиенты с чанками, хз как работает, но прикольно выглядит
                                 "QProgressBar::chunk {"
                                 "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 %1, stop:1 %2);"
                                 "    border-radius: %3px;"
                                 "    margin: 2px;" //ещё один фикс квадратов
                                 "}"
                                 ).arg(colorStart, colorEnd, QString::number(radius));

        QString baseStyle = //тут базовый стиль для прогресс баров
            "QProgressBar {"
            "    border: 1px solid #256a99;"
            "    border-radius: 5px;"
            "    background-color: #rgb(63, 73, 85);"
            "    text-align: center;"
            "    color: #ffffff;"
            "    font-weight: bold;"
            "}";

        pb->setStyleSheet(baseStyle + chunkStyle); //применяем объединенный стиль
        hLay->addWidget(lbl);
        hLay->addWidget(pb);
        ui->resultsLayout->layout()->addWidget(row);
    }
    static_cast<QVBoxLayout*>(ui->resultsLayout->layout())->addStretch(); //для бокса лейлаутов
}
void osnova::exportReport() {//формируем отчёт по нашей программе
    if (finalResults.isEmpty()) {
        QMessageBox::warning(this, "Экспорт", "Нет данных для экспорта. Сначала выполните расчет.");//стандартная проверка
        return;
    }
    QString defaultName = QString("Отчет_%1_%2_%3.txt")//имя файла в системе
                              .arg(ui->makeInput->text())
                              .arg(ui->modelInput->text())
                              .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm"));//дата

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчет", defaultName, "Текстовые файлы (*.txt)");//вызываем окно с проводником

    if (fileName.isEmpty()) return;//если пустое

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {//на случай неудачи
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл.");
        return;
    }

    QTextStream out(&file);//сам отчёт в нашем текстовике
    out << "       ДИАГНОСТИЧЕСКАЯ КАРТА АВТОМОБИЛЯ             \n";
    out << "Дата осмотра: " << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") << "\n";
    out << "Автомобиль:   " << ui->makeInput->text() << " " << ui->modelInput->text() << "\n";
    out << "Год выпуска:  " << ui->yearInput->currentText() << "\n";
    out << "Общий пробег: " << ui->mileageInput->value() << " км\n\n";

    out << "ДЕТАЛИЗАЦИЯ ПО СЕЗОНАМ (История владения):\n"; // заголовок для сезонов
    out << QString("%1 | %2 | %3 | %4\n")
               .arg("Год", 5)
               .arg("Сезон", 7)
               .arg("Пробег (км)", 12)
               .arg("Тип дороги");

    for (const auto &entry : std::as_const(generatedSeasons)) {//добавляем сезоны ещё
        out << QString("%1 | %2 | %3 | %4\n")
                   .arg(entry.year, 5)
                   .arg(entry.seasonName, 7)
                   .arg(entry.spinKm->value(), 12, 'f', 1) // выводим км с одним знаком после запятой
                   .arg(entry.cmbRoad->currentText());
    }

    QString make = ui->makeInput->text(); //марка для ссылок
    QString model = ui->modelInput->text(); //модель для ссылок

    QStringList critical;//категории поломок запчастей
    QStringList warning;
    QStringList normal;

    for (const auto& res : std::as_const(finalResults)) {//здесь генерация строк с сылками
        // Кодируем запрос для браузера марка + модель + деталь
        QString query = QUrl::toPercentEncoding(make + " " + model + " " + res.partName);
        QString avito = "https://www.avito.ru/all/zapchasti_i_aksessuary?q=" + query;
        QString drom = "https://baza.drom.ru/sell_spare_parts/?query=" + query;

        if (res.wearPercent >= 80) {
            // добавляем название, процент и сразу ссылки под деталью
            critical << QString("- %1: %2%\n  АВИТО: %3\n  DROM: %4\n")
                            .arg(res.partName, -25).arg(res.wearPercent).arg(avito).arg(drom);
        } else if (res.wearPercent >= 50) {
            warning << QString("- %1: %2%\n  Купить: %3\n  На Drom: %4\n")
                           .arg(res.partName, -25).arg(res.wearPercent).arg(avito).arg(drom);
        } else {
            normal << QString("- %1: %2%").arg(res.partName, -25).arg(res.wearPercent);
        }
    }

    out << " \n";
    double totalKmReport = 0;
    for (const auto &entry : std::as_const(generatedSeasons)) totalKmReport += entry.spinKm->value();
    double dailyKmReport = totalKmReport / (generatedSeasons.size() * 90.0);

    WearCalculator calc;
    calc.setCatalog(dbManager.getCarPartsResource(ui->makeInput->text(), ui->modelInput->text()));
    QList<PredictionData> predictions = calc.predictService(finalResults, dailyKmReport);

    if (!predictions.isEmpty()) {//если пустое
        out << "\nПРОГНОЗ ДАТ СЛЕДУЮЩЕГО ОБСЛУЖИВАНИЯ:\n";
        QLocale ru(QLocale::Russian);//для дат
        for (const auto& p : std::as_const(predictions)) {//вот тут сам прогноз
            QDate sDate = QDate::currentDate().addDays(p.daysLeft);
            out << QString("- %1: замена через %2 км (ориентировочно %3)\n")
                       .arg(p.partName)
                       .arg(static_cast<int>(p.remainingKm))
                       .arg(ru.toString(sDate, "dd.MM.yyyy"));
        }
    }
    out << " \n";

    //сами тексты отчета
    out << "КРИТИЧЕСКИЙ ИЗНОС (замена):\n";
    if (critical.isEmpty()) out << "Не выявлено\n";
    else for (const QString& s : std::as_const(critical)) out << s << "\n";

    out << "СРЕДНИЙ ИЗНОС (Бригада автомехаников, обратите внимание):\n";
    if (warning.isEmpty()) out << "Не выявлено\n";
    else for (const QString& s : std::as_const(warning)) out << s << "\n";

    out << "В НОРМЕ:\n";
    if (normal.isEmpty()) out << "Не выявлено\n";
    else for (const QString& s : std::as_const(normal)) out << s << "\n";

    file.close(); // закрываем файл
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:5000/upload"); // адрес локального сервера
    QNetworkRequest request(url);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart userPart; // кто отправил
    userPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"mechanic_name\""));
    userPart.setBody(currentUser.toUtf8());

    QHttpPart docPart; // сам файл
    QFileInfo fi(fileName); // берем чистое имя файла без полного пути
    docPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"document\"; filename=\"" + fi.fileName() + "\""));
    QFile *uploadFile = new QFile(fileName);
    uploadFile->open(QIODevice::ReadOnly);
    docPart.setBodyDevice(uploadFile);
    uploadFile->setParent(multiPart);
    multiPart->append(userPart);
    multiPart->append(docPart);

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "👌", "Отчет отправлен на сервер");
        } else {
            QMessageBox::warning(this, "📡", "Отчет сохранен, но сервер недоступен.");
        }
        reply->deleteLater();
    });
}
void osnova::deleteSelectedProfile() {//удаление профиля через список выбора
    QStringList profiles = dbManager.getSavedProfiles(currentUser);//получаем все наши json-файлы из папки

    if (profiles.isEmpty()) {
        QMessageBox::information(this, "Удаление", "Список профилей пуст, удалять нечего.");//защита на случай пустой папки
        return;
    }
    bool ok; //для кнопки удаления
    QString profileToDelete = QInputDialog::getItem(this,
                                                    "Менеджер профилей",
                                                    "Выберите профиль для удаления из системы:",
                                                    profiles, 0, false, &ok);

    if (ok && !profileToDelete.isEmpty()) {//если пользователь нажал ок и выбрал строку
        QMessageBox::StandardButton reply;//подтверждение удаления
        reply = QMessageBox::question(this, "Подтверждение удаления",
                                      QString("Вы действительно хотите навсегда стереть профиль автомобиля: %1?").arg(profileToDelete),
                                      QMessageBox::Yes | QMessageBox::No);//да и нет
        if (reply == QMessageBox::Yes) {
            QString path = "profiles/" + profileToDelete + ".json";//путь к файлу
            if (QFile::remove(path)) {//команда на физическое удаление файла с диска
                ui->profileSelector->clear();//обновляем список профилей на 1 страничке
                ui->profileSelector->addItems(dbManager.getSavedProfiles(currentUser));
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось удалить файл");//на случай ошибок
            }
        }
    }
}
void osnova::showTOPrediction() {//тут расчёт прогноза
    if (finalResults.isEmpty()) return;//если пустое
    double totalKm = 0;
    for (const auto &entry : std::as_const(generatedSeasons)) totalKm += entry.spinKm->value();//тут добавляем к сезонам
    double dailyKm = totalKm / (generatedSeasons.size() * 90.0);//тут к среднему добавляем
    WearCalculator calc;
    calc.setCatalog(dbManager.getCarPartsResource(ui->makeInput->text(), ui->modelInput->text()));//тут просто наши модель и марка
    QList<PredictionData> predictions = calc.predictService(finalResults, dailyKm);//здесь наш лист
    if (predictions.isEmpty()) {//если ничего нет больше 69 процентов
        QMessageBox::information(this, "Прогноз", "Критического износа не обнаружено.");
        return;
    }
    QString message = "<h3 style='color:#FFFFFF;'>Прогноз ТО</h3><hr>";//выводим окно на экран
    QLocale russian(QLocale::Russian);//без этого месяц почему-то показывается на забугорном
    for (const auto& p : std::as_const(predictions)) {//само окно
        QDate sDate = QDate::currentDate().addDays(p.daysLeft);
        message += QString(
                       "<p>Деталь: <b>%1 (%2%)</b><br>"
                       "Ресурс: ~<b>%3 км</b> | Дата: <b style='color:#27ae60;'>%4</b></p>"
                       ).arg(p.partName)
                       .arg(p.wearPercent)
                       .arg(static_cast<int>(p.remainingKm))
                       .arg(russian.toString(sDate, "dd MMMM yyyy 'г.'"));
    }

    QMessageBox::about(this, "Прогноз системы", message);//и сам прогноз системы
}
void osnova::openMapsForService() {//здесь функция открытия карт
    QString make = ui->makeInput->text(); //берём марку
    QString model = ui->modelInput->text(); //берём модель
    bool ok;    //вызываем быстрое окно ввода города (без сохранения в файл)
    QString city = QInputDialog::getText(this, "Поиск автосервиса",
                                         "Введите н.п. для поиска",
                                         QLineEdit::Normal, "", &ok);
    if (!ok) return; //если нажали отмену — выходим
    QString query = "автосервис " + make + " "; //запрос в гугл карты
    if (!city.isEmpty()) {
        query += " " + city; //добавляем город, если пользователь его вписал
    }
    QDesktopServices::openUrl(QUrl("https://yandex.ru/maps/?text=" + QUrl::toPercentEncoding(query)));    //открываем яндекс карты с готовым запросом в браузере
}

void osnova::setCurrentUser(const QString &username) { //установка текущего юзера
    currentUser = username;
    QDir().mkpath("profiles/" + currentUser); // создаем папку для него
    ui->profileSelector->clear();
    ui->profileSelector->addItems(dbManager.getSavedProfiles(currentUser)); // грузим его авто
}