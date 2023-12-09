CXX = clang++
CXXFLAGS = -std=c++17

TASKS = Task1 Task2 Task3

.PHONY: all clean $(TASKS) run

all: 
	@echo "Building..."
	@$(MAKE) $(TASKS)

$(TASKS):
	@find $@ -name "*.cpp" | xargs $(CXX) $(CXXFLAGS) -o $@/$$(echo $@ | tr A-Z a-z)

run:
	@./$(filter-out $@,$(MAKECMDGOALS))/$(shell echo $(filter-out $@,$(MAKECMDGOALS)) | tr A-Z a-z)

clean:
	@for task in $(TASKS); do \
		rm -f $$task/$$(echo $$task | tr A-Z a-z); \
	done