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
 */
class StorageRack {
public:
    StorageRack(const std::string& rackCode = "RACK-A1",
                int shelfCount = 5,
                double maxWeightKg = 500.0);

    void placeOnShelf(const Artifact& artifact);
    void removeFromShelf(const Artifact& artifact);
    double remainingWeightCapacityKg() const;

    const std::string& rackCode() const noexcept { return rackCode_; }
    int storedCount() const noexcept { return storedCount_; }
    double currentWeightKg() const noexcept { return currentWeightKg_; }

private:
    std::string rackCode_;
    int shelfCount_;
    double maxWeightKg_;
    double currentWeightKg_;
    int storedCount_;
};

/**
 * @class TransportCrate
 * @brief Специализированный климатический кофр для перевозки экспонатов.
 */
class TransportCrate {
public:
    TransportCrate(const std::string& crateId,
                   const Dimensions& innerBounds,
                   bool isShockproof = true,
                   bool isClimateSealed = true);

    void packArtifact(const Artifact& artifact);
    void unpackArtifact();
    double calculateGrossWeightKg(const Artifact& artifact) const;

    const std::string& crateId() const noexcept { return crateId_; }
    const std::string& packedArtifactId() const noexcept { return packedArtifactId_; }
    bool isEmpty() const noexcept { return packedArtifactId_.empty(); }

private:
    std::string crateId_;
    Dimensions innerBounds_;
    bool isShockproof_;
    bool isClimateSealed_;
    std::string packedArtifactId_;
};

/**
 * @class PartnerMuseum
 * @brief Музей-партнёр для межмузейного обмена и временных выставок.
 */
class PartnerMuseum {
public:
    PartnerMuseum(const std::string& partnerName = "Louvre",
                  const std::string& country = "France",
                  const std::string& city = "Paris",
                  double reliabilityRating = 95.0,
                  bool isVerifiedPartner = true);

    bool isEligibleForHighValueLoan(double artifactValue) const;
    void upgradeRating(double delta);
    void penalizeRating(double penalty);

    const std::string& partnerName() const noexcept { return partnerName_; }
    double reliabilityRating() const noexcept { return reliabilityRating_; }
    bool isVerifiedPartner() const noexcept { return isVerifiedPartner_; }

private:
    std::string partnerName_;
    std::string country_;
    std::string city_;
    double reliabilityRating_;
    bool isVerifiedPartner_;
};

/**
 * @class LoanAgreement
 * @brief Договор временной передачи экспоната в музей-партнёр.
 */
class LoanAgreement {
public:
    LoanAgreement(const std::string& agreementId,
                  const std::string& artifactId,
                  int durationDays,
                  double insuranceDeposit,
                  const PartnerMuseum& partner);

    void validateLoanForArtifact(const Artifact& artifact) const;
    void completeReturn(PartnerMuseum& partner, bool damaged);
    double calculateDailyFee() const;

    const std::string& agreementId() const noexcept { return agreementId_; }
    bool isReturned() const noexcept { return isReturned_; }
    const PartnerMuseum& partner() const noexcept { return partner_; }

private:
    std::string agreementId_;
    std::string artifactId_;
    int durationDays_;
    double insuranceDeposit_;
    bool isReturned_;
    PartnerMuseum partner_;
};

/**
 * @class CatalogEntry
 * @brief Учётная карточка экспоната в главном каталоге музея.
 */
class CatalogEntry {
public:
    CatalogEntry(const std::string& entryCode,
                 const std::string& artifactId,
                 int hallNumber,
                 const std::string& scientificDescription);

    void publishToOnlineCatalog();
    void relocateToHall(int newHallNumber);
    bool matchesSearchQuery(const std::string& keyword) const;

    const std::string& entryCode() const noexcept { return entryCode_; }
    const std::string& artifactId() const noexcept { return artifactId_; }
    int hallNumber() const noexcept { return hallNumber_; }
    bool isPublishedOnline() const noexcept { return isPublishedOnline_; }

private:
    std::string entryCode_;
    std::string artifactId_;
    int hallNumber_;
    std::string scientificDescription_;
    bool isPublishedOnline_;
};

/**
 * @class CollectionCatalog
 * @brief Главный реестр-каталог музейной коллекции.
 */
class CollectionCatalog {
public:
    explicit CollectionCatalog(const std::string& catalogTitle = "State Museum Catalog",
                               int lastUpdatedYear = 2026);

    void registerArtifact(const Artifact& artifact, int hallNumber, const std::string& description);
    const CatalogEntry& findEntryByArtifactId(const std::string& artifactId) const;
    std::vector<CatalogEntry> searchEntries(const std::string& keyword) const;
    int countPublishedEntries() const;

    std::size_t totalEntries() const noexcept { return entries_.size(); }
    const std::vector<CatalogEntry>& entries() const noexcept { return entries_; }

private:
    std::string catalogTitle_;
    int lastUpdatedYear_;
    std::vector<CatalogEntry> entries_;
};

#endif // MUSEUM_STORAGE_AND_LOGISTICS_H
