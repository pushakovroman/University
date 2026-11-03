/**
 * @file test_post_machine.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Модульные тесты для классов PostTape, PostRule, PostProgram и PostMachine (UnitTest++).
 * @details Содержит 36 тестовых сценариев, проверяющих все методы и операторы четырёх классов
 * машины Поста, включая динамическое расширение ленты в обе стороны, правило трёх,
 * обработку исключений, потоковый ввод/вывод и режим логирования (-log).
 */

#include "PostMachine.h"
#include "PostProgram.h"
#include "PostRule.h"
#include "PostTape.h"
#include <UnitTest++/UnitTest++.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

SUITE(PostMachineTests) {

    // 1. Конструктор ленты по умолчанию
    TEST(TapeDefaultConstructor) {
        PostTape tape;
        CHECK_EQUAL(0, tape.carriagePosition());
        CHECK(!tape.isMarked());
        CHECK_EQUAL(0u, tape.countMarks());
        CHECK_EQUAL("[0]", tape.toString());
        CHECK_EQUAL("0", tape.toPlainString());
    }

    // 2. Конструктор ленты из std::string и const char* со скобками каретки
    TEST(TapeConstructFromStringAndCString) {
        PostTape t1(std::string("10[1]1"));
        CHECK_EQUAL(2, t1.carriagePosition());
        CHECK(t1.isMarked());
        CHECK_EQUAL(3u, t1.countMarks());
        CHECK_EQUAL("10[1]1", t1.toString());
        CHECK_EQUAL("1011", t1.toPlainString());

        PostTape t2("01.1", 1);
        CHECK_EQUAL(1, t2.carriagePosition());
        CHECK(t2.isMarked());
        CHECK(!t2[2]);
    }

    // 3. Исключения при некорректной строке ленты или nullptr
    TEST(TapeThrowsOnInvalidStringOrNull) {
        const char* nullPtr = nullptr;
        CHECK_THROW(PostTape t(nullPtr), std::invalid_argument);
        CHECK_THROW(PostTape("[1][0]"), std::invalid_argument);
        CHECK_THROW(PostTape("[10]"), std::invalid_argument);
        CHECK_THROW(PostTape("]10["), std::invalid_argument);
        CHECK_THROW(PostTape("[10"), std::invalid_argument);
        CHECK_THROW(PostTape("01201"), std::invalid_argument);
    }

    // 4. Конструктор копирования и оператор присваивания PostTape
    TEST(TapeCopyConstructorAndAssignment) {
        PostTape orig("1[0]1");
        PostTape copy(orig);
        CHECK(orig == copy);

        copy.mark();
        CHECK(orig != copy);
        CHECK(!orig.isMarked());
        CHECK(copy.isMarked());

        orig = copy;
        CHECK(orig == copy);
        orig = orig; // самоприсваивание
        CHECK(orig == copy);
    }

    // 5. Установка и стирание меток под кареткой (mark / erase)
    TEST(TapeMarkAndEraseAtCarriage) {
        PostTape tape;
        CHECK(!tape.isMarked());
        tape.mark();
        CHECK(tape.isMarked());
        CHECK_EQUAL(1u, tape.countMarks());
        tape.erase();
        CHECK(!tape.isMarked());
        CHECK_EQUAL(0u, tape.countMarks());
    }

    // 6. Перемещение каретки пре- и постинкрементом / декрементом (++ и --)
    TEST(TapeCarriageIncrementAndDecrementOperators) {
        PostTape tape("[1]01");
        CHECK_EQUAL(0, tape.carriagePosition());

        ++tape;
        CHECK_EQUAL(1, tape.carriagePosition());
        CHECK(!tape.isMarked());

        PostTape beforePostInc = tape++;
        CHECK_EQUAL(1, beforePostInc.carriagePosition());
        CHECK_EQUAL(2, tape.carriagePosition());
        CHECK(tape.isMarked());

        --tape;
        CHECK_EQUAL(1, tape.carriagePosition());

        PostTape beforePostDec = tape--;
        CHECK_EQUAL(1, beforePostDec.carriagePosition());
        CHECK_EQUAL(0, tape.carriagePosition());
    }

    // 7. Динамическое расширение ленты в отрицательные и большие положительные индексы
    TEST(TapeDynamicExpansionNegativeAndPositiveIndices) {
        PostTape tape;
        tape[-15] = true;
        tape[25] = true;
        const PostTape& constRef = tape;
        CHECK(constRef[-15]);
        CHECK(constRef[25]);
        CHECK(!constRef[-100]);
        CHECK(!constRef[100]);
        CHECK_EQUAL(2u, tape.countMarks());

        tape.setCarriagePosition(-15);
        CHECK(tape.isMarked());
    }

    // 8. Очистка ленты clear()
    TEST(TapeClearResetsToEmpty) {
        PostTape tape("11[1]11");
        tape.clear();
        CHECK_EQUAL(0, tape.carriagePosition());
        CHECK_EQUAL(0u, tape.countMarks());
        CHECK_EQUAL("[0]", tape.toString());
    }

    // 9. Сравнение лент с разными размерами буфера, но одинаковыми метками
    TEST(TapeEqualityAndInequalityComparison) {
        PostTape t1("101", 0);
        PostTape t2;
        t2[0] = true;
        t2[2] = true;
        t2[50] = false; // расширяем буфер без добавления метки
        CHECK(t1 == t2);

        t2.setCarriagePosition(1);
        CHECK(t1 != t2);

        t2.setCarriagePosition(0);
        t2[5] = true;
        CHECK(t1 != t2);

        PostTape t3;
        t3[0] = true;
        t3[3] = true;
        CHECK(t1 != t3);
    }

    // 10. Потоковые операторы << и >> для PostTape
    TEST(TapeStreamInsertionAndExtraction) {
        std::istringstream iss("  01[1]0  ");
        PostTape tape;
        iss >> tape;
        CHECK(!iss.fail());
        CHECK_EQUAL(2, tape.carriagePosition());
        CHECK(tape.isMarked());

        std::ostringstream oss;
        oss << tape;
        CHECK_EQUAL("01[1]", oss.str());

        std::istringstream badIss("invalid_tape");
        badIss >> tape;
        CHECK(badIss.fail());

        std::istringstream emptyIss("   ");
        emptyIss >> tape;
        CHECK(emptyIss.fail());
    }

    // 11. Создание правил PostRule всех 6 типов (V, X, >, <, ?, !)
    TEST(RuleCreationAllActionTypes) {
        PostRule rMark(1, 'V', 2);
        PostRule rErase(2, 'X', 3);
        PostRule rRight(3, '>', 4);
        PostRule rLeft(4, '<', 5);
        PostRule rBranch(5, '?', 6, 1);
        PostRule rHalt(6, '!');

        CHECK_EQUAL(1, rMark.id());
        CHECK_EQUAL('V', rMark.actionChar());
        CHECK_EQUAL(2, rMark.next1());
        CHECK_EQUAL("1: V 2", rMark.toString());

        CHECK_EQUAL("2: X 3", rErase.toString());
        CHECK_EQUAL("3: > 4", rRight.toString());
        CHECK_EQUAL("4: < 5", rLeft.toString());
        CHECK_EQUAL("5: ? 6, 1", rBranch.toString());
        CHECK_EQUAL(6, rBranch.next1());
        CHECK_EQUAL(1, rBranch.next2());
        CHECK_EQUAL("6: !", rHalt.toString());
    }

    // 12. Альтернативные обозначения команд PostRule (1, 0, R, L, .)
    TEST(RuleAlternativeActionCharacters) {
        CHECK(PostRule::charToAction('1') == PostRule::Action::Mark);
        CHECK(PostRule::charToAction('v') == PostRule::Action::Mark);
        CHECK(PostRule::charToAction('0') == PostRule::Action::Erase);
        CHECK(PostRule::charToAction('x') == PostRule::Action::Erase);
        CHECK(PostRule::charToAction('R') == PostRule::Action::MoveRight);
        CHECK(PostRule::charToAction('r') == PostRule::Action::MoveRight);
        CHECK(PostRule::charToAction('L') == PostRule::Action::MoveLeft);
        CHECK(PostRule::charToAction('l') == PostRule::Action::MoveLeft);
        CHECK(PostRule::charToAction('.') == PostRule::Action::Halt);
    }

    // 13. Валидация параметров и исключения в PostRule
    TEST(RuleValidationThrowsOnInvalidArguments) {
        CHECK_THROW(PostRule(0, 'V', 1), std::invalid_argument);
        CHECK_THROW(PostRule(-2, '>', 1), std::invalid_argument);
        CHECK_THROW(PostRule(1, 'V', 0), std::invalid_argument);
        CHECK_THROW(PostRule(1, '?', 2, 0), std::invalid_argument);
        CHECK_THROW(PostRule(1, 'Z', 2), std::invalid_argument);
    }

    // 14. Парсинг PostRule::fromString и обработка синтаксических ошибок
    TEST(RuleFromStringParsingAndErrors) {
        PostRule r1 = PostRule::fromString("10: ? 11, 12 # comment");
        CHECK_EQUAL(10, r1.id());
        CHECK(r1.action() == PostRule::Action::Branch);
        CHECK_EQUAL(11, r1.next1());
        CHECK_EQUAL(12, r1.next2());

        PostRule r2 = PostRule::fromString("5: ! // stop here");
        CHECK(r2.action() == PostRule::Action::Halt);

        CHECK_THROW(PostRule::fromString(""), std::invalid_argument);
        CHECK_THROW(PostRule::fromString("1: ? 2"), std::invalid_argument);
        CHECK_THROW(PostRule::fromString("1: V"), std::invalid_argument);
        CHECK_THROW(PostRule::fromString("1: V 2 99"), std::invalid_argument);
    }

    // 15. Копирование, присваивание и потоковые операторы PostRule
    TEST(RuleCopyAssignmentAndStreams) {
        PostRule rDefault;
        CHECK_EQUAL(1, rDefault.id());
        CHECK(rDefault.action() == PostRule::Action::Halt);

        PostRule r1(2, '>', 3);
        PostRule r2(r1);
        CHECK(r1 == r2);

        rDefault = r1;
        rDefault = rDefault;
        CHECK(rDefault == r1);
        CHECK(!(rDefault != r1));

        std::ostringstream oss;
        oss << r1;
        CHECK_EQUAL("2: > 3", oss.str());

        std::istringstream iss("\n  3: < 4 \n");
        PostRule r3;
        iss >> r3;
        CHECK(!iss.fail());
        CHECK_EQUAL(3, r3.id());
        CHECK_EQUAL('<', r3.actionChar());

        std::istringstream badIss("bad rule");
        badIss >> r3;
        CHECK(badIss.fail());

        std::istringstream emptyIss("   ");
        emptyIss >> r3;
        CHECK(emptyIss.fail());
    }

    // 16. Добавление, замена и сортировка правил в PostProgram
    TEST(ProgramAddReplaceAndSorting) {
        PostProgram prog;
        CHECK(prog.isEmpty());
        CHECK_EQUAL(-1, prog.firstRuleId());

        prog += PostRule(3, '!');
        prog.addRule("1: > 2");
        prog.addRule(PostRule(2, 'V', 3));
        prog.addRule("4: < 3");
        prog.addRule("5: X 3");

        CHECK(!prog.isEmpty());
        CHECK_EQUAL(5u, prog.size());
        CHECK_EQUAL(1, prog.firstRuleId());
        CHECK_EQUAL(1, prog.ruleAtIndex(0).id());
        CHECK_EQUAL(2, prog.ruleAtIndex(1).id());

        // Замена существующего правила #2
        prog.addRule("2: X 3");
        CHECK_EQUAL(5u, prog.size());
        CHECK_EQUAL('X', prog[2].actionChar());
    }

    // 17. Удаление правил и проверка принадлежности в PostProgram
    TEST(ProgramRemoveAndLookupRules) {
        PostProgram prog;
        prog.addRule("1: > 2");
        prog.addRule("2: !");
        CHECK(prog.hasRule(1));
        CHECK(!prog.hasRule(99));

        CHECK(prog.removeRule(1));
        CHECK(!prog.removeRule(1));
        prog -= 2;
        CHECK(prog.isEmpty());

        CHECK_THROW(prog.getRule(1), std::out_of_range);
        CHECK_THROW(prog.ruleAtIndex(0), std::out_of_range);
    }

    // 18. Глубокое копирование, присваивание и сравнение PostProgram
    TEST(ProgramCopyAssignmentAndComparison) {
        PostProgram p1;
        p1.addRule("1: V 2");
        p1.addRule("2: !");

        PostProgram p2(p1);
        CHECK(p1 == p2);

        p2.addRule("3: > 1");
        CHECK(p1 != p2);

        PostProgram p3;
        p3.addRule("1: V 2");
        p3.addRule("2: > 1");
        CHECK(p1 != p3);

        p3 = p1;
        p3 = p3;
        CHECK(p3 == p1);

        p3.clear();
        CHECK(p3.isEmpty());
    }

    // 19. Потоковый ввод и вывод PostProgram
    TEST(ProgramStreamLoadAndOutput) {
        std::string text =
            "# Пример программы\n"
            "1: ? 2, 3 // проверка метки\n"
            "2: V 4\n"
            "3: > 1\n"
            "4: !\n";
        std::istringstream iss(text);
        PostProgram prog;
        iss >> prog;
        CHECK(!iss.fail());
        CHECK_EQUAL(4u, prog.size());

        std::ostringstream oss;
        oss << prog;
        // Второй вызов проверяет mutable-кэш
        CHECK_EQUAL(oss.str(), prog.toString());

        std::istringstream badIss("1: INVALID_ACTION 2");
        badIss >> prog;
        CHECK(badIss.fail());
    }

    // 20. Пошаговое выполнение PostMachine (step и операторы ++machine / machine++)
    TEST(MachineStepAndIncrementOperators) {
        PostTape tape("[0]0");
        PostProgram prog;
        prog.addRule("1: V 2");
        prog.addRule("2: > 3");
        prog.addRule("3: V 4");
        prog.addRule("4: < 5");
        prog.addRule("5: X 6");
        prog.addRule("6: !");

        PostMachine machine(tape, prog);
        CHECK_EQUAL(1, machine.currentRuleId());
        CHECK(!machine.isHalted());

        CHECK(machine.step()); // 1: V 2 -> ставит 1 в pos 0
        CHECK(machine.tape()[0]);
        CHECK_EQUAL(2, machine.currentRuleId());

        ++machine; // 2: > 3 -> сдвиг в pos 1
        CHECK_EQUAL(1, machine.tape().carriagePosition());

        PostMachine prev = machine++; // 3: V 4 -> ставит 1 в pos 1
        CHECK_EQUAL(3, prev.currentRuleId());
        CHECK(machine.tape()[1]);

        CHECK(machine.step()); // 4: < 5 -> возврат в pos 0
        CHECK(machine.step()); // 5: X 6 -> стирает метку в pos 0
        CHECK(!machine.tape()[0]);

        CHECK(!machine.step()); // 6: ! -> останов
        CHECK(machine.isHalted());
        CHECK(!machine.step()); // повторный вызов на остановленной машине возвращает false
    }

    // 21. Интерпретация алгоритма прибавления 1 к унарному числу (run)
    TEST(MachineRunUnaryIncrementAlgorithm) {
        PostTape tape("[1]11"); // унарное 3 (три метки)
        PostProgram prog;
        prog.addRule("1: ? 2, 3"); // если 0 -> правило 2 (поставить метку), если 1 -> правило 3 (шаг вправо)
        prog.addRule("2: V 4");
        prog.addRule("3: > 1");
        prog.addRule("4: !");

        PostMachine machine(tape, prog);
        std::ostringstream logOut;
        std::size_t steps = machine.run(true, logOut);

        CHECK(machine.isHalted());
        CHECK_EQUAL(9u, steps);
        CHECK_EQUAL(4u, machine.tape().countMarks());
        CHECK_EQUAL("111[1]", machine.tape().toString());
        CHECK(!logOut.str().empty());
    }

    // 22. Алгоритм инвертирования 3 битов на ленте
    TEST(MachineRunBitInversionAlgorithm) {
        PostTape tape("[1]01");
        PostProgram prog;
        // Бит 0
        prog.addRule("1: ? 2, 3");
        prog.addRule("2: V 4");
        prog.addRule("3: X 4");
        prog.addRule("4: > 5");
        // Бит 1
        prog.addRule("5: ? 6, 7");
        prog.addRule("6: V 8");
        prog.addRule("7: X 8");
        prog.addRule("8: > 9");
        // Бит 2
        prog.addRule("9: ? 10, 11");
        prog.addRule("10: V 12");
        prog.addRule("11: X 12");
        prog.addRule("12: !");

        PostMachine machine(tape, prog);
        machine.run();
        CHECK(machine.isHalted());
        CHECK_EQUAL("01[0]", machine.tape().toString());
    }

    // 23. Защита от бесконечного цикла (превышение maxSteps)
    TEST(MachineRunThrowsOnInfiniteLoop) {
        PostTape tape("[0]");
        PostProgram prog;
        prog.addRule("1: > 1"); // бесконечное движение вправо
        PostMachine machine(tape, prog);
        CHECK_THROW(machine.run(false, std::cout, 50), std::runtime_error);
    }

    // 24. Исключения при пустой программе или переходе на несуществующее правило
    TEST(MachineThrowsOnEmptyProgramOrMissingRule) {
        PostMachine emptyMachine;
        CHECK_THROW(emptyMachine.step(), std::runtime_error);

        PostMachine brokenMachine;
        brokenMachine.program().addRule("1: > 99");
        brokenMachine.resetExecution();
        CHECK(brokenMachine.step()); // выполняет правило 1 и переходит на 99
        CHECK_THROW(brokenMachine.step(), std::runtime_error); // правила 99 нет
    }

    // 25. Загрузка PostMachine из потока (TAPE:, CARRIAGE: и правила)
    TEST(MachineLoadFromStreamWithHeaders) {
        std::string content =
            "# Файл конфигурации Машины Поста\n"
            "TAPE: 1101\n"
            "CARRIAGE: 2\n"
            "1: ? 2, 3\n"
            "2: V 3\n"
            "3: !\n";
        std::istringstream iss(content);
        PostMachine machine;
        iss >> machine;
        CHECK(!iss.fail());
        CHECK_EQUAL(2, machine.tape().carriagePosition());
        CHECK_EQUAL(3u, machine.program().size());

        machine.run();
        CHECK(machine.isHalted());
        CHECK_EQUAL("11[1]1", machine.tape().toString());
    }

    // 26. Загрузка PostMachine из потока с краткой записью ленты без префикса TAPE:
    TEST(MachineLoadFromStreamCompactTapeLine) {
        std::string content =
            "[0]1\n"
            "1: V 2\n"
            "2: !\n";
        std::istringstream iss(content);
        PostMachine machine;
        machine.loadFromStream(iss);
        machine.run();
        CHECK_EQUAL("[1]1", machine.tape().toString());
    }

    // 27. Загрузка PostMachine из файла и проверка ошибки при отсутствии файла
    TEST(MachineLoadFromFileAndMissingFileError) {
        const std::string tempFile = "/tmp/test_post_machine_sample.txt";
        {
            std::ofstream ofs(tempFile);
            ofs << "TAPE: [1]1\n"
                << "1: X 2\n"
                << "2: > 3\n"
                << "3: !\n";
        }
        PostMachine machine;
        machine.loadFromFile(tempFile);
        machine.run();
        CHECK_EQUAL("0[1]", machine.tape().toString());
        std::remove(tempFile.c_str());

        CHECK_THROW(machine.loadFromFile("/tmp/non_existent_post_file_12345.txt"),
                    std::runtime_error);
    }

    // 28. Копирование, присваивание, сброс и сравнение PostMachine
    TEST(MachineCopyAssignmentResetAndComparison) {
        PostMachine m1;
        m1.tape().loadFromString("[1]0");
        m1.program().addRule("1: X 2");
        m1.program().addRule("2: !");
        m1.resetExecution();

        PostMachine m2(m1);
        CHECK(m1 == m2);

        m2.step();
        CHECK(m1 != m2);

        m2.resetExecution();
        m2.tape().loadFromString("[1]0");
        CHECK(m1 == m2);

        m1 = m2;
        m1 = m1;
        CHECK(m1 == m2);
    }

    // 29. Потоковый вывод << и ввод >> с ошибкой для PostMachine
    TEST(MachineStreamOutputAndInvalidInput) {
        PostMachine m1;
        m1.tape().loadFromString("[1]0");
        m1.program().addRule("1: !");
        m1.resetExecution();

        std::ostringstream oss;
        oss << m1;
        CHECK(!oss.str().empty());

        std::istringstream badIss("TAPE: [1]\n1: UNKNOWN_CMD 2\n");
        PostMachine m2;
        badIss >> m2;
        CHECK(badIss.fail());
    }

    // 30. Ручная установка номера текущего правила setCurrentRuleId
    TEST(MachineSetCurrentRuleIdDirectly) {
        PostMachine m;
        m.tape().loadFromString("[0]");
        m.program().addRule("1: > 2");
        m.program().addRule("5: V 6");
        m.program().addRule("6: !");

        m.setCurrentRuleId(5);
        CHECK_EQUAL(5, m.currentRuleId());
        m.run();
        CHECK(m.tape().isMarked());
        CHECK(m.isHalted());
    }

    // 31. Проверка константных геттеров PostMachine
    TEST(MachineConstGettersVerification) {
        PostTape t("[1]");
        PostProgram p;
        p.addRule("1: !");
        const PostMachine cm(t, p);

        CHECK(cm.tape().isMarked());
        CHECK_EQUAL(1u, cm.program().size());
        CHECK_EQUAL(1, cm.currentRuleId());
        CHECK_EQUAL(0u, cm.stepCount());
        CHECK(!cm.isHalted());
    }

} // SUITE(PostMachineTests)

int main(int, const char*[]) {
    return UnitTest::RunAllTests();
}
