/**
 * @file PostMachine.h
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Заголовочный файл класса PostMachine (Абстрактная машина Поста).
 * @details Объединяет ленту с кареткой (PostTape) и программу (PostProgram).
 * Обеспечивает загрузку начального состояния и правил из файла/потока,
 * пошаговую и полную интерпретацию программы, а также логирование состояния (-log).
 */

#ifndef POST_MACHINE_H
#define POST_MACHINE_H

#include "PostProgram.h"
#include "PostTape.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @class PostMachine
 * @brief Класс, реализующий абстрактную машину Поста и её интерпретатор.
 * @details Содержит ленту (`PostTape`), программу (`PostProgram`), номер текущего
 * выполняемого правила (`currentRuleId_`), счётчик выполненных шагов (`stepCount_`)
 * и флаг завершения работы (`halted_`).
 */
class PostMachine {
public:
    /**
     * @brief Конструктор по умолчанию. Создаёт машину с пустой лентой и пустой программой.
     */
    PostMachine();

    /**
     * @brief Конструктор машины Поста с заданной лентой и программой.
     * @param tape Начальное состояние ленты и каретки.
     * @param program Программа из правил машины Поста.
     */
    PostMachine(const PostTape& tape, const PostProgram& program);

    /**
     * @brief Конструктор копирования.
     * @param other Копируемая машина Поста.
     */
    PostMachine(const PostMachine& other);

    /**
     * @brief Деструктор класса.
     */
    ~PostMachine();

    /**
     * @brief Оператор присваивания копированием.
     * @param other Присваиваемая машина Поста.
     * @return Ссылка на текущий объект (`*this`).
     */
    PostMachine& operator=(const PostMachine& other);

    /**
     * @brief Возвращает ссылку на ленту машины для чтения и модификации.
     * @return Неконстантная ссылка на `PostTape`.
     */
    PostTape& tape() {
        return this->tape_;
    }

    /**
     * @brief Возвращает константную ссылку на ленту машины (inline-метод).
     * @return Константная ссылка на `PostTape`.
     */
    const PostTape& tape() const {
        return this->tape_;
    }

    /**
     * @brief Возвращает ссылку на программу машины для добавления/удаления правил.
     * @return Неконстантная ссылка на `PostProgram`.
     */
    PostProgram& program() {
        return this->program_;
    }

    /**
     * @brief Возвращает константную ссылку на программу машины (inline-метод).
     * @return Константная ссылка на `PostProgram`.
     */
    const PostProgram& program() const {
        return this->program_;
    }

    /**
     * @brief Возвращает номер текущего правила, готового к выполнению (inline-метод).
     * @return Номер текущего правила (или `-1`, если машина остановлена или программа пуста).
     */
    int currentRuleId() const {
        return this->currentRuleId_;
    }

    /**
     * @brief Задаёт номер текущего правила для выполнения.
     * @param ruleId Номер стартового/текущего правила.
     */
    void setCurrentRuleId(int ruleId);

    /**
     * @brief Возвращает количество выполненных шагов с момента запуска/сброса (inline-метод).
     * @return Число выполненных шагов (`std::size_t`).
     */
    std::size_t stepCount() const {
        return this->stepCount_;
    }

    /**
     * @brief Проверяет, завершила ли машина выполнение программы (inline-метод).
     * @return `true`, если выполнена команда останова `'!'`, иначе `false`.
     */
    bool isHalted() const {
        return this->halted_;
    }

    /**
     * @brief Сбрасывает счётчик шагов, снимает флаг останова и устанавливает стартовое правило.
     */
    void resetExecution();

    /**
     * @brief Выполняет ровно один шаг (одно правило) программы машины Поста.
     * @return `true`, если после выполнения шага машина может продолжать работу;
     *         `false`, если этим шагом была выполнена команда останова `'!'` (или машина уже остановлена).
     * @throw std::runtime_error Если программа пуста или отсутствует правило с номером `currentRuleId_`.
     */
    bool step();

    /**
     * @brief Префиксный оператор инкремента (`++machine`) — выполняет один шаг интерпретации.
     * @return Ссылка на текущий объект (`*this`).
     */
    PostMachine& operator++();

    /**
     * @brief Постфиксный оператор инкремента (`machine++`) — выполняет один шаг интерпретации.
     * @return Копия состояния машины Поста до выполнения шага.
     */
    PostMachine operator++(int);

    /**
     * @brief Выполняет интерпретацию всей программы до команды останова `'!'`.
     * @param logSteps Если `true` (режим `-log`), после каждого правила выводит состояние в `logStream`.
     * @param logStream Поток для вывода лога шагов (по умолчанию `std::cout`).
     * @param maxSteps Предельное количество шагов для защиты от бесконечного зацикливания.
     * @return Общее количество выполненных шагов за данный запуск.
     * @throw std::runtime_error При обращении к несуществующему правилу или превышении `maxSteps`.
     */
    std::size_t run(bool logSteps = false,
                    std::ostream& logStream = std::cout,
                    std::size_t maxSteps = 100000);

    /**
     * @brief Загружает начальное состояние ленты и программу машины Поста из потока.
     * @details Формат входного потока/файла:
     * - Строка `TAPE: <состояние_ленты>` (или первая значащая строка без двоеточия из `0`/`1`/`[...]`);
     * - Опционально строка `CARRIAGE: <позиция>`;
     * - Далее строки правил программы (`1: > 2`, `2: ? 3, 4`, `3: !` и т. д.).
     * Пустые строки и комментарии (`# ...` или `// ...`) игнорируются.
     * @param is Входной поток.
     * @return Ссылка на входной поток.
     * @throw std::invalid_argument При ошибке формата ленты или правил.
     */
    std::istream& loadFromStream(std::istream& is);

    /**
     * @brief Загружает начальное состояние машины Поста и программу из файла.
     * @param filePath Путь к файлу.
     * @throw std::runtime_error Если файл не удалось открыть.
     * @throw std::invalid_argument При ошибке формата данных внутри файла.
     */
    void loadFromFile(const std::string& filePath);

    /**
     * @brief Формирует строку с информацией о текущем состоянии машины Поста.
     * @return Строка вида `"Шаг N | Правило: K | Каретка: P | Статус: ... | Лента: ..."`.
     */
    std::string formatState() const;

    /**
     * @brief Оператор сравнения двух машин Поста на равенство (`==`).
     * @param other Вторая машина Поста.
     * @return `true`, если совпадают ленты, программы, текущие правила, шаги и статус останова.
     */
    bool operator==(const PostMachine& other) const;

    /**
     * @brief Оператор сравнения двух машин Поста на неравенство (`!=`).
     * @param other Вторая машина Поста.
     * @return `true`, если состояния машин различаются.
     */
    bool operator!=(const PostMachine& other) const;

    /**
     * @brief Потоковый оператор вывода состояния машины Поста (`<<`).
     * @param os Выходной поток.
     * @param machine Выводимая машина Поста.
     * @return Ссылка на выходной поток.
     */
    friend std::ostream& operator<<(std::ostream& os, const PostMachine& machine);

    /**
     * @brief Потоковый оператор ввода машины Поста (`>>`).
     * @param is Входной поток.
     * @param machine Загружаемая машина Поста.
     * @return Ссылка на входной поток.
     */
    friend std::istream& operator>>(std::istream& is, PostMachine& machine);

private:
    PostTape tape_;           ///< Лента и каретка машины Поста.
    PostProgram program_;     ///< Программа (набор правил).
    int currentRuleId_;       ///< Номер следующего выполняемого правила.
    std::size_t stepCount_;   ///< Количество выполненных шагов.
    bool halted_;             ///< Флаг останова машины.
};

#endif // POST_MACHINE_H
