# Target application.
TARGET := frame_test

# C++ compiler.
CXX_COMPILER := g++

# Source files.
SRC_FILES := source/comm/frame/frame.cpp \
             source/comm/frame/parser.cpp \

# Test files.
TST_FILES := test/main.cpp \
             test/comm/frame/frame_test.cpp \
			 test/comm/frame/parser_test.cpp \

# All files.
ALL_FILES := $(SRC_FILES) $(TST_FILES)

# Include main header directory.
INC_DIR := -Iinclude

# Include Google Test directory.
GTEST_DIR := -I/usr/include

# C++ flags.
CXX_FLAGS := -Wall -Werror -std=c++17 $(INC_DIR) $(GTEST_DIR)

# Linked libraries.
LINK_LIBS := -lgtest -lpthread

# Build and run the application as default.
default: build run

# Build the application.
build:
	@$(CXX_COMPILER) $(ALL_FILES) -o $(TARGET) $(CXX_FLAGS) $(LINK_LIBS)

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)
