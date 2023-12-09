CXX = clang++
CXXFLAGS = -std=c++17

# Define the source directories
TASK1_SRC_DIR = Task1
TASK2_SRC_DIR = Task2
TASK3_SRC_DIR = Task3

# Define the build directories
BUILD_DIR = build
TASK1_BUILD_DIR = $(BUILD_DIR)/Task1
TASK2_BUILD_DIR = $(BUILD_DIR)/Task2
TASK3_BUILD_DIR = $(BUILD_DIR)/Task3

# Define the source files
TASK1_SRC = $(wildcard $(TASK1_SRC_DIR)/*.cpp)
TASK2_SRC = $(wildcard $(TASK2_SRC_DIR)/*.cpp) $(wildcard $(TASK2_SRC_DIR)/simpletest/*.cpp)
TASK3_SRC = $(wildcard $(TASK3_SRC_DIR)/*.cpp)

# Define the executable names
TASK1_EXEC = $(BUILD_DIR)/Task1/executable_name
TASK2_EXEC = $(BUILD_DIR)/Task2/executable_name
TASK3_EXEC = $(BUILD_DIR)/Task3/executable_name

.PHONY: all clean run

all: $(TASK1_EXEC) $(TASK2_EXEC) $(TASK3_EXEC)

$(TASK1_EXEC): $(TASK1_SRC) | $(TASK1_BUILD_DIR)
	@echo "Compiling and linking $@"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(TASK2_EXEC): $(TASK2_SRC) | $(TASK2_BUILD_DIR)
	@echo "Compiling and linking $@"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(TASK3_EXEC): $(TASK3_SRC) | $(TASK3_BUILD_DIR)
	@echo "Compiling and linking $@"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR) $(TASK1_BUILD_DIR) $(TASK2_BUILD_DIR) $(TASK3_BUILD_DIR):
	@mkdir -p $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

run: all
	@echo "Usage: make run TASK=X (X is the task number)"
	@echo "Running task $(TASK)"
	@./$(BUILD_DIR)/Task$(TASK)/executable_name
