CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -Ithird_party
COVFLAGS := --coverage -O0 -g

BUILD_DIR := build
APP_BIN   := $(BUILD_DIR)/cantor_set_app
TEST_BIN  := $(BUILD_DIR)/cantor_set_tests

.PHONY: all app test coverage docs clean

all: app test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

app: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) src/CantorSet.cpp src/main.cpp -o $(APP_BIN)

test: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) src/CantorSet.cpp tests/test_cantor_set.cpp -o $(TEST_BIN)
	./$(TEST_BIN)

coverage: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) src/CantorSet.cpp tests/test_cantor_set.cpp -o $(TEST_BIN)_cov
	./$(TEST_BIN)_cov
	gcov -o $(BUILD_DIR) $(TEST_BIN)_cov-CantorSet.gcno

docs:
	doxygen Doxyfile

clean:
	rm -rf $(BUILD_DIR) *.gcov *.gcda *.gcno
