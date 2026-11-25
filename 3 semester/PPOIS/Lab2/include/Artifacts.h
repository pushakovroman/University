/**
 * @file Artifacts.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы музейных экспонатов, авторов, исторических эпох и габаритов.
 * @details Содержит 8 классов: Dimensions, HistoricalEra, ArtistProfile, Artifact,
 * Painting, Sculpture, AncientManuscript, ArcheologicalFind.
 */

#ifndef MUSEUM_ARTIFACTS_H
#define MUSEUM_ARTIFACTS_H

#include "MuseumExceptions.h"
#include <string>

/**
 * @class Dimensions
 * @brief Физические габариты и масса музейного экспоната или витрины.
 * @details Хранит ширину, высоту, глубину (в сантиметрах) и массу (в килограммах). При создании проверяет строгую положительность всех четырёх параметров.
 * @see Artifact, DisplayCase, TransportCrate
 */
class Dimensions {
public:
    /**
     * @brief Конструктор класса Dimensions.
     * @param widthCm Ширина предмета в сантиметрах (> 0).
     * @param heightCm Высота предмета в сантиметрах (> 0).
     * @param depthCm Глубина предмета в сантиметрах (> 0).
     * @param weightKg Масса предмета в килограммах (> 0).
     * @throw InvalidArtifactDataException При передаче некорректных параметров инициализации.
     */
    Dimensions(double widthCm = 10.0, double heightCm = 10.0, double depthCm = 10.0, double weightKg = 1.0);

    /**
     * @brief Вычисляет объём в м³ по формуле widthCm * heightCm * depthCm * 1e-6.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateVolumeCubicMeters() const;
    /**
     * @brief Вычисляет площадь основания в м²: (widthCm * 0.01) * (depthCm * 0.01).
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateFootprintAreaSqMeters() const;
    /**
     * @brief Проверяет, не превосходят ли габариты и масса заданные пределы maxBounds по всем 4 измерениям.
     * @param maxBounds Максимально допустимые габариты и масса контейнера/витрины.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool fitsWithin(const Dimensions& maxBounds) const;

    /// @brief Возвращает ширину в сантиметрах.
    double widthCm() const noexcept { return widthCm_; }
    /// @brief Возвращает высоту в сантиметрах.
    double heightCm() const noexcept { return heightCm_; }
    /// @brief Возвращает глубину в сантиметрах.
    double depthCm() const noexcept { return depthCm_; }
    /// @brief Возвращает массу в килограммах.
    double weightKg() const noexcept { return weightKg_; }

private:
    double widthCm_; ///< Ширина предмета в сантиметрах (> 0).
    double heightCm_; ///< Высота предмета в сантиметрах (> 0).
    double depthCm_; ///< Глубина предмета в сантиметрах (> 0).
    double weightKg_; ///< Масса предмета в килограммах (> 0).
};

/**
 * @class HistoricalEra
 * @brief Историческая эпоха происхождения музейного предмета.
 * @details Определяет хронологические границы [startYear_, endYear_] и географический регион культурно-исторической эпохи.
 * @see Artifact, ArtistProfile
 */
class HistoricalEra {
public:
    /**
     * @brief Конструктор класса HistoricalEra.
     * @param name Название исторической эпохи (непустая строка).
     * @param startYear Год начала эпохи (отрицательные значения — до н.э.).
     * @param endYear Год окончания эпохи (>= startYear).
     * @param region Географический регион распространения эпохи.
     * @throw InvalidArtifactDataException При передаче некорректных параметров инициализации.
     */
    HistoricalEra(const std::string& name = "Antiquity",
                  int startYear = -500,
                  int endYear = 500,
                  const std::string& region = "Mediterranean");

    /**
     * @brief Возвращает true, если год попадает в замкнутый интервал [startYear_, endYear_].
     * @param year Проверяемый календарный год.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool containsYear(int year) const;
    /**
     * @brief Вычисляет хронологическую длительность эпохи: endYear_ - startYear_.
     * @return Значение типа @c int, вычисленное методом.
     */
    int durationYears() const;
    /**
     * @brief Определяет принадлежность к античности/древности (endYear_ <= 500 г. н.э.).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isAncient() const;

    /// @brief Возвращает название эпохи.
    const std::string& name() const noexcept { return name_; }
    /// @brief Возвращает год начала эпохи.
    int startYear() const noexcept { return startYear_; }
    /// @brief Возвращает год окончания эпохи.
    int endYear() const noexcept { return endYear_; }
    /// @brief Возвращает географический регион эпохи.
    const std::string& region() const noexcept { return region_; }

private:
    std::string name_; ///< Название исторической эпохи (непустая строка).
    int startYear_; ///< Год начала эпохи (отрицательные значения — до н.э.).
    int endYear_; ///< Год окончания эпохи (>= startYear).
    std::string region_; ///< Географический регион распространения эпохи.
};

/**
 * @class ArtistProfile
 * @brief Профиль автора (художника, скульптора, мастера).
 * @details Хранит биографические данные автора и направление искусства; позволяет проверять годы творческой активности и соответствие эпохе.
 * @see HistoricalEra, Painting, Sculpture
 */
class ArtistProfile {
public:
    /**
     * @brief Конструктор класса ArtistProfile.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param nationality Национальность / культурная принадлежность мастера.
     * @param birthYear Год рождения мастера.
     * @param deathYear Год смерти мастера (>= birthYear).
     * @param artMovement Художественное направление или школа.
     * @throw InvalidArtifactDataException При передаче некорректных параметров инициализации.
     */
    ArtistProfile(const std::string& fullName = "Unknown Master",
                  const std::string& nationality = "Unknown",
                  int birthYear = 1450,
                  int deathYear = 1520,
                  const std::string& artMovement = "Renaissance");

    /**
     * @brief Проверяет творческую активность автора в указанном году: [birthYear_ + 14, deathYear_].
     * @param year Проверяемый календарный год.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool wasActiveInYear(int year) const;
    /**
     * @brief Возвращает продолжительность жизни автора: deathYear_ - birthYear_.
     * @return Значение типа @c int, вычисленное методом.
     */
    int lifespanYears() const;
    /**
     * @brief Проверяет хронологическое пересечение годов жизни мастера с границами эпохи era.
     * @param era Историческая эпоха для сопоставления или атрибуции.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool matchesEra(const HistoricalEra& era) const;

    /// @brief Возвращает полное имя.
    const std::string& fullName() const noexcept { return fullName_; }
    /// @brief Возвращает национальность мастера.
    const std::string& nationality() const noexcept { return nationality_; }
    /// @brief Возвращает художественное направление.
    const std::string& artMovement() const noexcept { return artMovement_; }

private:
    std::string fullName_; ///< Полное имя автора, сотрудника или посетителя.
    std::string nationality_; ///< Национальность / культурная принадлежность мастера.
    int birthYear_; ///< Год рождения мастера.
    int deathYear_; ///< Год смерти мастера (>= birthYear).
    std::string artMovement_; ///< Художественное направление или школа.
};

/**
 * @class Artifact
 * @brief Базовый класс музейного экспоната.
 * @details Базовый полиморфный класс коллекции. Содержит инвентарный номер, оценочную стоимость, процент сохранности, габариты и историческую эпоху.
 * @see Dimensions, HistoricalEra, Painting, Sculpture, AncientManuscript, ArcheologicalFind
 */
class Artifact {
public:
    /**
     * @brief Конструктор класса Artifact.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @param title Наименование экспоната или выставки.
     * @param creationYear Год создания музейного предмета.
     * @param estimatedValue Оценочная страховая стоимость (>= 0).
     * @param conditionPercent Текущий процент сохранности в диапазоне [0, 100].
     * @param dimensions Физические габариты и масса предмета.
     * @param era Историческая эпоха для сопоставления или атрибуции.
     * @throw InvalidArtifactDataException При передаче некорректных параметров инициализации.
     */
    Artifact(const std::string& inventoryNumber,
             const std::string& title,
             int creationYear,
             double estimatedValue,
             double conditionPercent,
             const Dimensions& dimensions,
             const HistoricalEra& era);
    /// @brief Виртуальный деструктор по умолчанию.
    virtual ~Artifact() = default;

    /**
     * @brief Возвращает true, если текущая сохранность conditionPercent_ ниже критического порога 40%.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool needsRestoration() const;
    /**
     * @brief Снижает сохранность на wearPoints (с нижней границей 0%).
     * @param wearPoints Величина износа/повреждения в процентных пунктах (>= 0).
     * @throw InvalidArtifactDataException При нарушении бизнес-правил или инвариантов операции.
     */
    void degradeCondition(double wearPoints);
    /**
     * @brief Повышает сохранность на restoredPoints (с верхней границей 100%).
     * @param restoredPoints Прирост сохранности после реставрации в процентных пунктах (>= 0).
     * @throw InvalidArtifactDataException При нарушении бизнес-правил или инвариантов операции.
     */
    void repairCondition(double restoredPoints);
    /**
     * @brief Рассчитывает годовой страховой взнос: estimatedValue_ * annualRate * (1.35 для древних эпох, иначе 1.0).
     * @param annualRate Годовая страховая тарифная ставка в долях (>= 0).
     * @return Значение типа @c double, вычисленное методом.
     * @throw InvalidArtifactDataException При нарушении бизнес-правил или инвариантов операции.
     */
    virtual double calculateInsurancePremium(double annualRate) const;

    /// @brief Устанавливает флаг размещения экспоната на экспозиции.
    void setOnDisplay(bool displayed) noexcept { isOnDisplay_ = displayed; }
    /// @brief Возвращает инвентарный номер экспоната.
    const std::string& inventoryNumber() const noexcept { return inventoryNumber_; }
    /// @brief Возвращает наименование.
    const std::string& title() const noexcept { return title_; }
    /// @brief Возвращает год создания.
    int creationYear() const noexcept { return creationYear_; }
    /// @brief Возвращает оценочную стоимость.
    double estimatedValue() const noexcept { return estimatedValue_; }
    /// @brief Возвращает текущий процент сохранности.
    double conditionPercent() const noexcept { return conditionPercent_; }
    /// @brief Возвращает true, если экспонат выставлен в зале.
    bool isOnDisplay() const noexcept { return isOnDisplay_; }
    /// @brief Возвращает физические габариты экспоната.
    const Dimensions& dimensions() const noexcept { return dimensions_; }
    /// @brief Возвращает историческую эпоху экспоната.
    const HistoricalEra& era() const noexcept { return era_; }

private:
    std::string inventoryNumber_; ///< Уникальный инвентарный номер экспоната.
    std::string title_; ///< Наименование экспоната или выставки.
    int creationYear_; ///< Год создания музейного предмета.
    double estimatedValue_; ///< Оценочная страховая стоимость (>= 0).
    double conditionPercent_; ///< Текущий процент сохранности в диапазоне [0, 100].
    bool isOnDisplay_; ///< Признак размещения экспоната в открытой витрине/зале.
    Dimensions dimensions_; ///< Физические габариты и масса предмета.
    HistoricalEra era_; ///< Историческая эпоха для сопоставления или атрибуции.
};

/**
 * @class Painting
 * @brief Живописное полотно (картина).
 * @details Специализация Artifact для станковой живописи: учитывает тип холста, технику письма, защитное стекло и допустимую освещённость.
 * @see Artifact, ArtistProfile, ClimateControlUnit
 */
class Painting : public Artifact {
public:
    /**
     * @brief Конструктор класса Painting.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @param title Наименование экспоната или выставки.
     * @param creationYear Год создания музейного предмета.
     * @param estimatedValue Оценочная страховая стоимость (>= 0).
     * @param conditionPercent Текущий процент сохранности в диапазоне [0, 100].
     * @param dimensions Физические габариты и масса предмета.
     * @param era Историческая эпоха для сопоставления или атрибуции.
     * @param artist Профиль автора произведения.
     * @param canvasType Тип основы/холста картины.
     * @param paintMedium Техника и состав красок (например, Oil, Tempera).
     * @param maxLightLux Максимально допустимая освещённость в люксах.
     */
    Painting(const std::string& inventoryNumber,
             const std::string& title,
             int creationYear,
             double estimatedValue,
             double conditionPercent,
             const Dimensions& dimensions,
             const HistoricalEra& era,
             const ArtistProfile& artist,
             const std::string& canvasType = "Linen",
             const std::string& paintMedium = "Oil",
             double maxLightLux = 150.0);

    /**
     * @brief Проверяет безопасность освещённости currentLux.
     * @details При наличии защитного стекла порог maxLightLux_ увеличивается на +25%.
     * @param currentLux Текущий уровень освещённости в люксах.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isLightSafe(double currentLux) const;
    /**
     * @brief Устанавливает антибликовое защитное стекло (hasProtectiveGlass_ = true).
     */
    void installProtectiveGlass();
    /**
     * @brief Оценивает художественную стоимость: estimatedValue * (1.15 для Oil, иначе 1.05) * (0.5 + 0.5 * condition / 100).
     * @return Значение типа @c double, вычисленное методом.
     */
    double evaluateArtisticValue() const;

    /// @brief Возвращает признак наличия защитного стекла.
    bool hasProtectiveGlass() const noexcept { return hasProtectiveGlass_; }
    /// @brief Возвращает предельную допустимую освещённость (лк).
    double maxLightLux() const noexcept { return maxLightLux_; }
    /// @brief Возвращает профиль автора.
    const ArtistProfile& artist() const noexcept { return artist_; }

private:
    std::string canvasType_; ///< Тип основы/холста картины.
    std::string paintMedium_; ///< Техника и состав красок (например, Oil, Tempera).
    bool hasProtectiveGlass_; ///< Наличие защитного антибликового стекла.
    double maxLightLux_; ///< Максимально допустимая освещённость в люксах.
    ArtistProfile artist_; ///< Профиль автора произведения.
};

/**
 * @class Sculpture
 * @brief Скульптурное произведение.
 * @details Специализация Artifact для скульптурных произведений: рассчитывает статическую нагрузку на перекрытие и поддерживает обработку воском.
 * @see Artifact, ArtistProfile, ExhibitionHall
 */
class Sculpture : public Artifact {
public:
    /**
     * @brief Конструктор класса Sculpture.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @param title Наименование экспоната или выставки.
     * @param creationYear Год создания музейного предмета.
     * @param estimatedValue Оценочная страховая стоимость (>= 0).
     * @param conditionPercent Текущий процент сохранности в диапазоне [0, 100].
     * @param dimensions Физические габариты и масса предмета.
     * @param era Историческая эпоха для сопоставления или атрибуции.
     * @param artist Профиль автора произведения.
     * @param material Материал скульптуры или расходный реставрационный материал.
     * @param isOutdoorSafe Признак устойчивости скульптуры к размещению на открытом воздухе.
     */
    Sculpture(const std::string& inventoryNumber,
              const std::string& title,
              int creationYear,
              double estimatedValue,
              double conditionPercent,
              const Dimensions& dimensions,
              const HistoricalEra& era,
              const ArtistProfile& artist,
              const std::string& material = "Marble",
              bool isOutdoorSafe = false);

    /**
     * @brief Вычисляет статическую нагрузку на перекрытие (кг/м²): weightKg / max(0.01, footprintArea).
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateFloorLoadKgPerSqM() const;
    /**
     * @brief Проверяет, не превышает ли нагрузка скульптуры допустимый предел перекрытия maxLoadKgPerSqM.
     * @param maxLoadKgPerSqM Предельная допустимая нагрузка на перекрытие зала (кг/м²).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool verifyFloorCapacity(double maxLoadKgPerSqM) const;
    /**
     * @brief Обрабатывает скульптуру защитным воском: разрешает уличное размещение и повышает сохранность на +5%.
     */
    void applyProtectiveWax();

    /// @brief Возвращает материал скульптуры.
    const std::string& material() const noexcept { return material_; }
    /// @brief Возвращает true, если скульптура тяжелее 300 кг и требует усиленного пола.
    bool requiresReinforcedFloor() const noexcept { return requiresReinforcedFloor_; }
    /// @brief Возвращает признак пригодности для уличной установки.
    bool isOutdoorSafe() const noexcept { return isOutdoorSafe_; }

private:
    std::string material_; ///< Материал скульптуры или расходный реставрационный материал.
    bool requiresReinforcedFloor_; ///< Требование усиленного перекрытия (масса >= 300 кг).
    bool isOutdoorSafe_; ///< Признак устойчивости скульптуры к размещению на открытом воздухе.
    ArtistProfile artist_; ///< Профиль автора произведения.
};

/**
 * @class AncientManuscript
 * @brief Древняя рукопись или фолиант.
 * @details Специализация Artifact для древних рукописей и фолиантов: контролирует гигроскопический режим, оцифровку и объём текста.
 * @see Artifact, ClimateControlUnit
 */
class AncientManuscript : public Artifact {
public:
    /**
     * @brief Конструктор класса AncientManuscript.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @param title Наименование экспоната или выставки.
     * @param creationYear Год создания музейного предмета.
     * @param estimatedValue Оценочная страховая стоимость (>= 0).
     * @param conditionPercent Текущий процент сохранности в диапазоне [0, 100].
     * @param dimensions Физические габариты и масса предмета.
     * @param era Историческая эпоха для сопоставления или атрибуции.
     * @param language Язык рукописи, экскурсии или озвучки аудиогида.
     * @param pageCount Количество страниц или листов рукописи (> 0).
     * @param inkType Состав чернил рукописи.
     * @param maxHumidityPercent Верхний допустимый порог относительной влажности (%).
     * @throw InvalidArtifactDataException При передаче некорректных параметров инициализации.
     */
    AncientManuscript(const std::string& inventoryNumber,
                      const std::string& title,
                      int creationYear,
                      double estimatedValue,
                      double conditionPercent,
                      const Dimensions& dimensions,
                      const HistoricalEra& era,
                      const std::string& language = "Latin",
                      int pageCount = 120,
                      const std::string& inkType = "Iron gall",
                      double maxHumidityPercent = 55.0);

    /**
     * @brief Выполняет цифровое сканирование (isDigitized_ = true) и добавляет +3% к сохранности.
     */
    void digitizeManuscript();
    /**
     * @brief Проверяет нахождение влажности currentHumidity в допустимом диапазоне [30%, maxHumidityPercent_].
     * @param currentHumidity Текущее значение относительной влажности (%).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isHumiditySafe(double currentHumidity) const;
    /**
     * @brief Оценивает трудоёмкость научного чтения/перевода из расчёта 15 страниц в час.
     * @return Значение типа @c double, вычисленное методом.
     */
    double estimateReadingHours() const;

    /// @brief Возвращает статус оцифровки рукописи.
    bool isDigitized() const noexcept { return isDigitized_; }
    /// @brief Возвращает количество страниц рукописи.
    int pageCount() const noexcept { return pageCount_; }
    /// @brief Возвращает максимально допустимую влажность (%).
    double maxHumidityPercent() const noexcept { return maxHumidityPercent_; }

private:
    std::string language_; ///< Язык рукописи, экскурсии или озвучки аудиогида.
    int pageCount_; ///< Количество страниц или листов рукописи (> 0).
    std::string inkType_; ///< Состав чернил рукописи.
    bool isDigitized_; ///< Флаг наличия цифровой копии рукописи.
    double maxHumidityPercent_; ///< Верхний допустимый порог относительной влажности (%).
};

/**
 * @class ArcheologicalFind
 * @brief Археологическая находка из раскопок.
 * @details Специализация Artifact для археологических находок: хранит место и глубину раскопок, хрупкость (1–10) и статус радиоуглеродной проверки.
 * @see Artifact
 */
class ArcheologicalFind : public Artifact {
public:
    /**
     * @brief Конструктор класса ArcheologicalFind.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @param title Наименование экспоната или выставки.
     * @param creationYear Год создания музейного предмета.
     * @param estimatedValue Оценочная страховая стоимость (>= 0).
     * @param conditionPercent Текущий процент сохранности в диапазоне [0, 100].
     * @param dimensions Физические габариты и масса предмета.
     * @param era Историческая эпоха для сопоставления или атрибуции.
     * @param excavationSite Место проведения археологических раскопок.
     * @param discoveryYear Год обнаружения находки.
     * @param soilDepthMeters Глубина залегания культурного слоя в метрах.
     * @param fragilityLevel Уровень хрупкости по шкале от 1 до 10.
     */
    ArcheologicalFind(const std::string& inventoryNumber,
                      const std::string& title,
                      int creationYear,
                      double estimatedValue,
                      double conditionPercent,
                      const Dimensions& dimensions,
                      const HistoricalEra& era,
                      const std::string& excavationSite,
                      int discoveryYear,
                      double soilDepthMeters,
                      int fragilityLevel);

    /**
     * @brief Сверяет радиоуглеродную датировку с годом создания (допуск ±150 лет) и сохраняет флаг подлинности.
     * @param carbonDatingYear Год по результатам радиоуглеродного анализа.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool verifyAuthenticity(int carbonDatingYear);
    /**
     * @brief Рассчитывает интегральный риск перевозки [0.0, 1.0]: 0.6 * (fragility / 10) + 0.4 * (1 - condition / 100).
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateTransportRisk() const;
    /**
     * @brief Возвращает число лет хранения после раскопок: max(0, currentYear - discoveryYear_).
     * @param currentYear Текущий календарный год для расчёта срока хранения.
     * @return Значение типа @c int, вычисленное методом.
     */
    int yearsSinceDiscovery(int currentYear) const;

    /// @brief Возвращает результат проверки подлинности.
    bool isAuthenticVerified() const noexcept { return isAuthenticVerified_; }
    /// @brief Возвращает уровень хрупкости (1–10).
    int fragilityLevel() const noexcept { return fragilityLevel_; }
    /// @brief Возвращает место раскопок.
    const std::string& excavationSite() const noexcept { return excavationSite_; }

private:
    std::string excavationSite_; ///< Место проведения археологических раскопок.
    int discoveryYear_; ///< Год обнаружения находки.
    double soilDepthMeters_; ///< Глубина залегания культурного слоя в метрах.
    int fragilityLevel_; ///< Уровень хрупкости по шкале от 1 до 10.
    bool isAuthenticVerified_; ///< Флаг подтверждённой радиоуглеродной подлинности.
};

#endif // MUSEUM_ARTIFACTS_H
