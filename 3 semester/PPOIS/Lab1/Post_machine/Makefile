CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -Ithird_party
COVFLAGS := --coverage -O0 -g

BUILD_DIR := build
SRCS      := src/PostTape.cpp src/PostRule.cpp src/PostProgram.cpp src/PostMachine.cpp
APP_BIN   := $(BUILD_DIR)/post_machine_app
TEST_BIN  := $(BUILD_DIR)/post_machine_tests

.PHONY: all app test coverage docs clean

all: app test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

app: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) src/main.cpp -o $(APP_BIN)

test: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) tests/test_post_machine.cpp -o $(TEST_BIN)
	./$(TEST_BIN)

coverage: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(SRCS) tests/test_post_machine.cpp -o $(TEST_BIN)_cov
	./$(TEST_BIN)_cov
	gcov -o $(BUILD_DIR) \
		$(TEST_BIN)_cov-PostTape.gcno \
		$(TEST_BIN)_cov-PostRule.gcno \
		$(TEST_BIN)_cov-PostProgram.gcno \
		$(TEST_BIN)_cov-PostMachine.gcno

docs:
	doxygen Doxyfile

clean:
	rm -rf $(BUILD_DIR) *.gcov *.gcda *.gcno
