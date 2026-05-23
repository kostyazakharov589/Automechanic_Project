#include "wearcalculator.h"
#include <QDate>
WearCalculator::WearCalculator() {
    roadCoefs["Город"] = 1.0;
    roadCoefs["Трасса"] = 0.8;
    roadCoefs["Грунтовые"] = 1.2;
    roadCoefs["Бездорожье"] = 1.6;
    roadCoefs["Гараж"] = 0.0; //дорожные коэфициенты

    seasonCoefs["Зима"] = 1.2;  // Холодный пуск, реагенты, пробуксовки
    seasonCoefs["Весна"] = 1.1; // Перепады температур
    seasonCoefs["Осень"] = 1.1; // Повышенная влажность
    seasonCoefs["Лето"] = 1.0;
}

void WearCalculator::setCatalog(const QMap<QString, int>& catalog) {
    partCatalog = catalog;//копируем каталог
}

double WearCalculator::calculateDistributedKm(int baseMileage, const QList<CalcSeasonData>& history) {//функция для подсчёта км если больше 0 и не гараж
    double userEntered = 0.0;//введённое юзером
    int emptyActive = 0;//пустое

    for (const auto& entry : history) {//перебираем
        if (entry.km > 0.0) {//если больше 0, то скалдываем с введённым юзером
            userEntered += entry.km;
        }
        else if (entry.roadType != "Гараж") { //для гаража
            emptyActive++;
        }
    }

    if (emptyActive > 0 && baseMileage > userEntered) {
        return std::max(0.0, (baseMileage - userEntered) / emptyActive);//для защиты от отрицательных исел при вводе пробега
    }
    return 0;
}

QList<PartResult> WearCalculator::calculate(int baseMileage, const QList<CalcSeasonData>& history, const QList<CalcReplaceData>& replacements) {//калькулятор иноса!!! САМОЕ ГЛАВНОЕ
    QList<PartResult> results;//запчасти

    int currentYear = 0;
    for (const auto& entry : history) {//чтобы нормально подставился год
        if (entry.year > currentYear) {
            currentYear = entry.year;
        }
    }
    if (currentYear == 0) currentYear = QDate::currentDate().year();//если год в сезонах не введён, то берём со второй страницы
    if (partCatalog.isEmpty()) return results;

    double distributedKm = calculateDistributedKm(baseMileage, history);

    for (auto it = partCatalog.begin(); it != partCatalog.end(); ++it) {//цикл проверки замен деталей
        QString partName = it.key();
        int maxResource = it.value();

        int startYear = history.isEmpty() ? currentYear : history.first().year;//если деталь родная, то её год - год производства авто
        QString startSeason = history.isEmpty() ? "Лето" : history.first().season;//берём лето за стартовый сезон, если сезоны не введены( как правило именно летом берут машины)
        bool replaced = false;

        for (const auto& rp : replacements) {//перебираем замены
            if (rp.partName == partName) {
                startYear = rp.year;
                startSeason = rp.season;
                replaced = true;
            }
        }

        double totalEffectiveKm = 0;
        bool countStarted = !replaced;

        for (const auto& entry : history) {//вот тут коэфы на случай гаража и невведённого
            if (!countStarted) {
                if (entry.year == startYear && entry.season == startSeason) {
                    countStarted = true;
                } else{
                continue;
                }
            }

            double km = entry.km;
            if (km == 0 && entry.roadType != "Гараж") {
                km = distributedKm;
            }

            double rCoef = roadCoefs.value(entry.roadType, 1.0);
            double sCoef = seasonCoefs.value(entry.season, 1.0);
            totalEffectiveKm += (km * rCoef * sCoef);//км+дорога+сезоны
        }

        int wearPercent = 0;
        if (maxResource > 0) {
            wearPercent = (totalEffectiveKm / maxResource) * 100;//считаем процент беря наши км и из базы заводской износ, делим их и умножаем на сотку
        }

        int age = currentYear - startYear;//хронологическое старение
        if (age >= 5 && (partName.contains("Ремень") || partName.contains("Шина") ||
                         partName.contains("Сайлентблок") || partName.contains("Патрубок") ||
                         partName.contains("Масло") || partName.contains("Антифриз"))) {
            wearPercent += (age - 4) * 15; // накидываем 15 процентов за 4 года, +- максимально усредненнёное значение
        }

        results.append({partName, wearPercent});
    }
    /////////////////каскадный износ деталей
    int suspensionWear = 0;
    int suspensionPartsCount = 0;

    for (const auto& res : std::as_const(results)) { // вычисление убитости подвестки
        if (res.partName.contains("Амортизатор") || res.partName.contains("Рычаг") || res.partName.contains("Стойка")) {
            suspensionWear += res.wearPercent;//добавляем к проценту
            suspensionPartsCount++;
        }
    }

    if (suspensionPartsCount > 0) {
        suspensionWear /= suspensionPartsCount;
    }

    for (auto& res : results) {//принимаем штрафы к зависящим деталям
        if (suspensionWear >= 75) {
            if (res.partName.contains("Шина") || res.partName.contains("Подшипник") ||
                res.partName.contains("Рулевая") || res.partName.contains("Колодки")) {
                res.wearPercent = (int)(res.wearPercent * 1.2); // +20 процентов к скорости износа
            }
        }

        if (res.wearPercent > 100) res.wearPercent = 100;//тпроврка если перевалить за 100
    }

    return results;//наш результатик
}
QList<PredictionData> WearCalculator::predictService(const QList<PartResult>& results, double dailyKm) {//тут калькулятор прогноза
    QList<PredictionData> predictions;
    if (dailyKm <= 0) dailyKm = 40.0;//тут если вообще ничё на третьей странице не ввели 

    QList<PartResult> sorted = results;//от большего к меньшему
    std::sort(sorted.begin(), sorted.end(), [](const PartResult& a, const PartResult& b) {
        return a.wearPercent > b.wearPercent;
    });

    for (const auto& res : std::as_const(sorted)) {//будем брать от 59 до 99 процентов
        if (res.wearPercent >= 60 && res.wearPercent < 100) {
            int totalResource = partCatalog.value(res.partName, 0);
            if (totalResource > 0) {
                PredictionData p;
                p.partName = res.partName;
                p.wearPercent = res.wearPercent;
                p.remainingKm = totalResource * (1.0 - (p.wearPercent / 100.0));
                p.daysLeft = static_cast<int>(p.remainingKm / dailyKm);
                predictions.append(p);
            }
        }
    }
    return predictions;
}
