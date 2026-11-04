/**
 * @file main.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Интерактивный консольный интерфейс (CLI/меню) для системы управления Музеем.
 * @details Демонстрирует содержательную бизнес-логику предметной области «Музей»:
 * приём экспонатов в коллекцию, организацию выставок, продажу билетов со скидками,
 * проведение экскурсий, реставрацию в мастерской, контроль микроклимата и безопасности,
 * работу сувенирной лавки и управление бюджетом/зарплатами.
 */

#include "FinanceAndShop.h"

#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace {

void printMainMenu(const Museum& museum, const Exhibition& mainExhibition) {
    std::cout << "\n======================================================================\n";
    std::cout << "          Лабораторная работа №2: Информационная система «МУЗЕЙ»\n";
    std::cout << "======================================================================\n";
    std::cout << museum.generateAnnualSummaryReport() << "\n";
    std::cout << "Активная выставка: «" << mainExhibition.title() << "» ("
              << (mainExhibition.isOpen() ? "ОТКРЫТА" : "ЗАКРЫТА")
              << ", экспонатов: " << mainExhibition.exhibitsCount() << ")\n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "  1. Просмотреть каталог экспонатов и залы музея\n";
    std::cout << "  2. Приобрести новый экспонат в коллекцию музея\n";
    std::cout << "  3. Открыть выставку и разместить экспонат в витрине\n";
    std::cout << "  4. Продать билет посетителю через кассу (с учётом льгот)\n";
    std::cout << "  5. Провести групповую экскурсию с гидом и выдать аудиогид\n";
    std::cout << "  6. Отправить повреждённый экспонат в реставрационную мастерскую\n";
    std::cout << "  7. Проверить микроклимат зала и систему безопасности\n";
    std::cout << "  8. Продать сувенир в магазине и принять грант от мецената\n";
    std::cout << "  9. Выплатить ежемесячную зарплату сотрудникам музея\n";
    std::cout << "  0. Выход\n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "Выберите действие: ";
}

std::string promptLine(const std::string& label) {
    std::cout << label;
    std::string value;
    std::getline(std::cin >> std::ws, value);
    return value;
}

void showCatalogAndHalls(Museum& museum) {
    std::cout << "\n--- Выставочные залы музея ---\n";
    for (const ExhibitionHall& hall : museum.halls()) {
        std::cout << "Зал #" << hall.hallNumber() << " «" << hall.hallName()
                  << "» | Посетителей: " << hall.currentVisitors() << "/" << hall.maxVisitors()
                  << " (" << hall.calculateOccupancyPercent() << "%)\n";
    }
    std::cout << "\n--- Главный каталог коллекции (" << museum.catalog().totalEntries() << ") ---\n";
    for (const CatalogEntry& entry : museum.catalog().entries()) {
        std::cout << "[" << entry.entryCode() << "] Инв.№: " << entry.artifactId()
                  << " | Зал #" << entry.hallNumber()
                  << " | Онлайн: " << (entry.isPublishedOnline() ? "Да" : "Нет") << "\n";
    }
}

void acquireInteractiveArtifact(Museum& museum, std::vector<std::unique_ptr<Artifact>>& ownedArtifacts) {
    std::string inv = promptLine("Введите инвентарный номер (например, ART-105): ");
    std::string title = promptLine("Введите название экспоната: ");
    std::cout << "Введите оценочную стоимость ($): ";
    double value = 10000.0;
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Dimensions dims(60.0, 80.0, 15.0, 8.0);
    HistoricalEra era("Renaissance", 1400, 1600, "Europe");
    auto item = std::make_unique<Artifact>(inv, title, 1510, value, 85.0, dims, era);
    museum.acquireNewArtifact(*item, 1, "Музейный предмет: " + title);
    ownedArtifacts.push_back(std::move(item));
    std::cout << "Экспонат успешно принят в фондохранилище и зарегистрирован в каталоге!\n";
}

} // namespace

int main() {
    Museum museum("Национальный художественно-исторический музей", 1939, "Минск");
    museum.addExhibitionHall(ExhibitionHall(1, "Зал Античности и Ренессанса", 1, 350.0, 60, 800.0));
    museum.addExhibitionHall(ExhibitionHall(2, "Галерея живописи XIX века", 2, 280.0, 45, 500.0));

    Curator chiefCurator("EMP-01", "Иван Соколов", 2200.0, 12, "Renaissance");
    chiefCurator.startShift();
    TourGuide seniorGuide("EMP-02", "Анна Морозова", 1600.0, 6, {"Russian", "English"});
    seniorGuide.startShift();
    Restorer masterRestorer("EMP-03", "Пётр Ковалёв", 2400.0, 15, "Oil & Wood", 5);
    masterRestorer.startShift();
    SecurityGuard chiefGuard("EMP-04", "Дмитрий Громов", 1500.0, 8, "LIC-9981", true);
    chiefGuard.startShift();

    museum.hrDepartment().hireEmployee(chiefCurator);
    museum.hrDepartment().hireEmployee(seniorGuide);
    museum.hrDepartment().hireEmployee(masterRestorer);
    museum.hrDepartment().hireEmployee(chiefGuard);

    std::vector<std::unique_ptr<Artifact>> ownedArtifacts;
    HistoricalEra renEra("Renaissance", 1400, 1600, "Italy");
    ArtistProfile daVinci("Leonardo School", "Italian", 1452, 1519, "High Renaissance");
    ownedArtifacts.push_back(std::make_unique<Painting>(
        "ART-001", "Мадонна с цветком", 1495, 250000.0, 92.0,
        Dimensions(70.0, 90.0, 5.0, 6.0), renEra, daVinci));
    ownedArtifacts.push_back(std::make_unique<ArcheologicalFind>(
        "ART-002", "Античная амфора", -320, 45000.0, 32.0,
        Dimensions(40.0, 65.0, 40.0, 12.0), HistoricalEra("Antiquity", -500, 400, "Greece"),
        "Херсонес", 1988, 3.5, 8));

    museum.acquireNewArtifact(*ownedArtifacts[0], 1, "Шедевр эпохи Возрождения");
    museum.acquireNewArtifact(*ownedArtifacts[1], 1, "Краснофигурная керамика IV в. до н.э.");

    Exhibition mainExhibition("EXH-MAIN", "Сокровища мирового искусства", "Искусство", 30.0, true);
    chiefCurator.organizeExhibition(mainExhibition, *ownedArtifacts[0], museum.halls()[0]);
    mainExhibition.openExhibition();

    TicketOffice ticketOffice("BOX-MAIN");
    GiftShop giftShop("Музейная лавка");
    giftShop.addSouvenirItem(SouvenirItem("SKU-01", "Альбом репродукций", 25.0, 40, "EXH-MAIN"));

    int choice = -1;
    while (true) {
        printMainMenu(museum, mainExhibition);
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Ошибка] Введите номер пункта от 0 до 9.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 0) {
            std::cout << "Завершение работы информационной системы музея.\n";
            break;
        }

        try {
            switch (choice) {
                case 1:
                    showCatalogAndHalls(museum);
                    break;
                case 2:
                    acquireInteractiveArtifact(museum, ownedArtifacts);
                    break;
                case 3: {
                    DisplayCase showcase("CASE-101", Dimensions(120.0, 150.0, 80.0, 50.0));
                    showcase.mountArtifact(*ownedArtifacts[0]);
                    chiefGuard.inspectDisplayCase(showcase);
                    mainExhibition.openExhibition();
                    std::cout << "Экспонат «" << ownedArtifacts[0]->title()
                              << "» установлен в герметичную витрину " << showcase.caseId()
                              << ", витрина заперта охраной.\n";
                    break;
                }
                case 4: {
                    Visitor studentVisitor("VIS-10", "Алексей Петров", 20, true, 100.0, true);
                    Ticket ticket = ticketOffice.sellTicket(
                        studentVisitor, mainExhibition, museum.halls()[0], museum.budget());
                    ticket.validateAndUse(mainExhibition.exhibitionId());
                    std::cout << "Продан льготный билет " << ticket.ticketNumber()
                              << " по цене $" << ticket.finalPrice()
                              << ". Остаток у посетителя: $" << studentVisitor.walletBalance() << "\n";
                    break;
                }
                case 5: {
                    ExcursionRoute route("RT-10", "Обзорный маршрут");
                    route.appendHallStop(museum.halls()[0], 25);
                    route.appendHallStop(museum.halls()[1], 20);
                    GuidedTour tour("TOUR-1", "Russian", 15, route);
                    tour.enrollParticipant();
                    seniorGuide.leadGuidedTour(tour);
                    AudioGuideDevice audioGuide("AG-01", 90, "Russian", 40);
                    audioGuide.rentToVisitor("Russian");
                    audioGuide.playTrack(20);
                    std::cout << "Экскурсия проведена гидом " << seniorGuide.fullName()
                              << " (длительность: " << route.durationMinutes()
                              << " мин). Выдан аудиогид (заряд: " << audioGuide.batteryPercent() << "%).\n";
                    break;
                }
                case 6: {
                    Artifact& damaged = *ownedArtifacts[1];
                    std::cout << "Состояние «" << damaged.title() << "» до реставрации: "
                              << damaged.conditionPercent() << "%\n";
                    RestorationWorkshop workshop("Лаборатория керамики", 5, 20000.0);
                    RestorationMaterial glue("Акриловый консолидант", 300.0, 1.5, false);
                    workshop.acceptForRestoration(damaged, masterRestorer);
                    workshop.completeTaskById("TASK-" + damaged.inventoryNumber(),
                                              damaged, masterRestorer, glue);
                    std::cout << "Реставрация завершена! Новое состояние: "
                              << damaged.conditionPercent() << "%\n";
                    break;
                }
                case 7: {
                    ClimateSensor sensor("SNS-HALL1", 21.0, 46.0, 130.0);
                    ClimateControlUnit climateUnit("CCU-HALL1", 20.0, 45.0, 150.0);
                    climateUnit.adjustToSensor(sensor);
                    SecurityCamera cam("CAM-HALL1", 1, true);
                    cam.recordFootage(4.0);
                    museum.securityCenter().registerCamera(cam);
                    std::cout << "Климат в норме: T=" << sensor.temperatureCelsius()
                              << "C, H=" << sensor.humidityPercent()
                              << "%, Свет=" << sensor.lightLux()
                              << " лк. Камера записала " << cam.storageUsedGb() << " ГБ архива.\n";
                    break;
                }
                case 8: {
                    Visitor buyer("VIS-22", "Мария Иванова", 34, false, 200.0, true);
                    double spent = giftShop.purchaseSouvenir(buyer, "SKU-01", 2, museum.budget());
                    DonationGrant grant("GR-2026", "Фонд Культурного Наследия", 15000.0, "Реставрация");
                    grant.transferToBudget(museum.budget());
                    std::cout << "Продано 2 альбома в сувенирной лавке на $" << spent
                              << ". Зачислен меценатский грант $" << grant.amount() << ".\n";
                    break;
                }
                case 9: {
                    double payroll = museum.hrDepartment().calculateTotalPayroll();
                    museum.payMonthlyStaffSalaries();
                    std::cout << "Выплачена зарплата всему штату (" << museum.hrDepartment().staffCount()
                              << " сотр.) на сумму $" << payroll
                              << ". Текущий бюджет музея: $" << museum.budget().currentBalance() << "\n";
                    break;
                }
                default:
                    std::cout << "Неизвестная команда.\n";
                    break;
            }
        } catch (const MuseumException& ex) {
            std::cout << "[Музейное исключение] " << ex.formatErrorReport() << "\n";
        } catch (const std::exception& ex) {
            std::cout << "[Ошибка] " << ex.what() << "\n";
        }
    }
    return 0;
}
