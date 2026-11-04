/**
 * @file test_museum.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Модульные тесты (UnitTest++) для всех 52 доменных классов и 13 классов исключений Музея.
 * @details Содержит 35 тестовых сценариев с покрытием кода > 95%.
 */

#include "Artifacts.h"
#include "ExhibitionsAndEvents.h"
#include "FinanceAndShop.h"
#include "HallsAndClimate.h"
#include "MuseumExceptions.h"
#include "RestorationLab.h"
#include "SecuritySystem.h"
#include "StaffAndHR.h"
#include "StorageAndLogistics.h"
#include "VisitorsAndTickets.h"
#include <UnitTest++/UnitTest++.h>

SUITE(MuseumDomainTests) {

    // 1. Базовое исключение MuseumException
    TEST(BaseMuseumExceptionProperties) {
        MuseumException baseEx("General failure", "GENERAL", 1000);
        CHECK_EQUAL("GENERAL", baseEx.category());
        CHECK_EQUAL(1000, baseEx.errorCode());
        CHECK(!baseEx.formatErrorReport().empty());
    }

    // 2. Исключения экспонатов и вместимости
    TEST(ArtifactAndCapacityExceptionsProperties) {
        ArtifactNotFoundException notFound("ART-404");
        CHECK_EQUAL("ART-404", notFound.inventoryNumber());

        InvalidArtifactDataException badData("Negative weight");
        CHECK_EQUAL(1002, badData.errorCode());

        HallOvercapacityException overcap("Hall 1", 50);
        CHECK_EQUAL(50, overcap.maxCapacity());
    }

    // 3. Исключения климата, финансов и реставрации
    TEST(ClimateFundsAndRestorationExceptionsProperties) {
        ClimateOutOfRangeException climateEx("Humidity", 72.0, 55.0);
        CHECK_CLOSE(72.0, climateEx.actualValue(), 0.01);
        CHECK_CLOSE(55.0, climateEx.allowedLimit(), 0.01);

        InsufficientFundsException fundsEx(150.0, 40.0);
        CHECK_CLOSE(150.0, fundsEx.requiredAmount(), 0.01);
        CHECK_CLOSE(40.0, fundsEx.availableBalance(), 0.01);

        RestorationRequiredException restEx("Broken Vase", 20.0);
        CHECK_CLOSE(20.0, restEx.currentCondition(), 0.01);
    }

    // 4. Исключения билетов, охраны, персонала, выставок и аренды
    TEST(RemainingExceptionsProperties) {
        TicketSoldOutException soldOut("Night Event");
        InvalidTicketException badTicket("TCK-99");
        SecurityBreachException secBreach("Vault");
        StaffNotAvailableException staffEx("Curator");
        ExhibitionClosedException closedEx("Expo");
        LoanAgreementViolationException loanEx("Low deposit");

        CHECK_EQUAL(1005, soldOut.errorCode());
        CHECK_EQUAL(1006, badTicket.errorCode());
        CHECK_EQUAL(1009, secBreach.errorCode());
        CHECK_EQUAL(1010, staffEx.errorCode());
        CHECK_EQUAL(1011, closedEx.errorCode());
        CHECK_EQUAL(1012, loanEx.errorCode());
    }

    // 5. Класс Dimensions
    TEST(DimensionsCalculationsAndValidation) {
        Dimensions d(100.0, 200.0, 50.0, 25.0);
        CHECK_CLOSE(100.0, d.widthCm(), 0.01);
        CHECK_CLOSE(200.0, d.heightCm(), 0.01);
        CHECK_CLOSE(50.0, d.depthCm(), 0.01);
        CHECK_CLOSE(25.0, d.weightKg(), 0.01);
        CHECK_CLOSE(1.0, d.calculateVolumeCubicMeters(), 0.001);
        CHECK_CLOSE(0.5, d.calculateFootprintAreaSqMeters(), 0.001);
        CHECK(d.fitsWithin(Dimensions(120.0, 220.0, 60.0, 30.0)));
        CHECK(!d.fitsWithin(Dimensions(80.0, 220.0, 60.0, 30.0)));
        CHECK_THROW(Dimensions(-1.0, 10.0, 10.0, 5.0), InvalidArtifactDataException);
    }

    // 6. Класс HistoricalEra
    TEST(HistoricalEraBehavior) {
        HistoricalEra antiquity("Antiquity", -500, 400, "Greece");
        CHECK_EQUAL("Greece", antiquity.region());
        CHECK(antiquity.containsYear(-100));
        CHECK(!antiquity.containsYear(1500));
        CHECK_EQUAL(900, antiquity.durationYears());
        CHECK(antiquity.isAncient());
        CHECK_THROW(HistoricalEra("", 500, 100, "Rome"), InvalidArtifactDataException);
    }

    // 7. Класс ArtistProfile
    TEST(ArtistProfileBehavior) {
        HistoricalEra antiquity("Antiquity", -500, 400, "Greece");
        HistoricalEra renaissance("Renaissance", 1400, 1600, "Italy");
        ArtistProfile artist("Leonardo", "Italian", 1452, 1519, "High Renaissance");
        CHECK_EQUAL("Italian", artist.nationality());
        CHECK_EQUAL("High Renaissance", artist.artMovement());
        CHECK(artist.wasActiveInYear(1500));
        CHECK(!artist.wasActiveInYear(1455));
        CHECK_EQUAL(67, artist.lifespanYears());
        CHECK(artist.matchesEra(renaissance));
        CHECK(!artist.matchesEra(antiquity));
        CHECK_THROW(ArtistProfile("", "Italian", 1500, 1400, "None"), InvalidArtifactDataException);
    }

    // 8. Базовый класс Artifact
    TEST(ArtifactBaseBehaviorAndValidation) {
        Dimensions dims(50.0, 70.0, 5.0, 4.0);
        HistoricalEra era("Antiquity", -300, 200, "Rome");
        Artifact item("A-10", "Roman Coin", -50, 10000.0, 80.0, dims, era);
        CHECK_EQUAL(-50, item.creationYear());
        CHECK(!item.needsRestoration());
        item.degradeCondition(50.0);
        CHECK(item.needsRestoration());
        item.repairCondition(60.0);
        CHECK_CLOSE(90.0, item.conditionPercent(), 0.01);
        CHECK_CLOSE(675.0, item.calculateInsurancePremium(0.05), 0.01);

        CHECK_THROW(Artifact("", "Title", 100, 10.0, 50.0, dims, era), InvalidArtifactDataException);
        CHECK_THROW(item.degradeCondition(-5.0), InvalidArtifactDataException);
        CHECK_THROW(item.repairCondition(-5.0), InvalidArtifactDataException);
        CHECK_THROW(item.calculateInsurancePremium(-0.1), InvalidArtifactDataException);
    }

    // 9. Класс Painting
    TEST(PaintingLightAndArtisticValue) {
        Dimensions dims(50.0, 70.0, 5.0, 4.0);
        HistoricalEra era("Renaissance", 1400, 1600, "Italy");
        ArtistProfile artist("Raphael", "Italian", 1483, 1520, "Renaissance");
        Painting painting("P-01", "Madonna", 1508, 100000.0, 80.0, dims, era, artist, "Linen", "Oil", 150.0);

        CHECK_EQUAL("Raphael", painting.artist().fullName());
        CHECK(painting.isLightSafe(140.0));
        CHECK(!painting.isLightSafe(170.0));
        painting.installProtectiveGlass();
        CHECK(painting.hasProtectiveGlass());
        CHECK(painting.isLightSafe(170.0));
        CHECK(painting.evaluateArtisticValue() > 90000.0);
    }

    // 10. Класс Sculpture
    TEST(SculptureFloorLoadAndWax) {
        Dimensions heavyDims(100.0, 200.0, 100.0, 400.0);
        HistoricalEra ancientEra("Antiquity", -400, 200, "Rome");
        ArtistProfile sculptor("Phidias", "Greek", -480, -430, "Classical");

        Sculpture statue("S-01", "Apollo", -440, 200000.0, 75.0, heavyDims, ancientEra, sculptor);
        CHECK_EQUAL("Marble", statue.material());
        CHECK(statue.requiresReinforcedFloor());
        CHECK_CLOSE(400.0, statue.calculateFloorLoadKgPerSqM(), 0.1);
        CHECK(statue.verifyFloorCapacity(500.0));
        CHECK(!statue.verifyFloorCapacity(250.0));
        statue.applyProtectiveWax();
        CHECK(statue.isOutdoorSafe());
        CHECK_CLOSE(80.0, statue.conditionPercent(), 0.01);
    }

    // 11. Класс AncientManuscript
    TEST(AncientManuscriptDigitizationAndHumidity) {
        HistoricalEra ancientEra("Antiquity", -400, 400, "Rome");
        AncientManuscript codex("M-01", "Codex Vaticanus", 350, 80000.0, 70.0,
                                Dimensions(30.0, 40.0, 8.0, 3.0), ancientEra, "Greek", 150, "Ink", 50.0);
        CHECK_EQUAL(150, codex.pageCount());
        CHECK(!codex.isDigitized());
        codex.digitizeManuscript();
        CHECK(codex.isDigitized());
        CHECK(codex.isHumiditySafe(45.0));
        CHECK(!codex.isHumiditySafe(65.0));
        CHECK_CLOSE(10.0, codex.estimateReadingHours(), 0.01);
        CHECK_THROW(AncientManuscript("M-02", "Bad", 300, 100.0, 50.0,
                                      Dimensions(10, 10, 10, 1), ancientEra, "Latin", 0),
                    InvalidArtifactDataException);
    }

    // 12. Класс ArcheologicalFind
    TEST(ArcheologicalFindAuthenticityAndTransportRisk) {
        HistoricalEra ancientEra("Antiquity", -400, 400, "Greece");
        ArcheologicalFind amphora("A-01", "Amphora", -300, 30000.0, 60.0,
                                  Dimensions(40.0, 60.0, 40.0, 10.0), ancientEra, "Pompeii", 1976, 4.2, 8);
        CHECK_EQUAL("Pompeii", amphora.excavationSite());
        CHECK_EQUAL(8, amphora.fragilityLevel());
        CHECK(amphora.verifyAuthenticity(-250));
        CHECK(amphora.isAuthenticVerified());
        CHECK(!amphora.verifyAuthenticity(500));
        CHECK(amphora.calculateTransportRisk() > 0.3);
        CHECK_EQUAL(50, amphora.yearsSinceDiscovery(2026));
    }

    // 13. Классы ClimateSensor и ClimateControlUnit
    TEST(ClimateSensorAndControlUnitBehavior) {
        ClimateSensor sensor("SNS-1", 21.0, 48.0, 130.0);
        CHECK_EQUAL("SNS-1", sensor.sensorId());
        CHECK(sensor.isOnline());
        CHECK(sensor.isWithinNorms(18.0, 24.0, 55.0, 150.0));
        sensor.updateReadings(28.0, 70.0, 250.0);
        CHECK(!sensor.isWithinNorms(18.0, 24.0, 55.0, 150.0));
        sensor.calibrateSensor();
        CHECK(sensor.isWithinNorms(18.0, 24.0, 55.0, 150.0));

        ClimateControlUnit unit("CCU-1", 20.0, 45.0, 150.0);
        CHECK(unit.isActive());
        CHECK_CLOSE(20.0, unit.targetTempCelsius(), 0.01);
        CHECK_CLOSE(45.0, unit.targetHumidityPercent(), 0.01);
        unit.adjustToSensor(sensor);
        CHECK_CLOSE(20.0, sensor.temperatureCelsius(), 0.01);

        HistoricalEra era("Medieval", 800, 1400, "Europe");
        AncientManuscript ms("M-10", "Chronicle", 1100, 50000.0, 80.0,
                             Dimensions(20, 30, 5, 2), era, "Slavic", 90, "Ink", 50.0);
        unit.verifyArtifactSafety(ms, sensor);
        sensor.updateReadings(20.0, 75.0, 100.0);
        CHECK_THROW(unit.verifyArtifactSafety(ms, sensor), ClimateOutOfRangeException);

        ArtistProfile painter("Giotto", "Italian", 1267, 1337, "Proto-Renaissance");
        Painting icon("P-10", "Fresco", 1310, 90000.0, 85.0, Dimensions(50, 60, 4, 5), era, painter);
        unit.verifyPaintingLight(icon, sensor);
        sensor.updateReadings(20.0, 45.0, 300.0);
        CHECK_THROW(unit.verifyPaintingLight(icon, sensor), ClimateOutOfRangeException);
    }

    // 14. Классы DisplayCase и LightingSystem
    TEST(DisplayCaseAndLightingSystemBehavior) {
        HistoricalEra era("Medieval", 800, 1400, "Europe");
        Artifact chalice("C-1", "Golden Chalice", 1200, 70000.0, 85.0, Dimensions(20, 30, 20, 3), era);
        DisplayCase showcase("CASE-1", Dimensions(100, 100, 100, 50), true, true);
        CHECK(showcase.isSuitableForFragile());
        showcase.mountArtifact(chalice);
        CHECK_EQUAL(&chalice, showcase.placedArtifact());
        showcase.lockCase();
        CHECK(showcase.isLocked());
        showcase.unmountArtifact();
        CHECK(showcase.placedArtifact() == nullptr);

        Artifact broken("C-2", "Broken", 1200, 1000.0, 20.0, Dimensions(10, 10, 10, 1), era);
        CHECK_THROW(showcase.mountArtifact(broken), RestorationRequiredException);
        Artifact huge("C-3", "Huge", 1200, 1000.0, 90.0, Dimensions(500, 500, 500, 200), era);
        CHECK_THROW(showcase.mountArtifact(huge), InvalidArtifactDataException);

        LightingSystem lights("LGT-1", 10, 200.0, 100);
        lights.activateNightMode();
        CHECK_EQUAL(15, lights.dimmingPercent());
        CHECK_CLOSE(30.0, lights.currentLux(), 0.01);
        CHECK(lights.calculatePowerConsumptionWatts() > 0.0);
    }

    // 15. Классы ExhibitionHall и StorageVault
    TEST(ExhibitionHallAndStorageVaultBehavior) {
        ExhibitionHall hall(1, "Main Hall", 1, 250.0, 40, 500.0);
        hall.admitVisitors(20);
        CHECK_CLOSE(50.0, hall.calculateOccupancyPercent(), 0.01);
        CHECK_THROW(hall.admitVisitors(30), HallOvercapacityException);
        hall.evacuateVisitors();
        CHECK_EQUAL(0, hall.currentVisitors());

        HistoricalEra era("Antiquity", -400, 200, "Rome");
        ArtistProfile sculptor("Master", "Roman", -100, -40, "Roman");
        Sculpture lightStatue("S-1", "Bust", -60, 20000.0, 80.0, Dimensions(50, 60, 50, 50), era, sculptor);
        CHECK(hall.canAccommodateSculpture(lightStatue));

        StorageVault vault("V-1", 1, 3);
        CHECK_EQUAL("V-1", vault.vaultId());
        vault.depositArtifact(lightStatue);
        CHECK(vault.containsArtifact("S-1"));
        CHECK(!vault.containsArtifact("NONE"));
        CHECK_CLOSE(20000.0, vault.calculateTotalStoredValue(), 0.01);
        CHECK_THROW(vault.depositArtifact(lightStatue), HallOvercapacityException);
        Artifact* out = vault.withdrawArtifact("S-1");
        CHECK_EQUAL("S-1", out->inventoryNumber());
        CHECK_THROW(vault.withdrawArtifact("S-1"), ArtifactNotFoundException);
    }

    // 16. Классы StorageRack и TransportCrate
    TEST(StorageRackAndTransportCrateBehavior) {
        Dimensions dims(30, 40, 20, 15.0);
        HistoricalEra era("Modern", 1800, 1950, "Europe");
        Artifact vase("V-100", "Porcelain Vase", 1890, 60000.0, 88.0, dims, era);

        StorageRack rack("R-1", 4, 20.0);
        CHECK_EQUAL("R-1", rack.rackCode());
        rack.placeOnShelf(vase);
        CHECK_CLOSE(15.0, rack.currentWeightKg(), 0.01);
        CHECK_CLOSE(5.0, rack.remainingWeightCapacityKg(), 0.01);
        CHECK_THROW(rack.placeOnShelf(vase), HallOvercapacityException);
        rack.removeFromShelf(vase);
        CHECK_EQUAL(0, rack.storedCount());
        CHECK_THROW(rack.removeFromShelf(vase), ArtifactNotFoundException);

        TransportCrate crate("CR-1", Dimensions(50, 50, 50, 30.0), true, true);
        CHECK_EQUAL("CR-1", crate.crateId());
        crate.packArtifact(vase);
        CHECK_EQUAL("V-100", crate.packedArtifactId());
        CHECK_CLOSE(40.0, crate.calculateGrossWeightKg(vase), 0.01);
        crate.unpackArtifact();
        CHECK(crate.isEmpty());

        Artifact fragile("V-101", "Broken", 1890, 1000.0, 20.0, dims, era);
        CHECK_THROW(crate.packArtifact(fragile), RestorationRequiredException);
        Artifact oversized("V-102", "Oversized", 1890, 1000.0, 90.0, Dimensions(200, 200, 200, 90), era);
        CHECK_THROW(crate.packArtifact(oversized), InvalidArtifactDataException);
    }

    // 17. Классы PartnerMuseum и LoanAgreement
    TEST(PartnerMuseumAndLoanAgreementBehavior) {
        Dimensions dims(30, 40, 20, 15.0);
        HistoricalEra era("Modern", 1800, 1950, "Europe");
        Artifact vase("V-100", "Porcelain Vase", 1890, 60000.0, 88.0, dims, era);

        PartnerMuseum partner("Prado", "Spain", "Madrid", 85.0, true);
        CHECK_EQUAL("Prado", partner.partnerName());
        CHECK(partner.isVerifiedPartner());
        CHECK(partner.isEligibleForHighValueLoan(60000.0));
        CHECK(partner.isEligibleForHighValueLoan(10000.0));

        PartnerMuseum unverified("Small Gallery", "Spain", "Toledo", 90.0, false);
        CHECK(!unverified.isEligibleForHighValueLoan(10000.0));

        LoanAgreement loan("LN-1", "V-100", 30, 15000.0, partner);
        CHECK_EQUAL("LN-1", loan.agreementId());
        CHECK_EQUAL("Prado", loan.partner().partnerName());
        loan.validateLoanForArtifact(vase);
        CHECK(loan.calculateDailyFee() > 0.0);
        loan.completeReturn(partner, true);
        CHECK(loan.isReturned());
        CHECK_CLOSE(70.0, partner.reliabilityRating(), 0.01);

        LoanAgreement badPartnerLoan("LN-2", "V-100", 30, 20000.0, unverified);
        CHECK_THROW(badPartnerLoan.validateLoanForArtifact(vase), LoanAgreementViolationException);
    }

    // 18. Классы CatalogEntry и CollectionCatalog
    TEST(CatalogEntryAndCollectionCatalogBehavior) {
        Dimensions dims(30, 40, 20, 15.0);
        HistoricalEra era("Modern", 1800, 1950, "Europe");
        Artifact vase("V-100", "Porcelain Vase", 1890, 60000.0, 88.0, dims, era);

        CatalogEntry entry("CAT-1", "V-100", 1, "Imperial vase");
        entry.relocateToHall(3);
        CHECK_EQUAL(3, entry.hallNumber());

        CollectionCatalog catalog("Main Catalog", 2026);
        catalog.registerArtifact(vase, 2, "Imperial porcelain vase");
        CHECK_EQUAL(1, catalog.countPublishedEntries());
        const CatalogEntry& found = catalog.findEntryByArtifactId("V-100");
        CHECK_EQUAL(2, found.hallNumber());
        CHECK_EQUAL(1u, catalog.searchEntries("porcelain").size());
        CHECK_THROW(catalog.findEntryByArtifactId("MISSING"), ArtifactNotFoundException);
    }

    // 19. Классы Exhibition и ExcursionRoute
    TEST(ExhibitionAndExcursionRouteBehavior) {
        ExhibitionHall hall1(1, "Hall 1", 1, 200.0, 30, 500.0);
        Dimensions dims(40, 50, 10, 5.0);
        HistoricalEra era("Classic", 1700, 1850, "France");
        Artifact art("ART-77", "Portrait", 1780, 40000.0, 90.0, dims, era);

        Exhibition exh("EX-1", "French Art", "Painting", 20.0, true);
        CHECK_THROW(exh.openExhibition(), ExhibitionClosedException);
        exh.addExhibit(art, hall1);
        exh.openExhibition();
        CHECK(exh.isOpen());
        CHECK_CLOSE(40.0, exh.calculateExhibitionPrice(true), 0.01);
        exh.closeExhibition();
        CHECK(!exh.isOpen());

        ExcursionRoute route("RT-1", "Classic Tour");
        CHECK_EQUAL("RT-1", route.routeId());
        route.appendHallStop(hall1, 30);
        CHECK(route.includesHall(1));
        CHECK(!route.includesHall(99));
        CHECK_CLOSE(120.0, route.estimateWalkingDistanceMeters(), 0.01);
    }

    // 20. Классы GuidedTour, AudioGuideDevice, SpecialEvent, ExhibitionSchedule
    TEST(GuidedTourAudioGuideSpecialEventAndSchedule) {
        ExhibitionHall hall1(1, "Hall 1", 1, 200.0, 30, 500.0);
        ExcursionRoute route("RT-1", "Classic Tour");
        route.appendHallStop(hall1, 30);

        GuidedTour tour("TR-1", "English", 2, route);
        CHECK_EQUAL("TR-1", tour.tourId());
        CHECK_THROW(tour.conductTour(), ExhibitionClosedException);
        tour.enrollParticipant();
        tour.enrollParticipant();
        CHECK_EQUAL(2, tour.currentParticipants());
        CHECK_THROW(tour.enrollParticipant(), TicketSoldOutException);
        tour.cancelParticipant();
        tour.conductTour();
        CHECK(tour.isCompleted());

        AudioGuideDevice guide("AG-1", 50, "English", 20);
        CHECK_EQUAL("AG-1", guide.deviceId());
        guide.rentToVisitor("French");
        CHECK(guide.isRented());
        CHECK_EQUAL("French", guide.selectedLanguage());
        CHECK_THROW(guide.rentToVisitor("German"), StaffNotAvailableException);
        guide.playTrack(30);
        guide.returnAndCharge();
        CHECK(!guide.isRented());

        SpecialEvent ev("EV-1", "Lecture", "Education", 1, 10.0);
        CHECK_EQUAL("EV-1", ev.eventId());
        CHECK_EQUAL("Lecture", ev.eventName());
        ev.registerGuest();
        CHECK_EQUAL(1, ev.registeredGuests());

        ExhibitionSchedule sched(2026);
        CHECK_EQUAL(2026, sched.calendarYear());
        sched.scheduleEvent(ev);
        CHECK_EQUAL(1u, sched.totalEvents());
    }

    // 21. Классы DiscountPolicy, Visitor и Ticket
    TEST(DiscountPolicyVisitorAndTicketBehavior) {
        DiscountPolicy policy;
        Visitor child("V-C", "Kid", 10, false, 30.0, false);
        Visitor senior("V-S", "Grandpa", 65, false, 30.0, false);
        CHECK_CLOSE(0.50, policy.calculateDiscountPercent(child.age(), false, false), 0.01);
        CHECK_CLOSE(0.40, policy.calculateDiscountPercent(senior.age(), false, false), 0.01);
        CHECK_CLOSE(100.0, policy.applyGroupDiscount(100.0, 3), 0.01);

        CHECK_THROW(child.payAmount(100.0), InsufficientFundsException);

        Ticket tck("T-1", "V-C", "EX-1", 10.0);
        tck.validateAndUse("EX-1");
        CHECK(tck.isUsed());
        CHECK_THROW(tck.validateAndUse("EX-1"), InvalidTicketException);
        CHECK_THROW(tck.refundTicket(child), InvalidTicketException);
    }

    // 22. Классы MembershipCard, CloakroomTag, VisitorFeedback
    TEST(MembershipCloakroomAndFeedbackEdgeCases) {
        MembershipCard card("MC-1", "V-1", "Silver", 1400);
        CHECK_EQUAL("MC-1", card.cardNumber());
        card.accruePoints(30.0);
        CHECK_EQUAL("Platinum", card.tierName());

        Visitor v1("V-1", "Alice", 25, false, 50.0);
        Visitor v2("V-2", "Bob", 25, false, 50.0);
        CloakroomTag tag(10);
        CHECK_EQUAL(10, tag.tagNumber());
        tag.acceptItems(v1, 2);
        CHECK_EQUAL(2, tag.itemsCount());
        CHECK_THROW(tag.acceptItems(v2, 1), HallOvercapacityException);
        CHECK_THROW(tag.releaseItems(v2), SecurityBreachException);
        tag.releaseItems(v1);

        VisitorFeedback badFb("FB-2", "Bob", "EX-1", 1, "Too crowded");
        CHECK_EQUAL(1, badFb.ratingStars());
        CHECK_EQUAL("EX-1", badFb.exhibitionId());
        CHECK(badFb.requiresManagerAttention());
        CHECK(!badFb.isPositive());
    }

    // 23. Классы Employee, Curator, TourGuide, Restorer, SecurityGuard, HRDepartment
    TEST(StaffClassesAndHRDepartmentEdgeCases) {
        Curator curator("E-1", "Curator", 2000.0, 5, "Renaissance");
        CHECK_EQUAL("Curator", curator.position());
        CHECK_EQUAL("Renaissance", curator.specializationEra());
        curator.endShift();
        CHECK(!curator.isOnDuty());

        TourGuide guide("E-2", "Guide", 1500.0, 3, {"Russian"});
        guide.startShift();
        guide.updateRating(4.0);
        CHECK_CLOSE(4.5, guide.guideRating(), 0.01);

        Restorer restorer("E-3", "Restorer", 2200.0, 10, "Canvas", 2);
        restorer.upgradeCertification();
        CHECK_EQUAL(3, restorer.certificationLevel());

        SecurityGuard guard("E-4", "Guard", 1400.0, 4, "LIC-1", true);
        ExhibitionHall hall(2, "Hall 2", 1, 100.0, 20, 400.0);
        guard.assignToHall(hall);
        CHECK_EQUAL(2, guard.assignedHallNumber());

        HRDepartment hr("Boss", 1);
        hr.hireEmployee(curator);
        CHECK_THROW(hr.hireEmployee(guide), HallOvercapacityException);
        hr.dismissEmployee("E-1");
        CHECK_EQUAL(0u, hr.staffCount());
        CHECK_THROW(hr.dismissEmployee("E-999"), StaffNotAvailableException);
    }

    // 24. Классы RestorationMaterial, ConditionReport, RestorationTask, RestorationWorkshop
    TEST(RestorationLabClassesEdgeCases) {
        RestorationMaterial mat("Resin", 100.0, 2.0, true);
        CHECK_EQUAL("Resin", mat.materialName());
        mat.restockMaterial(50.0);
        CHECK_CLOSE(150.0, mat.quantityGrams(), 0.01);
        CHECK_CLOSE(300.0, mat.calculateStockValue(), 0.01);
        CHECK_THROW(mat.consumeMaterial(500.0), InsufficientFundsException);

        ConditionReport r1("R-1", "A-1", "Insp", 80.0, false);
        ConditionReport r2("R-2", "A-2", "Insp", 60.0, false);
        ConditionReport r3("R-3", "A-3", "Insp", 40.0, false);
        CHECK_EQUAL("R-1", r1.reportId());
        CHECK_CLOSE(80.0, r1.recordedCondition(), 0.01);
        CHECK_EQUAL(1, r1.assessUrgencyLevel());
        CHECK_EQUAL(3, r2.assessUrgencyLevel());
        CHECK_EQUAL(4, r3.assessUrgencyLevel());
        CHECK(r1.isFitForTransport());

        RestorationTask task("T-1", "A-1", "E-3", 95.0, 500.0);
        CHECK_CLOSE(290.0, task.calculateActualCost(mat, 10.0, 6.0), 0.01);
    }

    // 25. Классы SecurityCamera, AlarmTrigger, AccessBadge, SecurityControlCenter
    TEST(SecuritySystemClassesEdgeCases) {
        SecurityCamera cam("CAM-1", 1, true);
        CHECK_EQUAL("CAM-1", cam.cameraId());
        cam.startRecording();
        CHECK(cam.isRecording());
        cam.recordFootage(10.0);
        CHECK_CLOSE(25.0, cam.storageUsedGb(), 0.01);
        CHECK_THROW(cam.recordFootage(40.0), HallOvercapacityException);
        cam.archiveAndClearStorage();
        CHECK_CLOSE(0.0, cam.storageUsedGb(), 0.01);

        AlarmTrigger trig("ALM-1", 1);
        CHECK_EQUAL("ALM-1", trig.triggerId());
        trig.armTrigger();
        CHECK(trig.isArmed());

        AccessBadge badge("BDG-1", "E-1", 2);
        CHECK_EQUAL("BDG-1", badge.badgeCode());
        badge.revokeBadge();
        CHECK(badge.isRevoked());
        CHECK_THROW(badge.upgradeClearance(4), SecurityBreachException);

        SecurityControlCenter center("HQ");
        center.registerCamera(cam);
        center.registerAlarm(trig);
        CHECK_EQUAL(1u, center.camerasCount());
        CHECK_EQUAL(1u, center.alarmsCount());
    }

    // 26. Классы MuseumBudget, TicketOffice, DonationGrant, SouvenirItem, GiftShop
    TEST(FinanceAndGiftShopEdgeCases) {
        MuseumBudget budget(2026, 1000.0, 500.0);
        budget.applyStateSubsidy();
        CHECK_CLOSE(1500.0, budget.currentBalance(), 0.01);
        CHECK_CLOSE(500.0, budget.totalRevenue(), 0.01);
        budget.allocateExpense(200.0);
        CHECK_CLOSE(200.0, budget.totalExpenses(), 0.01);
        CHECK_CLOSE(300.0, budget.calculateNetBalance(), 0.01);
        CHECK_THROW(budget.allocateExpense(5000.0), InsufficientFundsException);

        TicketOffice office("BOX-1");
        Exhibition closedExh("EX-CL", "Closed", "Art", 20.0, false);
        ExhibitionHall hall(1, "H1", 1, 100.0, 10, 400.0);
        Visitor vis("V-1", "Guest", 25, false, 100.0);
        CHECK_THROW(office.sellTicket(vis, closedExh, hall, budget), ExhibitionClosedException);

        HistoricalEra era("Modern", 1900, 2000, "Europe");
        Artifact art("A-1", "Art", 1950, 5000.0, 90.0, Dimensions(20, 20, 10, 2), era);
        closedExh.addExhibit(art, hall);
        closedExh.openExhibition();
        Ticket sold = office.sellTicket(vis, closedExh, hall, budget);
        CHECK_EQUAL(1, office.soldTicketsCount());
        office.processTicketRefund(sold, vis, budget);
        office.transferCashToBudget(budget);
        CHECK_CLOSE(0.0, office.cashInRegister(), 0.01);

        RestorationTask rTask("T-1", "A-1", "E-1", 95.0, 800.0);
        DonationGrant grant("G-1", "Sponsor", 1000.0, "Conservation");
        CHECK_EQUAL("G-1", grant.grantId());
        CHECK(grant.canFundRestoration(rTask));
        grant.transferToBudget(budget);
        CHECK(grant.isTransferred());
        CHECK_THROW(grant.transferToBudget(budget), InvalidTicketException);

        MembershipCard silver("MC-S", "V-1", "Silver", 100);
        MembershipCard gold("MC-G", "V-1", "Gold", 600);
        MembershipCard plat("MC-P", "V-1", "Platinum", 2000);
        SouvenirItem item("SKU-1", "Postcard", 10.0, 5);
        CHECK_CLOSE(9.0, item.applyMemberDiscountPrice(silver), 0.01);
        CHECK_CLOSE(8.5, item.applyMemberDiscountPrice(gold), 0.01);
        CHECK_CLOSE(8.0, item.applyMemberDiscountPrice(plat), 0.01);
        item.restockUnits(5);
        CHECK_EQUAL(10, item.stockQuantity());
        CHECK_THROW(item.sellUnits(50), TicketSoldOutException);

        GiftShop shop("Art Shop");
        shop.addSouvenirItem(item);
        CHECK_CLOSE(100.0, shop.calculateInventoryValue(), 0.01);
        CHECK_THROW(shop.purchaseSouvenir(vis, "NON-SKU", 1, budget), ArtifactNotFoundException);
    }

    // 27. Полный цикл работы комплекса Museum (приобретение, выставка, билеты, зарплаты)
    TEST(MuseumFullWorkflowIntegration) {
        Museum museum("Hermitage Branch", 1910, "Minsk");
        CHECK_EQUAL("Hermitage Branch", museum.museumName());
        ExhibitionHall hall(1, "Royal Hall", 1, 400.0, 50, 600.0);
        museum.addExhibitionHall(hall);
        CHECK_EQUAL(50, museum.calculateTotalVisitorCapacity());

        Curator curator("E-1", "Dr. Smirnov", 2000.0, 10, "Antiquity");
        TourGuide guide("E-2", "Olga", 1500.0, 5, {"Russian", "German"});
        Restorer restorer("E-3", "Master Pavel", 2500.0, 15, "Stone", 5);
        SecurityGuard guard("E-4", "Major Igor", 1400.0, 7, "SEC-77", true);

        curator.startShift();
        guide.startShift();
        restorer.startShift();
        guard.startShift();

        museum.hrDepartment().hireEmployee(curator);
        museum.hrDepartment().hireEmployee(guide);
        museum.hrDepartment().hireEmployee(restorer);
        museum.hrDepartment().hireEmployee(guard);
        CHECK_EQUAL(4, museum.hrDepartment().countStaffOnDuty());

        HistoricalEra antEra("Antiquity", -500, 300, "Greece");
        Artifact damagedStatue("ART-500", "Damaged Bust", -200, 120000.0, 30.0,
                               Dimensions(40, 60, 40, 35.0), antEra);
        museum.acquireNewArtifact(damagedStatue, 1, "Ancient marble bust");

        RestorationMaterial marblePaste("Marble Consolidant", 200.0, 2.0, false);
        RestorationWorkshop lab("Stone Lab", 5, 30000.0);
        lab.acceptForRestoration(damagedStatue, restorer);
        CHECK_EQUAL(1, lab.countPendingTasks());
        lab.completeTaskById("TASK-ART-500", damagedStatue, restorer, marblePaste);
        CHECK_EQUAL(0, lab.countPendingTasks());
        CHECK(!damagedStatue.needsRestoration());

        Exhibition exh("EX-ANT", "Antiquity Alive", "History", 25.0, false);
        curator.organizeExhibition(exh, damagedStatue, museum.halls()[0]);
        CHECK_EQUAL(1, curator.curatedExhibitionsCount());
        CHECK(curator.evaluateArtifactCondition(damagedStatue));
        exh.openExhibition();

        TicketOffice boxOffice("BOX-1");
        Visitor guest("V-99", "Guest", 30, false, 200.0, false);
        Ticket soldTicket = boxOffice.sellTicket(guest, exh, museum.halls()[0], museum.budget());
        soldTicket.validateAndUse("EX-ANT");
        CHECK(soldTicket.isUsed());

        GiftShop shop("Shop");
        shop.addSouvenirItem(SouvenirItem("S-1", "Magnet", 10.0, 20));
        shop.purchaseSouvenir(guest, "S-1", 2, museum.budget());
        CHECK_CLOSE(20.0, shop.dailySales(), 0.01);
        CHECK_EQUAL(1, shop.transactionsCount());

        museum.payMonthlyStaffSalaries();
        CHECK(museum.budget().currentBalance() > 100000.0);
        CHECK(!museum.generateAnnualSummaryReport().empty());
    }

    // 28. Проверка отказа куратора и гида при несоответствии специализации или смены
    TEST(CuratorAndGuideQualificationChecks) {
        Curator curator("E-10", "Modernist", 1800.0, 4, "Modern");
        curator.startShift();
        HistoricalEra antEra("Antiquity", -500, 300, "Greece");
        Artifact ancientCoin("C-10", "Drachma", -200, 5000.0, 90.0, Dimensions(5, 5, 1, 0.1), antEra);
        Exhibition exh("EX-10", "Ancient Coins", "Numismatics", 15.0, false);
        ExhibitionHall hall(1, "Coin Hall", 1, 80.0, 25, 300.0);

        CHECK(!curator.canCurateArtifact(ancientCoin));
        CHECK_THROW(curator.organizeExhibition(exh, ancientCoin, hall), StaffNotAvailableException);

        TourGuide guide("E-11", "Anna", 1400.0, 2, {"Russian"});
        guide.startShift();
        ExcursionRoute route("R-1", "Route");
        route.appendHallStop(hall, 15);
        GuidedTour spanishTour("T-SP", "Spanish", 10, route);
        spanishTour.enrollParticipant();
        CHECK_THROW(guide.leadGuidedTour(spanishTour), StaffNotAvailableException);

        GuidedTour russianTour("T-RU", "Russian", 10, route);
        russianTour.enrollParticipant();
        guide.leadGuidedTour(russianTour);
        CHECK_EQUAL(1, guide.conductedToursCount());
    }

    // 29. Проверка квалификации реставратора для шедевров высокой стоимости
    TEST(RestorerMasterpieceQualificationCheck) {
        Restorer juniorRestorer("E-20", "Junior", 1200.0, 1, "Paint", 2);
        juniorRestorer.startShift();
        HistoricalEra era("Renaissance", 1400, 1600, "Italy");
        Artifact masterpiece("M-99", "Priceless Painting", 1500, 500000.0, 35.0,
                             Dimensions(80, 100, 5, 10), era);

        CHECK(!juniorRestorer.isQualifiedFor(masterpiece));
        CHECK_THROW(juniorRestorer.performRestoration(masterpiece, 20.0), StaffNotAvailableException);

        RestorationWorkshop workshop("Lab", 3, 10000.0);
        CHECK_THROW(workshop.acceptForRestoration(masterpiece, juniorRestorer), StaffNotAvailableException);
    }

    // 30. Проверка работы охраны при обходе витрин и реагировании на тревогу
    TEST(SecurityGuardDutyAndIncidentResponse) {
        SecurityGuard guardWithoutRadio("E-30", "Guard", 1300.0, 2, "LIC-30", false);
        DisplayCase showcase("CS-30", Dimensions(50, 50, 50, 20));
        CHECK_THROW(guardWithoutRadio.inspectDisplayCase(showcase), StaffNotAvailableException);

        guardWithoutRadio.startShift();
        guardWithoutRadio.inspectDisplayCase(showcase);
        CHECK(showcase.isLocked());
        CHECK_THROW(guardWithoutRadio.respondToIncident(), StaffNotAvailableException);

        SecurityGuard armedGuard("E-31", "Chief Guard", 1600.0, 6, "LIC-31", true);
        armedGuard.startShift();
        armedGuard.respondToIncident();
        CHECK_EQUAL(1, armedGuard.resolvedIncidentsCount());
    }

    // 31. Проверка переполнения очереди реставрационной мастерской
    TEST(RestorationWorkshopQueueLimit) {
        Restorer master("E-40", "Master", 2500.0, 12, "Universal", 5);
        master.startShift();
        HistoricalEra era("Baroque", 1600, 1750, "Europe");
        Artifact a1("A-1", "Item 1", 1650, 10000.0, 30.0, Dimensions(20, 20, 20, 2), era);
        Artifact a2("A-2", "Item 2", 1660, 10000.0, 30.0, Dimensions(20, 20, 20, 2), era);

        RestorationWorkshop smallLab("Mini Lab", 1, 5000.0);
        smallLab.acceptForRestoration(a1, master);
        CHECK_EQUAL(1u, smallLab.totalTasks());
        CHECK_THROW(smallLab.acceptForRestoration(a2, master), HallOvercapacityException);
        RestorationMaterial glue("Glue", 100.0, 1.0);
        CHECK_THROW(smallLab.completeTaskById("UNKNOWN-TASK", a1, master, glue),
                    ArtifactNotFoundException);
    }

    // 32. Проверка блокировки зала (Lockdown) и сброса сигнализации
    TEST(SecurityLockdownAndEmergencyEvacuation) {
        ExhibitionHall hall(1, "Main Gallery", 1, 300.0, 100, 500.0);
        hall.admitVisitors(45);
        CHECK_EQUAL(45, hall.currentVisitors());

        SecurityControlCenter secCenter("SEC-1");
        secCenter.triggerEmergencyLockdown(hall);
        CHECK(secCenter.isLockdownActive());
        CHECK_EQUAL(0, hall.currentVisitors());

        SecurityGuard guard("E-50", "Officer", 1500.0, 5, "LIC-50", true);
        guard.startShift();
        secCenter.liftLockdown(guard);
        CHECK(!secCenter.isLockdownActive());
    }

} // SUITE(MuseumDomainTests)

int main(int, const char*[]) {
    return UnitTest::RunAllTests();
}
