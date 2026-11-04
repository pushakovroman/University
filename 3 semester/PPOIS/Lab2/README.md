# Лабораторная работа №2. Крупный ООП-проект (Предметная область: «Музей»)

Реализация объектно-ориентированной модели предметной области **«Музей» (Museum)** на языке **C++ (C++17)** в рамках курса «Проектирование программного обеспечения интеллектуальных систем» (ППОИС).

## Архитектура и подсистемы проекта

Объектная модель разделена на 10 логических модулей (`include/*.h`, `src/*.cpp`), охватывающих все процессы работы современного музея:
1. **Экспонаты и коллекция (`Artifacts`)**: габариты, исторические эпохи, профили авторов, базовый экспонат и специализированные виды (картины, скульптуры, древние рукописи, археологические находки).
2. **Выставочные залы и микроклимат (`HallsAndClimate`)**: датчики температуры/влажности/освещённости, климатические установки, защищённые витрины, освещение, выставочные залы и фондохранилище.
3. **Складская логистика, межмузейный обмен и каталог (`StorageAndLogistics`)**: стеллажи, транспортировочные кофры, музеи-партнёры, договоры межмузейного займа/аренды, карточки и главный каталог коллекции.
4. **Выставки, экскурсии и мероприятия (`ExhibitionsAndEvents`)**: постоянные и временные выставки, экскурсионные маршруты, групповые туры, аудиогиды, спецмероприятия («Ночь музеев», лекции) и календарное расписание.
5. **Посетители и билетная система (`VisitorsAndTickets`)**: льготная политика скидок, посетители, билеты, клубные карты лояльности «Друг музея», гардероб и книга отзывов.
6. **Персонал и отдел кадров (`StaffAndHR`)**: штатные сотрудники, научные кураторы, экскурсоводы, художники-реставраторы, служба безопасности и отдел кадров.
7. **Реставрационная мастерская (`RestorationLab`)**: расходные материалы, акты осмотра сохранности, наряды-задания и реставрационная лаборатория.
8. **Система безопасности (`SecuritySystem`)**: камеры видеонаблюдения, датчики сигнализации, электронные пропуска с уровнями допуска и центральный пульт охраны.
9. **Финансы, касса, сувенирная лавка и комплекс Музея (`FinanceAndShop`)**: бюджет музея, билетная касса, гранты меценатов, сувениры, магазин и главный фасадный класс `Museum`.
10. **Исключения предметной области (`MuseumExceptions`)**: 13 специализированных классов исключений.

---

## Описание классов, полей, методов и ассоциаций

Формат строки: `Класс | число полей | число содержательных методов | связанные классы`.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|---|---|---|
| Dimensions | 4 | 3 | Dimensions |
| HistoricalEra | 4 | 3 | — |
| ArtistProfile | 5 | 3 | HistoricalEra |
| Artifact | 8 | 4 | Dimensions, HistoricalEra |
| Painting | 5 | 3 | Artifact, ArtistProfile, Dimensions, HistoricalEra |
| Sculpture | 4 | 3 | Artifact, ArtistProfile, Dimensions, HistoricalEra |
| AncientManuscript | 5 | 3 | Artifact, Dimensions, HistoricalEra |
| ArcheologicalFind | 5 | 3 | Artifact, Dimensions, HistoricalEra |
| ClimateSensor | 5 | 3 | — |
| ClimateControlUnit | 5 | 3 | ClimateSensor, AncientManuscript, Painting |
| DisplayCase | 6 | 4 | Dimensions, Artifact |
| LightingSystem | 4 | 3 | — |
| ExhibitionHall | 7 | 4 | Sculpture |
| StorageVault | 5 | 4 | Artifact, ClimateControlUnit |
| StorageRack | 5 | 3 | Artifact |
| TransportCrate | 5 | 3 | Dimensions, Artifact |
| PartnerMuseum | 5 | 3 | — |
| LoanAgreement | 6 | 3 | PartnerMuseum, Artifact |
| CatalogEntry | 5 | 3 | — |
| CollectionCatalog | 3 | 4 | CatalogEntry, Artifact |
| Exhibition | 7 | 4 | Artifact, ExhibitionHall |
| ExcursionRoute | 4 | 3 | ExhibitionHall |
| GuidedTour | 6 | 4 | ExcursionRoute |
| AudioGuideDevice | 5 | 4 | — |
| SpecialEvent | 6 | 3 | — |
| ExhibitionSchedule | 3 | 3 | Exhibition, SpecialEvent |
| DiscountPolicy | 5 | 3 | — |
| Visitor | 6 | 3 | — |
| Ticket | 6 | 3 | Visitor |
| MembershipCard | 5 | 4 | — |
| CloakroomTag | 4 | 3 | Visitor |
| VisitorFeedback | 5 | 3 | — |
| Employee | 6 | 3 | — |
| Curator | 3 | 3 | Employee, Artifact, Exhibition, ExhibitionHall |
| TourGuide | 3 | 3 | Employee, GuidedTour |
| Restorer | 3 | 3 | Employee, Artifact |
| SecurityGuard | 4 | 3 | Employee, ExhibitionHall, DisplayCase |
| HRDepartment | 3 | 4 | Employee |
| RestorationMaterial | 4 | 3 | — |
| ConditionReport | 5 | 3 | — |
| RestorationTask | 6 | 3 | Artifact, Restorer, RestorationMaterial |
| RestorationWorkshop | 4 | 3 | RestorationTask, Artifact, Restorer, RestorationMaterial |
| SecurityCamera | 5 | 3 | — |
| AlarmTrigger | 4 | 3 | SecurityGuard |
| AccessBadge | 4 | 3 | StorageVault |
| SecurityControlCenter | 4 | 4 | SecurityCamera, AlarmTrigger, ExhibitionHall, SecurityGuard |
| MuseumBudget | 5 | 4 | — |
| TicketOffice | 4 | 3 | DiscountPolicy, Visitor, Exhibition, ExhibitionHall, Ticket, MuseumBudget |
| DonationGrant | 5 | 3 | MuseumBudget, RestorationTask |
| SouvenirItem | 5 | 3 | MembershipCard |
| GiftShop | 4 | 3 | SouvenirItem, Visitor, MuseumBudget |
| Museum | 9 | 5 | CollectionCatalog, StorageVault, MuseumBudget, HRDepartment, SecurityControlCenter, ExhibitionHall, Artifact |
| Exceptions (13 классов) | 9 | 8 | MuseumException, ArtifactNotFoundException, ClimateOutOfRangeException и др. |

---

## Собственные классы исключений (13)

1. `MuseumException` — базовый класс исключений музейной системы (с кодом и категорией ошибки)
2. `ArtifactNotFoundException` — экспонат не найден в каталоге, витрине или хранилище
3. `InvalidArtifactDataException` — некорректные параметры экспоната (габариты, даты, сохранность)
4. `HallOvercapacityException` — превышена допустимая вместимость зала, стеллажа или хранилища
5. `ClimateOutOfRangeException` — нарушение температурно-влажностного или светового режима хранения
6. `TicketSoldOutException` — билеты на мероприятие, экскурсию или сувениры распроданы
7. `InvalidTicketException` — билет или абонемент недействителен, уже использован или возвращён
8. `InsufficientFundsException` — недостаточно средств у посетителя или в бюджете музея
9. `RestorationRequiredException` — экспонат в аварийном состоянии (< 40%) и требует реставрации
10. `SecurityBreachException` — нарушение режима безопасности или попытка доступа по отозванному пропуску
11. `StaffNotAvailableException` — сотрудник не на смене или не имеет необходимой квалификации
12. `ExhibitionClosedException` — выставка закрыта для посещения или пуста
13. `LoanAgreementViolationException` — нарушение условий межмузейного займа (низкий рейтинг партнёра или залог)

---

## Итоговая статистика проекта

| Показатель | Требование | Фактическое значение |
|---|---|---|
| Классы (доменные + исключения) | $\ge 50$ | **52 доменных + 13 классов исключений (всего 65)** |
| Поля | $\ge 150$ | **259** |
| Уникальные содержательные поведения (без геттеров/сеттеров) | $\ge 100$ | **175** |
| Ассоциации между классами | $\ge 30$ | **76** |
| Собственные классы исключений | $\ge 12$ | **13** |
| Модульные тесты (`UnitTest++`) | покрытие $> 90\%$ | **32 тестовых сценария (покрытие > 95%)** |

---

## Сборка и запуск

### На Windows (PowerShell + Visual Studio CMake)

```powershell
chcp 65001
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$cmake = "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

cd "C:\Users\Рома\Desktop\Универ\ППОИС\Lab2"
& $cmake -S . -B build
& $cmake --build build --config Release

# Запуск всех 32 модульных тестов UnitTest++
.\build\Release\museum_tests.exe

# Запуск интерактивной консольной программы с меню
.\build\Release\museum_app.exe
```

### На Linux / macOS (Make)

```bash
make all
./build/museum_tests
./build/museum_app
make coverage
make docs
```
