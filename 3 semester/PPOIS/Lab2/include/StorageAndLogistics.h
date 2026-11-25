/**
 * @file StorageAndLogistics.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы складской логистики, межмузейного обмена и главного каталога коллекции.
 * @details Содержит 6 классов: StorageRack, TransportCrate, PartnerMuseum,
 * LoanAgreement, CatalogEntry, CollectionCatalog.
 */

#ifndef MUSEUM_STORAGE_AND_LOGISTICS_H
#define MUSEUM_STORAGE_AND_LOGISTICS_H

#include "Artifacts.h"
#include "MuseumExceptions.h"
#include <string>
#include <vector>

/**
 * @class StorageRack
 * @brief Стеллаж в фондохранилище музея.
 * @details Многоярусный грузовой стеллаж депозитария с контролем предельной суммарной массы размещённых предметов.
 * @see Artifact
 */
class StorageRack {
public:
    /**
     * @brief Конструктор класса StorageRack.
     * @param rackCode Инвентарный код стеллажа хранения.
     * @param shelfCount Количество полок на стеллаже (>= 1).
     * @param maxWeightKg Предельная суммарная масса на стеллаже (кг).
     */
    StorageRack(const std::string& rackCode = "RACK-A1",
                int shelfCount = 5,
                double maxWeightKg = 500.0);

    /**
     * @brief Размещает экспонат на стеллаже, контролируя предел суммарной массы maxWeightKg_.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void placeOnShelf(const Artifact& artifact);
    /**
     * @brief Снимает экспонат со стеллажа, уменьшая текущую нагрузку и счётчик предметов.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @throw ArtifactNotFoundException При нарушении бизнес-правил или инвариантов операции.
     */
    void removeFromShelf(const Artifact& artifact);
    /**
     * @brief Возвращает остаточный запас грузоподъёмности стеллажа в килограммах.
     * @return Значение типа @c double, вычисленное методом.
     */
    double remainingWeightCapacityKg() const;

    /// @brief Возвращает код стеллажа.
    const std::string& rackCode() const noexcept { return rackCode_; }
    /// @brief Возвращает количество хранимых предметов.
    int storedCount() const noexcept { return storedCount_; }
    /// @brief Возвращает текущую нагрузку на стеллаж (кг).
    double currentWeightKg() const noexcept { return currentWeightKg_; }

private:
    std::string rackCode_; ///< Инвентарный код стеллажа хранения.
    int shelfCount_; ///< Количество полок на стеллаже (>= 1).
    double maxWeightKg_; ///< Предельная суммарная масса на стеллаже (кг).
    double currentWeightKg_; ///< Текущая суммарная масса предметов на стеллаже (кг).
    int storedCount_; ///< Количество предметов на стеллаже.
};

/**
 * @class TransportCrate
 * @brief Специализированный климатический кофр для перевозки экспонатов.
 * @details Защитный климатический кофр для межмузейной транспортировки экспонатов с проверкой сохранности и внутренних габаритов.
 * @see Dimensions, Artifact
 */
class TransportCrate {
public:
    /**
     * @brief Конструктор класса TransportCrate.
     * @param crateId Идентификатор транспортировочного кофра.
     * @param innerBounds Внутренние габариты и предельная масса для кофра.
     * @param isShockproof Наличие ударопрочного демпфирующего слоя.
     * @param isClimateSealed Наличие климатической герметизации кофра.
     */
    TransportCrate(const std::string& crateId,
                   const Dimensions& innerBounds,
                   bool isShockproof = true,
                   bool isClimateSealed = true);

    /**
     * @brief Упаковывает экспонат в кофр после проверки его состояния (>= 40%) и внутренних габаритов.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @throw RestorationRequiredException При нарушении бизнес-правил или инвариантов операции.
     * @throw InvalidArtifactDataException При нарушении бизнес-правил или инвариантов операции.
     */
    void packArtifact(const Artifact& artifact);
    /**
     * @brief Освобождает кофр, очищая идентификатор упакованного предмета.
     */
    void unpackArtifact();
    /**
     * @brief Рассчитывает вес брутто: масса экспоната + 18 кг тары (+ 7 кг для ударопрочного кофра).
     * @param artifact Музейный экспонат, участвующий в операции.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateGrossWeightKg(const Artifact& artifact) const;

    /// @brief Возвращает идентификатор кофра.
    const std::string& crateId() const noexcept { return crateId_; }
    /// @brief Возвращает инвентарный номер упакованного экспоната.
    const std::string& packedArtifactId() const noexcept { return packedArtifactId_; }
    /// @brief Возвращает true, если кофр пуст.
    bool isEmpty() const noexcept { return packedArtifactId_.empty(); }

private:
    std::string crateId_; ///< Идентификатор транспортировочного кофра.
    Dimensions innerBounds_; ///< Внутренние габариты и предельная масса для кофра.
    bool isShockproof_; ///< Наличие ударопрочного демпфирующего слоя.
    bool isClimateSealed_; ///< Наличие климатической герметизации кофра.
    std::string packedArtifactId_; ///< Инвентарный номер упакованного экспоната (пусто, если кофр свободен).
};

/**
 * @class PartnerMuseum
 * @brief Музей-партнёр для межмузейного обмена и временных выставок.
 * @details Профиль внешнего музея-партнёра с рейтингом надёжности (0–100) и статусом верификации для выдачи дорогих экспонатов.
 * @see LoanAgreement
 */
class PartnerMuseum {
public:
    /**
     * @brief Конструктор класса PartnerMuseum.
     * @param partnerName Название музея-партнёра.
     * @param country Страна расположения партнёра.
     * @param city Город расположения музея.
     * @param reliabilityRating Рейтинг надёжности партнёра в диапазоне [0, 100].
     * @param isVerifiedPartner Флаг прохождения официальной аккредитации партнёра.
     */
    PartnerMuseum(const std::string& partnerName = "Louvre",
                  const std::string& country = "France",
                  const std::string& city = "Paris",
                  double reliabilityRating = 95.0,
                  bool isVerifiedPartner = true);

    /**
     * @brief Проверяет допуск партнёра: обязательна верификация.
     * @details Рейтинг >= 80 для предметов >= 50 000, иначе >= 50.
     * @param artifactValue Оценочная стоимость передаваемого экспоната.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isEligibleForHighValueLoan(double artifactValue) const;
    /**
     * @brief Повышает рейтинг надёжности партнёра на delta (верхняя граница 100.0).
     * @param delta Величина повышения рейтинга (>= 0).
     */
    void upgradeRating(double delta);
    /**
     * @brief Снижает рейтинг надёжности партнёра на penalty (нижняя граница 0.0).
     * @param penalty Величина штрафного снижения рейтинга (>= 0).
     */
    void penalizeRating(double penalty);

    /// @brief Возвращает название музея-партнёра.
    const std::string& partnerName() const noexcept { return partnerName_; }
    /// @brief Возвращает текущий рейтинг надёжности партнёра.
    double reliabilityRating() const noexcept { return reliabilityRating_; }
    /// @brief Возвращает признак верификации партнёра.
    bool isVerifiedPartner() const noexcept { return isVerifiedPartner_; }

private:
    std::string partnerName_; ///< Название музея-партнёра.
    std::string country_; ///< Страна расположения партнёра.
    std::string city_; ///< Город расположения музея.
    double reliabilityRating_; ///< Рейтинг надёжности партнёра в диапазоне [0, 100].
    bool isVerifiedPartner_; ///< Флаг прохождения официальной аккредитации партнёра.
};

/**
 * @class LoanAgreement
 * @brief Договор временной передачи экспоната в музей-партнёр.
 * @details Юридический контракт временной передачи экспоната партнёру с проверкой залога (>= 20%) и перерасчётом рейтинга при возврате.
 * @see PartnerMuseum, Artifact
 */
class LoanAgreement {
public:
    /**
     * @brief Конструктор класса LoanAgreement.
     * @param agreementId Регистрационный номер договора межмузейного займа.
     * @param artifactId Инвентарный номер экспоната.
     * @param durationDays Срок действия договора займа в днях (>= 1).
     * @param insuranceDeposit Сумма страхового залога по договору.
     * @param partner Музей-партнёр, принимающий экспонат.
     */
    LoanAgreement(const std::string& agreementId,
                  const std::string& artifactId,
                  int durationDays,
                  double insuranceDeposit,
                  const PartnerMuseum& partner);

    /**
     * @brief Проверяет сохранность экспоната, рейтинг музея-партнёра и размер страхового депозита (>= 20% стоимости).
     * @param artifact Музейный экспонат, участвующий в операции.
     * @throw RestorationRequiredException При нарушении бизнес-правил или инвариантов операции.
     * @throw LoanAgreementViolationException При нарушении бизнес-правил или инвариантов операции.
     */
    void validateLoanForArtifact(const Artifact& artifact) const;
    /**
     * @brief Фиксирует возврат экспоната: штрафует партнёра (-15) при повреждении или поощряет (+3) при сохранности.
     * @param partner Музей-партнёр, принимающий экспонат.
     * @param damaged Признак наличия повреждений при возврате предмета.
     */
    void completeReturn(PartnerMuseum& partner, bool damaged);
    /**
     * @brief Вычисляет суточную стоимость аренды: (insuranceDeposit_ * 0.005) / durationDays_.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateDailyFee() const;

    /// @brief Возвращает номер договора займа.
    const std::string& agreementId() const noexcept { return agreementId_; }
    /// @brief Возвращает признак возврата экспоната.
    bool isReturned() const noexcept { return isReturned_; }
    /// @brief Возвращает профиль музея-партнёра.
    const PartnerMuseum& partner() const noexcept { return partner_; }

private:
    std::string agreementId_; ///< Регистрационный номер договора межмузейного займа.
    std::string artifactId_; ///< Инвентарный номер экспоната.
    int durationDays_; ///< Срок действия договора займа в днях (>= 1).
    double insuranceDeposit_; ///< Сумма страхового залога по договору.
    bool isReturned_; ///< Признак закрытия договора и возврата экспоната.
    PartnerMuseum partner_; ///< Музей-партнёр, принимающий экспонат.
};

/**
 * @class CatalogEntry
 * @brief Учётная карточка экспоната в главном каталоге музея.
 * @details Учётная карточка экспоната в главном каталоге с номером зала, научным описанием и флагом онлайн-публикации.
 * @see CollectionCatalog
 */
class CatalogEntry {
public:
    /**
     * @brief Конструктор класса CatalogEntry.
     * @param entryCode Уникальный код учётной карточки каталога.
     * @param artifactId Инвентарный номер экспоната.
     * @param hallNumber Порядковый номер выставочного зала.
     * @param scientificDescription Научное и атрибуционное описание экспоната.
     */
    CatalogEntry(const std::string& entryCode,
                 const std::string& artifactId,
                 int hallNumber,
                 const std::string& scientificDescription);

    /**
     * @brief Публикует учётную карточку в онлайн-каталоге музея.
     */
    void publishToOnlineCatalog();
    /**
     * @brief Обновляет номер зала размещения экспоната в карточке каталога.
     * @param newHallNumber Номер нового зала размещения экспоната.
     */
    void relocateToHall(int newHallNumber);
    /**
     * @brief Выполняет подстрочный поиск keyword по коду записи, инвентарному номеру и научному описанию.
     * @param keyword Поисковая подстрока для фильтрации карточек каталога.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool matchesSearchQuery(const std::string& keyword) const;

    /// @brief Возвращает код карточки каталога.
    const std::string& entryCode() const noexcept { return entryCode_; }
    /// @brief Возвращает инвентарный номер экспоната.
    const std::string& artifactId() const noexcept { return artifactId_; }
    /// @brief Возвращает номер выставочного зала.
    int hallNumber() const noexcept { return hallNumber_; }
    /// @brief Возвращает статус публикации в онлайн-каталоге.
    bool isPublishedOnline() const noexcept { return isPublishedOnline_; }

private:
    std::string entryCode_; ///< Уникальный код учётной карточки каталога.
    std::string artifactId_; ///< Инвентарный номер экспоната.
    int hallNumber_; ///< Порядковый номер выставочного зала.
    std::string scientificDescription_; ///< Научное и атрибуционное описание экспоната.
    bool isPublishedOnline_; ///< Признак публикации карточки в онлайн-каталоге.
};

/**
 * @class CollectionCatalog
 * @brief Главный реестр-каталог музейной коллекции.
 * @details Главный реестр музейной коллекции с автоматической регистрацией карточек и подстрочным поиском.
 * @see CatalogEntry, Artifact, Museum
 */
class CollectionCatalog {
public:
    /**
     * @brief Конструктор класса CollectionCatalog.
     * @param catalogTitle Наименование главного каталога коллекции.
     * @param lastUpdatedYear Год последнего обновления реестра.
     */
    explicit CollectionCatalog(const std::string& catalogTitle = "State Museum Catalog",
                               int lastUpdatedYear = 2026);

    /**
     * @brief Создаёт карточку с кодом CAT-<inv>, публикует её онлайн и добавляет в реестр.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param hallNumber Порядковый номер выставочного зала.
     * @param description Краткое научное описание регистрируемого экспоната.
     */
    void registerArtifact(const Artifact& artifact, int hallNumber, const std::string& description);
    /**
     * @brief Возвращает константную ссылку на учётную карточку по инвентарному номеру.
     * @param artifactId Инвентарный номер экспоната.
     * @return Значение типа @c const CatalogEntry&, вычисленное методом.
     * @throw ArtifactNotFoundException При нарушении бизнес-правил или инвариантов операции.
     */
    const CatalogEntry& findEntryByArtifactId(const std::string& artifactId) const;
    /**
     * @brief Возвращает все карточки каталога, совпадающие с поисковым запросом keyword.
     * @param keyword Поисковая подстрока для фильтрации карточек каталога.
     * @return Значение типа @c std::vector<CatalogEntry>, вычисленное методом.
     */
    std::vector<CatalogEntry> searchEntries(const std::string& keyword) const;
    /**
     * @brief Подсчитывает количество карточек со статусом онлайн-публикации.
     * @return Значение типа @c int, вычисленное методом.
     */
    int countPublishedEntries() const;

    /// @brief Возвращает общее число записей в каталоге.
    std::size_t totalEntries() const noexcept { return entries_.size(); }
    /// @brief Возвращает константный список всех карточек каталога.
    const std::vector<CatalogEntry>& entries() const noexcept { return entries_; }

private:
    std::string catalogTitle_; ///< Наименование главного каталога коллекции.
    int lastUpdatedYear_; ///< Год последнего обновления реестра.
    std::vector<CatalogEntry> entries_; ///< Коллекция учётных карточек каталога.
};

#endif // MUSEUM_STORAGE_AND_LOGISTICS_H
