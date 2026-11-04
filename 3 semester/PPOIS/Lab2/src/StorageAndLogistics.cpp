/**
 * @file StorageAndLogistics.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов складской логистики, межмузейного обмена и каталога.
 */

#include "StorageAndLogistics.h"

#include <algorithm>

namespace {
constexpr double kCrateTareWeightKg = 18.0;
constexpr double kShockproofExtraWeightKg = 7.0;
constexpr double kHighValueThreshold = 50000.0;
constexpr double kMinHighValuePartnerRating = 80.0;
constexpr double kMinDepositCoverageRatio = 0.20;
constexpr double kPartnerRewardDelta = 3.0;
constexpr double kPartnerDamagePenalty = 15.0;
constexpr double kDailyFeeRate = 0.005;
} // namespace

StorageRack::StorageRack(const std::string& rackCode, int shelfCount, double maxWeightKg)
    : rackCode_(rackCode),
      shelfCount_(std::max(1, shelfCount)),
      maxWeightKg_(maxWeightKg),
      currentWeightKg_(0.0),
      storedCount_(0) {}

void StorageRack::placeOnShelf(const Artifact& artifact) {
    double weight = artifact.dimensions().weightKg();
    if (currentWeightKg_ + weight > maxWeightKg_) {
        throw HallOvercapacityException(rackCode_, static_cast<int>(maxWeightKg_));
    }
    currentWeightKg_ += weight;
    ++storedCount_;
}

void StorageRack::removeFromShelf(const Artifact& artifact) {
    if (storedCount_ <= 0) {
        throw ArtifactNotFoundException(artifact.inventoryNumber());
    }
    currentWeightKg_ = std::max(0.0, currentWeightKg_ - artifact.dimensions().weightKg());
    --storedCount_;
}

double StorageRack::remainingWeightCapacityKg() const {
    return std::max(0.0, maxWeightKg_ - currentWeightKg_);
}

TransportCrate::TransportCrate(const std::string& crateId,
                               const Dimensions& innerBounds,
                               bool isShockproof,
                               bool isClimateSealed)
    : crateId_(crateId),
      innerBounds_(innerBounds),
      isShockproof_(isShockproof),
      isClimateSealed_(isClimateSealed),
      packedArtifactId_("") {}

void TransportCrate::packArtifact(const Artifact& artifact) {
    if (artifact.needsRestoration()) {
        throw RestorationRequiredException(artifact.title(), artifact.conditionPercent());
    }
    if (!artifact.dimensions().fitsWithin(innerBounds_)) {
        throw InvalidArtifactDataException("Artifact does not fit inside transport crate " + crateId_);
    }
    packedArtifactId_ = artifact.inventoryNumber();
}

void TransportCrate::unpackArtifact() {
    packedArtifactId_.clear();
}

double TransportCrate::calculateGrossWeightKg(const Artifact& artifact) const {
    double extra = isShockproof_ ? kShockproofExtraWeightKg : 0.0;
    return artifact.dimensions().weightKg() + kCrateTareWeightKg + extra;
}

PartnerMuseum::PartnerMuseum(const std::string& partnerName,
                             const std::string& country,
                             const std::string& city,
                             double reliabilityRating,
                             bool isVerifiedPartner)
    : partnerName_(partnerName),
      country_(country),
      city_(city),
      reliabilityRating_(std::clamp(reliabilityRating, 0.0, 100.0)),
      isVerifiedPartner_(isVerifiedPartner) {}

bool PartnerMuseum::isEligibleForHighValueLoan(double artifactValue) const {
    if (!isVerifiedPartner_) {
        return false;
    }
    if (artifactValue >= kHighValueThreshold) {
        return reliabilityRating_ >= kMinHighValuePartnerRating;
    }
    return reliabilityRating_ >= 50.0;
}

void PartnerMuseum::upgradeRating(double delta) {
    reliabilityRating_ = std::min(100.0, reliabilityRating_ + std::max(0.0, delta));
}

void PartnerMuseum::penalizeRating(double penalty) {
    reliabilityRating_ = std::max(0.0, reliabilityRating_ - std::max(0.0, penalty));
}

LoanAgreement::LoanAgreement(const std::string& agreementId,
                             const std::string& artifactId,
                             int durationDays,
                             double insuranceDeposit,
                             const PartnerMuseum& partner)
    : agreementId_(agreementId),
      artifactId_(artifactId),
      durationDays_(std::max(1, durationDays)),
      insuranceDeposit_(insuranceDeposit),
      isReturned_(false),
      partner_(partner) {}

void LoanAgreement::validateLoanForArtifact(const Artifact& artifact) const {
    if (artifact.needsRestoration()) {
        throw RestorationRequiredException(artifact.title(), artifact.conditionPercent());
    }
    if (!partner_.isEligibleForHighValueLoan(artifact.estimatedValue())) {
        throw LoanAgreementViolationException("Partner museum is not eligible for artifact value");
    }
    if (insuranceDeposit_ < artifact.estimatedValue() * kMinDepositCoverageRatio) {
        throw LoanAgreementViolationException("Insurance deposit is below minimum 20% threshold");
    }
}

void LoanAgreement::completeReturn(PartnerMuseum& partner, bool damaged) {
    isReturned_ = true;
    if (damaged) {
        partner.penalizeRating(kPartnerDamagePenalty);
    } else {
        partner.upgradeRating(kPartnerRewardDelta);
    }
}

double LoanAgreement::calculateDailyFee() const {
    return (insuranceDeposit_ * kDailyFeeRate) / static_cast<double>(durationDays_);
}

CatalogEntry::CatalogEntry(const std::string& entryCode,
                           const std::string& artifactId,
                           int hallNumber,
                           const std::string& scientificDescription)
    : entryCode_(entryCode),
      artifactId_(artifactId),
      hallNumber_(hallNumber),
      scientificDescription_(scientificDescription),
      isPublishedOnline_(false) {}

void CatalogEntry::publishToOnlineCatalog() {
    isPublishedOnline_ = true;
}

void CatalogEntry::relocateToHall(int newHallNumber) {
    hallNumber_ = newHallNumber;
}

bool CatalogEntry::matchesSearchQuery(const std::string& keyword) const {
    return artifactId_.find(keyword) != std::string::npos ||
           scientificDescription_.find(keyword) != std::string::npos ||
           entryCode_.find(keyword) != std::string::npos;
}

CollectionCatalog::CollectionCatalog(const std::string& catalogTitle, int lastUpdatedYear)
    : catalogTitle_(catalogTitle), lastUpdatedYear_(lastUpdatedYear), entries_() {}

void CollectionCatalog::registerArtifact(const Artifact& artifact,
                                         int hallNumber,
                                         const std::string& description) {
    std::string code = "CAT-" + artifact.inventoryNumber();
    CatalogEntry entry(code, artifact.inventoryNumber(), hallNumber, description);
    entry.publishToOnlineCatalog();
    entries_.push_back(entry);
}

const CatalogEntry& CollectionCatalog::findEntryByArtifactId(const std::string& artifactId) const {
    for (const CatalogEntry& entry : entries_) {
        if (entry.artifactId() == artifactId) {
            return entry;
        }
    }
    throw ArtifactNotFoundException(artifactId);
}

std::vector<CatalogEntry> CollectionCatalog::searchEntries(const std::string& keyword) const {
    std::vector<CatalogEntry> results;
    for (const CatalogEntry& entry : entries_) {
        if (entry.matchesSearchQuery(keyword)) {
            results.push_back(entry);
        }
    }
    return results;
}

int CollectionCatalog::countPublishedEntries() const {
    int count = 0;
    for (const CatalogEntry& entry : entries_) {
        if (entry.isPublishedOnline()) {
            ++count;
        }
    }
    return count;
}
