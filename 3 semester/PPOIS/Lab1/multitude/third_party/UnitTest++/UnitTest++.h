/**
 * @file UnitTest++.h
 * @brief Lightweight header-only implementation compatible with UnitTest++ API.
 * @details Provides SUITE, TEST, CHECK, CHECK_EQUAL, CHECK_CLOSE, CHECK_THROW,
 * and UnitTest::RunAllTests() so the test suite compiles out-of-the-box when
 * the system-wide libunittest++-dev package is not installed. If system UnitTest++
 * is installed, CMake/Make can also link against the system library directly.
 */

#ifndef UNITTESTCPP_H
#define UNITTESTCPP_H

#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace UnitTest {

struct TestCase {
    std::string suite;
    std::string name;
    std::function<void()> func;
};

inline std::vector<TestCase>& GetRegistry() {
    static std::vector<TestCase> registry;
    return registry;
}

inline int& CurrentTestFailures() {
    static int failures = 0;
    return failures;
}

struct TestRegistrar {
    TestRegistrar(const char* suite, const char* name, std::function<void()> func) {
        GetRegistry().push_back(TestCase{suite, name, func});
    }
};

inline void ReportFailure(const char* file, int line, const std::string& message) {
    ++CurrentTestFailures();
    std::cerr << file << ":" << line << ": error: Failure in test: " << message << "\n";
}

template <typename Expected, typename Actual>
inline void CheckEqual(const Expected& expected, const Actual& actual,
                       const char* file, int line) {
    if (!(expected == actual)) {
        std::ostringstream oss;
        oss << "Expected [" << expected << "] but was [" << actual << "]";
        ReportFailure(file, line, oss.str());
    }
}

inline void CheckEqual(const char* expected, const std::string& actual,
                       const char* file, int line) {
    CheckEqual(std::string(expected), actual, file, line);
}

inline void CheckEqual(const std::string& expected, const char* actual,
                       const char* file, int line) {
    CheckEqual(expected, std::string(actual), file, line);
}

inline void CheckEqual(const char* expected, const char* actual,
                       const char* file, int line) {
    CheckEqual(std::string(expected), std::string(actual), file, line);
}

inline int RunAllTests() {
    int passed = 0;
    int failed = 0;
    const auto& tests = GetRegistry();

    for (const auto& test : tests) {
        int beforeFailures = CurrentTestFailures();
        try {
            test.func();
        } catch (const std::exception& ex) {
            ReportFailure(__FILE__, __LINE__,
                          std::string("Unhandled exception in ") + test.name + ": " + ex.what());
        } catch (...) {
            ReportFailure(__FILE__, __LINE__,
                          std::string("Unhandled unknown exception in ") + test.name);
        }
        if (CurrentTestFailures() == beforeFailures) {
            ++passed;
        } else {
            ++failed;
        }
    }

    if (failed == 0) {
        std::cout << "Success: " << passed << " tests passed.\n";
        return 0;
    } else {
        std::cout << "FAILURE: " << failed << " out of " << tests.size()
                  << " tests failed (" << CurrentTestFailures() << " failures).\n";
        return 1;
    }
}

} // namespace UnitTest

#define SUITE(SuiteName) namespace Suite_##SuiteName

#define TEST(TestName)                                                                 \
    static void Test_##TestName();                                                     \
    static ::UnitTest::TestRegistrar Registrar_##TestName(                             \
        "Suite", #TestName, &Test_##TestName);                                         \
    static void Test_##TestName()

#define CHECK(value)                                                                   \
    do {                                                                               \
        if (!(value)) {                                                                \
            ::UnitTest::ReportFailure(__FILE__, __LINE__, "CHECK(" #value ") failed"); \
        }                                                                              \
    } while (0)

#define CHECK_EQUAL(expected, actual)                                                  \
    do {                                                                               \
        ::UnitTest::CheckEqual((expected), (actual), __FILE__, __LINE__);              \
    } while (0)

#define CHECK_CLOSE(expected, actual, tolerance)                                       \
    do {                                                                               \
        if (std::fabs(static_cast<double>(expected) - static_cast<double>(actual)) >   \
            static_cast<double>(tolerance)) {                                          \
            ::UnitTest::ReportFailure(__FILE__, __LINE__,                              \
                                      "CHECK_CLOSE(" #expected ", " #actual ") failed");\
        }                                                                              \
    } while (0)

#define CHECK_THROW(expression, ExpectedExceptionType)                                 \
    do {                                                                               \
        bool caughtExpected = false;                                                   \
        try {                                                                          \
            expression;                                                                \
        } catch (const ExpectedExceptionType&) {                                       \
            caughtExpected = true;                                                     \
        } catch (...) {                                                                \
        }                                                                              \
        if (!caughtExpected) {                                                         \
            ::UnitTest::ReportFailure(__FILE__, __LINE__,                              \
                                      "Expected exception " #ExpectedExceptionType     \
                                      " not thrown by " #expression);                  \
        }                                                                              \
    } while (0)

#endif // UNITTESTCPP_H
