/**
 * @file main.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Точка входа консольного приложения Машины Поста (вариант 1.8 / 1.8.1).
 * @details Поддерживает два режима работы:
 * 1. Пакетный режим командной строки (по заданию п. 1.8):
 *    `./post_machine_app <путь_к_файлу> [-log]`
 *    Считывает начальное состояние ленты и программу из файла и выполняет интерпретацию.
 *    При наличии флага `-log` выводит состояние машины после каждого выполненного правила.
 * 2. Интерактивный режим с консольным меню (при запуске без аргументов или с флагом `--menu`).
 */

#include "PostMachine.h"

#include <iostream>
#include <limits>
#include <string>

namespace {

/**
 * @brief Выводит главное меню интерактивного режима.
 * @param machine Текущий экземпляр машины Поста.
 */
void printInteractiveMenu(const PostMachine& machine) {
    std::cout << "\n====================================================================\n";
    std::cout << "     Лабораторная работа №1: Машина Поста (Вариант 1.8 / 1.8.1)\n";
    std::cout << "====================================================================\n";
    std::cout << " Текущее состояние: " << machine.formatState() << "\n";
    std::cout << " Правил в программе: " << machine.program().size()
              << " | Статус останова: " << (machine.isHalted() ? "ОСТАНОВЛЕНА" : "ГОТОВА") << "\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "  1. Загрузить начальное состояние и программу из файла\n";
    std::cout << "  2. Задать состояние ленты из строки (например: [1]110 или 0110)\n";
    std::cout << "  3. Изменить значение секции на ленте / переместить каретку (++ / --)\n";
    std::cout << "  4. Просмотреть все правила программы\n";
    std::cout << "  5. Добавить или заменить правило (например: 1: ? 2, 3)\n";
    std::cout << "  6. Удалить правило по номеру\n";
    std::cout << "  7. Выполнить один шаг программы (step / ++machine)\n";
    std::cout << "  8. Выполнить всю программу целиком (без лога)\n";
    std::cout << "  9. Выполнить всю программу с пошаговым логом (-log)\n";
    std::cout << " 10. Сбросить счётчик шагов и вернуть на стартовое правило\n";
    std::cout << "  0. Выход\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "Выберите пункт меню: ";
}

/**
 * @brief Запускает интерактивное консольное меню.
 * @return Код возврата (0).
 */
int runInteractiveMenu() {
    PostMachine machine;
    // Загрузим демонстрационную программу по умолчанию (прибавление 1 к унарному числу)
    machine.tape().loadFromString("[1]11");
    machine.program().addRule("1: ? 2, 3");
    machine.program().addRule("2: V 4");
    machine.program().addRule("3: > 1");
    machine.program().addRule("4: !");
    machine.resetExecution();

    int choice = -1;
    while (true) {
        printInteractiveMenu(machine);
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                std::cout << "\nЗавершение работы (EOF).\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Ошибка] Введите номер пункта меню от 0 до 10.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            std::cout << "Выход из программы.\n";
            break;
        }

        try {
            switch (choice) {
                case 1: {
                    std::cout << "Введите путь к файлу: ";
                    std::string path;
                    std::getline(std::cin >> std::ws, path);
                    machine.loadFromFile(path);
                    std::cout << "Файл успешно загружен.\n";
                    break;
                }
                case 2: {
                    std::cout << "Введите состояние ленты (например, [1]101): ";
                    std::string tapeStr;
                    std::getline(std::cin >> std::ws, tapeStr);
                    machine.tape().loadFromString(tapeStr);
                    std::cout << "Лента обновлена: " << machine.tape() << "\n";
                    break;
                }
                case 3: {
                    std::cout << "Действие (1 - метка под кареткой, 0 - стереть, > - сдвиг вправо, < - сдвиг влево): ";
                    char op = '\0';
                    std::cin >> op;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (op == '1') {
                        machine.tape().mark();
                    } else if (op == '0') {
                        machine.tape().erase();
                    } else if (op == '>') {
                        ++machine.tape();
                    } else if (op == '<') {
                        --machine.tape();
                    } else {
                        std::cout << "Неизвестная операция.\n";
                    }
                    break;
                }
                case 4: {
                    if (machine.program().isEmpty()) {
                        std::cout << "Программа пуста.\n";
                    } else {
                        std::cout << "Список правил программы:\n" << machine.program() << "\n";
                    }
                    break;
                }
                case 5: {
                    std::cout << "Введите правило (например, '1: > 2' или '2: ? 3, 4' или '4: !'): ";
                    std::string ruleStr;
                    std::getline(std::cin >> std::ws, ruleStr);
                    machine.program().addRule(ruleStr);
                    if (machine.currentRuleId() <= 0) {
                        machine.resetExecution();
                    }
                    std::cout << "Правило добавлено.\n";
                    break;
                }
                case 6: {
                    std::cout << "Введите номер удаляемого правила: ";
                    int id = 0;
                    std::cin >> id;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    bool removed = machine.program().removeRule(id);
                    std::cout << (removed ? "Правило удалено.\n" : "Правило с таким номером не найдено.\n");
                    break;
                }
                case 7: {
                    bool cont = machine.step();
                    std::cout << "Шаг выполнен: " << machine.formatState() << "\n";
                    if (!cont) {
                        std::cout << "Машина достигла команды останова (!).\n";
                    }
                    break;
                }
                case 8: {
                    std::size_t steps = machine.run(false, std::cout);
                    std::cout << "Программа завершена за " << steps << " шаг(ов).\n";
                    std::cout << "Итоговое состояние: " << machine.formatState() << "\n";
                    break;
                }
                case 9: {
                    std::size_t steps = machine.run(true, std::cout);
                    std::cout << "Программа завершена за " << steps << " шаг(ов).\n";
                    break;
                }
                case 10: {
                    machine.resetExecution();
                    std::cout << "Состояние интерпретатора сброшено на начало программы.\n";
                    break;
                }
                default:
                    std::cout << "[Ошибка] Выберите пункт от 0 до 10.\n";
                    break;
            }
        } catch (const std::exception& ex) {
            std::cout << "[Исключение] " << ex.what() << "\n";
        }
    }
    return 0;
}

} // namespace

/**
 * @brief Главная функция программы.
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * @return 0 при успешном выполнении, 1 при ошибке.
 */
int main(int argc, char* argv[]) {
    if (argc <= 1) {
        return runInteractiveMenu();
    }

    bool logFlag = false;
    std::string filePath;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-log" || arg == "--log") {
            logFlag = true;
        } else if (arg == "--menu") {
            return runInteractiveMenu();
        } else if (filePath.empty()) {
            filePath = arg;
        }
    }

    if (filePath.empty()) {
        std::cerr << "Использование: " << argv[0] << " <путь_к_файлу> [-log]\n";
        return 1;
    }

    try {
        PostMachine machine;
        machine.loadFromFile(filePath);
        if (logFlag) {
            std::cout << "[Начальное состояние] " << machine.formatState() << "\n";
        }
        machine.run(logFlag, std::cout);
        std::cout << "Результат на ленте: " << machine.tape() << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Ошибка выполнения машины Поста: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
