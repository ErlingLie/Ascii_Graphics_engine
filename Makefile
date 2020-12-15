SOURCE_DIR := source
BUILD_DIR := build

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))


CXX := g++
CXXFLAGS := -O0 -g3 -Wall -Werror -I$(SOURCE_DIR)
LDFLAGS := -L$(BUILD_DIR)

.DEFAULT_GOAL := executable

executable : $(OBJ)
	@echo $(SOURCES)
	@echo $^
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR) :
	mkdir -p $@

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@



.PHONY: clean
clean :
	rm -rf $(BUILD_DIR) executable