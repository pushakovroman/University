/**
 * @file main.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Консольная программа с интерактивным меню для демонстрации класса CantorSet.
 * @details Пользовательский интерфейс полностью отделён от реализации класса CantorSet.
 * Позволяет интерактивно вводить множества, добавлять/удалять элементы, проверять
 * принадлежность, вычислять мощность, объединение, пересечение, разность и булеан.
 */

#include "CantorSet.h"

#include <iostream>
#include <limits>
#include <string>

namespace {

/**
 * @brief Печатает главное меню консольной программы.
 * @param setA Первое рабочее множество A.
 * @param setB Второе рабочее множество B.
 */
void printMenu(const CantorSet& setA, const CantorSet& setB) {
    std::cout << "\n============================================================\n";
    std::cout << "   Лабораторная работа №1: Канторовское множество (1.4.1)\n";
    std::cout << "============================================================\n";
    std::cout << " Текущее множество A = " << setA
              << " (мощность: " << setA.cardinality()
              << ", пустое: " << (setA.isEmpty() ? "да" : "нет") << ")\n";
    std::cout << " Текущее множество B = " << setB
              << " (мощность: " << setB.cardinality()
              << ", пустое: " << (setB.isEmpty() ? "да" : "нет") << ")\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "  1. Задать множество A из строки (например: {a, b, {a, c}, {}})\n";
    std::cout << "  2. Задать множество B из строки\n";
    std::cout << "  3. Добавить элемент (атом или подмножество) в множество A\n";
    std::cout << "  4. Удалить элемент (атом или подмножество) из множества A\n";
    std::cout << "  5. Проверить принадлежность элемента множеству A (оператор [])\n";
    std::cout << "  6. Объединение множеств: A + B и A += B\n";
    std::cout << "  7. Пересечение множеств: A * B и A *= B\n";
    std::cout << "  8. Разность множеств: A - B и A -= B\n";
    std::cout << "  9. Построить булеан P(A) (множество всех подмножеств A)\n";
    std::cout << " 10. Сравнить множества A и B (==, !=, <=, <, >=, >)\n";
    std::cout << " 11. Очистить множество A\n";
    std::cout << "  0. Выход\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Выберите пункт меню: ";
}

/**
 * @brief Считывает непустую строку от пользователя.
 * @param prompt Приглашение ко вводу.
 * @return Введённая строка.
 */
std::string readLinePrompt(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

} // namespace

/**
 * @brief Точка входа в консольную программу.
 * @return Код завершения программы (0 при успешном завершении).
 */
int main() {
    CantorSet setA("{a, b, {a, b}, {}}");
    CantorSet setB("{b, c, {a, b}}");

    int choice = -1;
    while (true) {
        printMenu(setA, setB);
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                std::cout << "\nЗавершение работы (EOF).\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Ошибка] Введите целое число от 0 до 11.\n";
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
                    std::string s = readLinePrompt("Введите множество A в формате {...}: ");
                    setA = CantorSet(s);
                    std::cout << "Множество A успешно обновлено: " << setA << "\n";
                    break;
                }
                case 2: {
                    std::string s = readLinePrompt("Введите множество B в формате {...}: ");
                    setB = CantorSet(s);
                    std::cout << "Множество B успешно обновлено: " << setB << "\n";
                    break;
                }
                case 3: {
                    std::string elem = readLinePrompt("Введите элемент (атом или подмножество {...}): ");
                    bool added = false;
                    if (!elem.empty() && elem.front() == '{') {
                        added = setA.add(CantorSet(elem));
                    } else {
                        added = setA.add(elem);
                    }
                    std::cout << (added ? "Элемент добавлен в A: " : "Элемент уже присутствует в A: ")
                              << setA << "\n";
                    break;
                }
                case 4: {
                    std::string elem = readLinePrompt("Введите удаляемый элемент (атом или {...}): ");
                    bool removed = false;
                    if (!elem.empty() && elem.front() == '{') {
                        removed = setA.remove(CantorSet(elem));
                    } else {
                        removed = setA.remove(elem);
                    }
                    std::cout << (removed ? "Элемент удалён из A: " : "Элемент не найден в A: ")
                              << setA << "\n";
                    break;
                }
                case 5: {
                    std::string elem = readLinePrompt("Введите проверяемый элемент (атом или {...}): ");
                    bool found = false;
                    if (!elem.empty() && elem.front() == '{') {
                        found = setA[CantorSet(elem)];
                    } else {
                        found = setA[elem];
                    }
                    std::cout << "Результат A[" << elem << "] = "
                              << (found ? "ПРИНАДЛЕЖИТ (true)" : "НЕ ПРИНАДЛЕЖИТ (false)") << "\n";
                    break;
                }
                case 6: {
                    CantorSet res = setA + setB;
                    std::cout << "A + B = " << res << " (мощность: " << res.cardinality() << ")\n";
                    break;
                }
                case 7: {
                    CantorSet res = setA * setB;
                    std::cout << "A * B = " << res << " (мощность: " << res.cardinality() << ")\n";
                    break;
                }
                case 8: {
                    CantorSet res = setA - setB;
                    std::cout << "A - B = " << res << " (мощность: " << res.cardinality() << ")\n";
                    break;
                }
                case 9: {
                    CantorSet ps = setA.powerSet();
                    std::cout << "Булеан P(A) = " << ps
                              << "\nМощность P(A): " << ps.cardinality() << "\n";
                    break;
                }
                case 10: {
                    std::cout << "A == B : " << (setA == setB ? "true" : "false") << "\n";
                    std::cout << "A != B : " << (setA != setB ? "true" : "false") << "\n";
                    std::cout << "A <= B (подмножество)          : " << (setA <= setB ? "true" : "false") << "\n";
                    std::cout << "A <  B (собственное подмнож.)  : " << (setA < setB ? "true" : "false") << "\n";
                    std::cout << "A >= B (надмножество)          : " << (setA >= setB ? "true" : "false") << "\n";
                    std::cout << "A >  B (собственное надмнож.)  : " << (setA > setB ? "true" : "false") << "\n";
                    break;
                }
                case 11: {
                    setA.clear();
                    std::cout << "Множество A очищено: " << setA << "\n";
                    break;
                }
                default:
                    std::cout << "[Ошибка] Неизвестный пункт меню. Выберите от 0 до 11.\n";
                    break;
            }
        } catch (const std::exception& ex) {
            std::cout << "[Исключение] " << ex.what() << "\n";
        }
    }

    return 0;
}
