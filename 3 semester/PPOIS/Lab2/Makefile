CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -Ithird_party
COVFLAGS := --coverage -O0 -g

BUILD_DIR := build
SRCS := src/MuseumExceptions.cpp \
        src/Artifacts.cpp \
        src/HallsAndClimate.cpp \
        src/StorageAndLogistics.cpp \
        src/ExhibitionsAndEvents.cpp \
        src/VisitorsAndTickets.cpp \
        src/StaffAndHR.cpp \
        src/RestorationLab.cpp \
        src/SecuritySystem.cpp \
        src/FinanceAndShop.cpp

APP_BIN  := $(BUILD_DIR)/museum_app
TEST_BIN := $(BUILD_DIR)/museum_tests

.PHONY: all app test coverage docs clean

all: app test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

app: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) src/main.cpp -o $(APP_BIN)

test: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) tests/test_museum.cpp -o $(TEST_BIN)
	./$(TEST_BIN)

coverage: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(SRCS) tests/test_museum.cpp -o $(TEST_BIN)_cov
	./$(TEST_BIN)_cov
	gcov -o $(BUILD_DIR) $(SRCS)

docs:
	doxygen Doxyfile

clean:
	rm -rf $(BUILD_DIR) *.gcov *.gcda *.gcno
