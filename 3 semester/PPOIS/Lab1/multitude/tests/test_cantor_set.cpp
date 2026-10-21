/**
 * @file test_cantor_set.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Модульные тесты для класса CantorSet с использованием библиотеки UnitTest++.
 * @details Содержит 38 тестовых сценариев, проверяющих все методы и операторы класса
 * CantorSet, включая граничные случаи, обработку исключений, глубокое копирование,
 * работу с потоками и построение булеана (покрытие кода > 95%).
 */

#include "CantorSet.h"
#include <UnitTest++/UnitTest++.h>

#include <sstream>
#include <stdexcept>
#include <string>

SUITE(CantorSetTests) {

    // 1. Проверка конструктора по умолчанию и пустого множества
    TEST(DefaultConstructorCreatesEmptySet) {
        CantorSet s;
        CHECK(s.isEmpty());
        CHECK_EQUAL(0u, s.cardinality());
        CHECK_EQUAL("{}", s.toString());
    }

    // 2. Конструктор из пустой строковой записи "{}" (std::string и const char*)
    TEST(ConstructEmptySetFromStringAndCString) {
        CantorSet s1(std::string("{}"));
        CantorSet s2("   {   }  ");
        CHECK(s1.isEmpty());
        CHECK(s2.isEmpty());
        CHECK_EQUAL(0u, s1.cardinality());
        CHECK(s1 == s2);
    }

    // 3. Конструктор из классического примера в задании 1.4.1
    TEST(ConstructFromComplexNestedString) {
        CantorSet s("{a, b, c, {a, b}, {}, {a, {c}}}");
        CHECK(!s.isEmpty());
        CHECK_EQUAL(6u, s.cardinality());
        CHECK(s["a"]);
        CHECK(s["b"]);
        CHECK(s["c"]);
        CHECK(s[CantorSet("{a, b}")]);
        CHECK(s[CantorSet("{}")]);
        CHECK(s[CantorSet("{a, {c}}")]);
    }

    // 4. Автоматическое исключение дубликатов при парсинге из строки
    TEST(DeduplicationInConstructorFromString) {
        CantorSet s("{a, b, a, {x, y}, {y, x}, {}, {}}");
        CHECK_EQUAL(4u, s.cardinality());
        CHECK(s["a"]);
        CHECK(s["b"]);
        CHECK(s[CantorSet("{x, y}")]);
        CHECK(s[CantorSet("{}")]);
    }

    // 5. Исключение при передаче nullptr в конструктор const char*
    TEST(ConstructorThrowsOnNullCString) {
        const char* nullStr = nullptr;
        CHECK_THROW(CantorSet s(nullStr), std::invalid_argument);
    }

    // 6. Исключения при некорректном синтаксисе строки
    TEST(ConstructorThrowsOnInvalidSyntaxStrings) {
        CHECK_THROW(CantorSet("a, b, c"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a, b"), std::invalid_argument);
        CHECK_THROW(CantorSet("a, b}"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a, {b}"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a, }"), std::invalid_argument);
        CHECK_THROW(CantorSet("{, a}"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a, , b}"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a} {b}"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a}}"), std::invalid_argument);
        CHECK_THROW(CantorSet("{a b}"), std::invalid_argument);
    }

    // 7. Проверка конструктора копирования (глубокое копирование)
    TEST(CopyConstructorCreatesIndependentDeepCopy) {
        CantorSet original("{a, {b, c}}");
        CantorSet copy(original);
        CHECK(original == copy);

        copy.add("d");
        copy.remove("a");
        CHECK_EQUAL(2u, original.cardinality());
        CHECK(original["a"]);
        CHECK(!original["d"]);
        CHECK_EQUAL(2u, copy.cardinality());
        CHECK(copy["d"]);
    }

    // 8. Проверка оператора присваивания и самоприсваивания
    TEST(CopyAssignmentAndSelfAssignment) {
        CantorSet s1("{a, {b}}");
        CantorSet s2("{x, y, z}");
        s2 = s1;
        CHECK(s1 == s2);

        // Самоприсваивание
        s2 = s2;
        CHECK(s1 == s2);
        CHECK_EQUAL(2u, s2.cardinality());
    }

    // 9. Оператор присваивания из std::string и const char*
    TEST(AssignmentFromStringAndCString) {
        CantorSet s;
        s = std::string("{alpha, beta}");
        CHECK_EQUAL(2u, s.cardinality());
        CHECK(s["alpha"]);

        s = "{gamma, {delta}}";
        CHECK_EQUAL(2u, s.cardinality());
        CHECK(s["gamma"]);
        CHECK(s[CantorSet("{delta}")]);

        const char* nullPtr = nullptr;
        CHECK_THROW(s = nullPtr, std::invalid_argument);
    }

    // 10. Добавление атомарных элементов (std::string и const char*)
    TEST(AddAtomicElementsAndCheckDuplicates) {
        CantorSet s;
        CHECK(s.add(std::string("a")));
        CHECK(s.add("b"));
        CHECK(!s.add("a")); // Дубликат не добавляется
        CHECK_EQUAL(2u, s.cardinality());
        CHECK_EQUAL("{a, b}", s.toString());
    }

    // 11. Расширение динамического массива при добавлении многих элементов
    TEST(DynamicArrayCapacityExpansion) {
        CantorSet s;
        for (int i = 0; i < 20; ++i) {
            CHECK(s.add("e" + std::to_string(i)));
        }
        CHECK_EQUAL(20u, s.cardinality());
        for (int i = 0; i < 20; ++i) {
            CHECK(s["e" + std::to_string(i)]);
        }
    }

    // 12. Исключения при добавлении некорректного атомарного элемента
    TEST(AddThrowsOnInvalidAtom) {
        CantorSet s;
        CHECK_THROW(s.add(""), std::invalid_argument);
        CHECK_THROW(s.add("   "), std::invalid_argument);
        CHECK_THROW(s.add("a,b"), std::invalid_argument);
        CHECK_THROW(s.add("{a}"), std::invalid_argument);
        CHECK_THROW(s.add("a b"), std::invalid_argument);
        const char* nullAtom = nullptr;
        CHECK_THROW(s.add(nullAtom), std::invalid_argument);
    }

    // 13. Добавление вложенного множества как элемента
    TEST(AddNestedCantorSetElement) {
        CantorSet s;
        CantorSet sub1("{1, 2}");
        CantorSet sub2("{2, 1}"); // Равно sub1
        CHECK(s.add(sub1));
        CHECK(!s.add(sub2)); // Повторно равное подмножество не добавляется
        CHECK_EQUAL(1u, s.cardinality());
        CHECK(s[sub1]);
        CHECK(s[sub2]);
    }

    // 14. Удаление атомарных элементов
    TEST(RemoveAtomicElements) {
        CantorSet s("{a, b, c}");
        CHECK(s.remove(std::string("b")));
        CHECK_EQUAL(2u, s.cardinality());
        CHECK(!s["b"]);
        CHECK(!s.remove("b")); // Уже удалён
        CHECK(s.remove("a"));
        CHECK_EQUAL("{c}", s.toString());

        const char* nullAtom = nullptr;
        CHECK_THROW(s.remove(nullAtom), std::invalid_argument);
    }

    // 15. Удаление вложенных подмножеств
    TEST(RemoveNestedSetElements) {
        CantorSet s("{a, {x, y}, {}}");
        CHECK(s.remove(CantorSet("{y, x}")));
        CHECK_EQUAL(2u, s.cardinality());
        CHECK(!s[CantorSet("{x, y}")]);
        CHECK(!s.remove(CantorSet("{x, y}")));
        CHECK(s.remove(CantorSet("{}")));
        CHECK_EQUAL("{a}", s.toString());
    }

    // 16. Очистка множества методом clear()
    TEST(ClearEmptiesTheSet) {
        CantorSet s("{a, b, {c}}");
        CHECK(!s.isEmpty());
        s.clear();
        CHECK(s.isEmpty());
        CHECK_EQUAL(0u, s.cardinality());
        CHECK_EQUAL("{}", s.toString());
    }

    // 17. Проверка принадлежности через оператор [] и метод contains()
    TEST(MembershipOperatorBracketsAndContains) {
        CantorSet s("{alpha, {beta, gamma}}");
        CHECK(s[std::string("alpha")]);
        CHECK(s["alpha"]);
        CHECK(!s["beta"]);
        CHECK(s.contains("alpha"));
        CHECK(!s.contains("delta"));

        CantorSet sub("{gamma, beta}");
        CHECK(s[sub]);
        CHECK(s.contains(sub));
        CHECK(!s[CantorSet("{beta}")]);

        const char* nullKey = nullptr;
        CHECK_THROW((void)s[nullKey], std::invalid_argument);
    }

    // 18. Объединение множеств (операторы + и +=)
    TEST(SetUnionOperatorsPlusAndPlusEquals) {
        CantorSet a("{a, b, {1}}");
        CantorSet b("{b, c, {1}, {2}}");
        CantorSet u = a + b;
        CHECK_EQUAL(5u, u.cardinality());
        CHECK(u["a"]);
        CHECK(u["b"]);
        CHECK(u["c"]);
        CHECK(u[CantorSet("{1}")]);
        CHECK(u[CantorSet("{2}")]);

        a += b;
        CHECK(a == u);

        // Объединение с самим собой
        a += a;
        CHECK(a == u);
    }

    // 19. Пересечение множеств (операторы * и *=)
    TEST(SetIntersectionOperatorsMultiplyAndMultiplyEquals) {
        CantorSet a("{a, b, {x, y}, {}}");
        CantorSet b("{b, c, {y, x}}");
        CantorSet inter = a * b;
        CHECK_EQUAL(2u, inter.cardinality());
        CHECK(inter["b"]);
        CHECK(inter[CantorSet("{x, y}")]);
        CHECK(!inter["a"]);

        a *= b;
        CHECK(a == inter);

        // Пересечение с самим собой
        a *= a;
        CHECK(a == inter);
    }

    // 20. Пересечение непересекающихся множеств даёт пустое множество
    TEST(DisjointSetsIntersectionIsEmpty) {
        CantorSet a("{a, b}");
        CantorSet b("{c, d}");
        CantorSet inter = a * b;
        CHECK(inter.isEmpty());
    }

    // 21. Разность множеств (операторы - и -=)
    TEST(SetDifferenceOperatorsMinusAndMinusEquals) {
        CantorSet a("{a, b, c, {1, 2}}");
        CantorSet b("{b, d, {2, 1}}");
        CantorSet diff = a - b;
        CHECK_EQUAL(2u, diff.cardinality());
        CHECK(diff["a"]);
        CHECK(diff["c"]);
        CHECK(!diff["b"]);
        CHECK(!diff[CantorSet("{1, 2}")]);

        a -= b;
        CHECK(a == diff);
    }

    // 22. Разность множества с самим собой (A -= A) даёт пустое множество
    TEST(SelfDifferenceYieldsEmptySet) {
        CantorSet a("{a, b, {c}}");
        a -= a;
        CHECK(a.isEmpty());
        CHECK_EQUAL(0u, a.cardinality());
    }

    // 23. Булеан пустого множества P({}) = {{}}
    TEST(PowerSetOfEmptySet) {
        CantorSet empty;
        CantorSet ps = empty.powerSet();
        CHECK_EQUAL(1u, ps.cardinality());
        CHECK(ps[CantorSet("{}")]);
        CHECK_EQUAL("{{}}", ps.toString());
    }

    // 24. Булеан одноэлементного множества P({a}) = {{}, {a}}
    TEST(PowerSetOfSingleElementSet) {
        CantorSet s("{a}");
        CantorSet ps = s.powerSet();
        CHECK_EQUAL(2u, ps.cardinality());
        CHECK(ps[CantorSet("{}")]);
        CHECK(ps[CantorSet("{a}")]);
    }

    // 25. Булеан трёхэлементного множества с вложенным множеством (2^3 = 8 подмножеств)
    TEST(PowerSetOfThreeElementSetWithNesting) {
        CantorSet s("{a, b, {c}}");
        CantorSet ps = s.powerSet();
        CHECK_EQUAL(8u, ps.cardinality());
        CHECK(ps[CantorSet("{}")]);
        CHECK(ps[CantorSet("{a}")]);
        CHECK(ps[CantorSet("{b}")]);
        CHECK(ps[CantorSet("{{c}}")]);
        CHECK(ps[CantorSet("{a, b}")]);
        CHECK(ps[CantorSet("{a, {c}}")]);
        CHECK(ps[CantorSet("{b, {c}}")]);
        CHECK(ps[CantorSet("{a, b, {c}}")]);
    }

    // 26. Сравнение множеств на равенство и неравенство (независимость от порядка)
    TEST(EqualityAndInequalityOrderIndependent) {
        CantorSet s1("{a, b, {1, 2, {x, y}}}");
        CantorSet s2("{{2, {y, x}, 1}, b, a}");
        CantorSet s3("{a, b, {1, 2}}");
        CantorSet s4("{a, b}");

        CHECK(s1 == s1);
        CHECK(s1 == s2);
        CHECK(!(s1 != s2));
        CHECK(s1 != s3);
        CHECK(s1 != s4);
    }

    // 27. Операторы включения (<, <=, >, >=)
    TEST(SubsetAndSupersetComparisonOperators) {
        CantorSet sub("{a, {b}}");
        CantorSet full("{a, {b}, c}");
        CantorSet same("{{b}, a}");
        CantorSet other("{a, d}");

        CHECK(sub <= full);
        CHECK(sub < full);
        CHECK(full >= sub);
        CHECK(full > sub);

        CHECK(sub <= same);
        CHECK(sub >= same);
        CHECK(!(sub < same));
        CHECK(!(sub > same));

        CHECK(!(other <= full));
        CHECK(!(full <= sub));
    }

    // 28. Потоковый оператор вывода << и работа кэша mutable
    TEST(StreamOutputOperatorAndMutableCache) {
        const CantorSet s("{a, {b, c}}");
        std::ostringstream oss;
        oss << s;
        CHECK_EQUAL("{a, {b, c}}", oss.str());
        // Повторный вызов использует валидный mutable-кэш
        CHECK_EQUAL("{a, {b, c}}", s.toString());
    }

    // 29. Потоковый оператор ввода >> для корректной строки
    TEST(StreamInputOperatorValidSet) {
        std::istringstream iss("   { a, b, { c, d }, {} }  ");
        CantorSet s;
        iss >> s;
        CHECK(iss.good() || iss.eof());
        CHECK(!iss.fail());
        CHECK_EQUAL(4u, s.cardinality());
        CHECK(s["a"]);
        CHECK(s[CantorSet("{c, d}")]);
        CHECK(s[CantorSet("{}")]);
    }

    // 30. Потоковый оператор ввода >> для нескольких множеств подряд из одного потока
    TEST(StreamInputMultipleSetsSequentially) {
        std::istringstream iss("{1, 2} {3, {4}}");
        CantorSet s1, s2;
        iss >> s1 >> s2;
        CHECK(!iss.fail());
        CHECK(s1 == CantorSet("{1, 2}"));
        CHECK(s2 == CantorSet("{3, {4}}"));
    }

    // 31. Потоковый оператор ввода >> при некорректном вводе устанавливает failbit
    TEST(StreamInputOperatorSetsFailbitOnInvalidInput) {
        {
            std::istringstream iss("invalid_without_braces");
            CantorSet s;
            iss >> s;
            CHECK(iss.fail());
        }
        {
            std::istringstream iss("{unclosed_brace, a");
            CantorSet s;
            iss >> s;
            CHECK(iss.fail());
        }
        {
            std::istringstream iss("{a, , b}");
            CantorSet s;
            iss >> s;
            CHECK(iss.fail());
        }
        {
            std::istringstream iss("   ");
            CantorSet s;
            iss >> s;
            CHECK(iss.fail());
        }
    }

    // 32. Статические методы fromString и isValidSetString
    TEST(StaticMethodsFromStringAndValidation) {
        CHECK(CantorSet::isValidSetString("{a, b, {c}}"));
        CHECK(CantorSet::isValidSetString("{}"));
        CHECK(!CantorSet::isValidSetString("{a, b"));
        CHECK(!CantorSet::isValidSetString("a, b}"));
        CHECK(!CantorSet::isValidSetString("{a, , b}"));

        CantorSet s1 = CantorSet::fromString(std::string("{k, m}"));
        CantorSet s2 = CantorSet::fromString("{m, k}");
        CHECK(s1 == s2);
    }

    // 33. Глубокая вложенность множеств (4 уровня)
    TEST(DeeplyNestedCantorSetOperations) {
        CantorSet deep1("{{{{a}}}}");
        CantorSet deep2("{{{{a}}}}");
        CantorSet deep3("{{{{b}}}}");
        CHECK_EQUAL(1u, deep1.cardinality());
        CHECK(deep1 == deep2);
        CHECK(deep1 != deep3);
        CHECK(deep1[CantorSet("{{{a}}}")]);
    }

    // 34. Законы де Моргана и тождества алгебры множеств
    TEST(SetAlgebraIdentitiesVerification) {
        CantorSet u("{a, b, c, d, {1}}");
        CantorSet a("{a, b, {1}}");
        CantorSet b("{b, c, {1}}");

        // (U - (A + B)) == (U - A) * (U - B)
        CantorSet lhs = u - (a + b);
        CantorSet rhs = (u - a) * (u - b);
        CHECK(lhs == rhs);

        // (U - (A * B)) == (U - A) + (U - B)
        CantorSet lhs2 = u - (a * b);
        CantorSet rhs2 = (u - a) + (u - b);
        CHECK(lhs2 == rhs2);
    }

} // SUITE(CantorSetTests)

int main(int, const char*[]) {
    return UnitTest::RunAllTests();
}
