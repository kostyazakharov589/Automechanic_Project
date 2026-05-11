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

double WearCalculator::calculateDistributedKm(int baseMileage, const QList<CalcSeasonData>& history) {//функция для подсчёта км
    double userEntered = 0;//введённые
    int emptyActive = 0;//пустые

    for (const auto& entry : history) {
        if (entry.km > 0) {
            userEntered += entry.km;//записываем введённые
        } else if (!entry.roadType.contains("0.0")) {
            emptyActive++; // Считаем пустые сезоны игнорируя гараж
        }
    }

    if (emptyActive > 0 && baseMileage > userEntered) {
        return (baseMileage - userEntered) / emptyActive;//возвращаем среднее
    }
    return 0;
}

QList<PartResult> WearCalculator::calculate(int baseMileage, const QList<CalcSeasonData>& history, const QList<CalcReplaceData>& replacements) {//калькулятор иноса!!!
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

    for (auto it = partCatalog.begin(); it != partCatalog.end(); ++it) {//самое первое - базовое старение за годы работы
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
                }
                continue;
            }

            double km = entry.km;
            if (km == 0 && !entry.roadType.contains("Гараж")) {
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

        int age = currentYear - startYear;//а вот тут второе - хронологическое старение
        if (age >= 5 && (partName.contains("Ремень") || partName.contains("Шина") ||
                         partName.contains("Сайлентблок") || partName.contains("Патрубок") ||
                         partName.contains("Масло") || partName.contains("Антифриз"))) {
            wearPercent += (age - 4) * 15; // накидываем 15 процентов за 4 года, +- максимально усредненнёное значение, но пойдёт
        }

        results.append({partName, wearPercent});
    }
    /////////////////каскадный износ деталей
    int suspensionWear = 0;
    int suspensionPartsCount = 0;

    // Вычисляем среднюю "убитость" подвески
    for (const auto& res : results) {
        if (res.partName.contains("Амортизатор") || res.partName.contains("Рычаг") || res.partName.contains("Стойка")) {
            suspensionWear += res.wearPercent;
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

        /////ФИНАЛ - все введённые до этого коэфы
        if (res.wearPercent > 100) res.wearPercent = 100;
    }

    return results;//наш результатик
}