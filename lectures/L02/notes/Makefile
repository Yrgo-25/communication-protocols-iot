# Target application.
TARGET := frame_test

# C++ compiler.
CXX_COMPILER := g++

# Source files.
SOURCE_FILES := source/comm/frame/frame.cpp \
                source/comm/frame/parser.cpp \
                source/main.cpp \

# Include directory.
INCLUDE_DIR := -Iinclude

# C++ flags.
CXX_FLAGS := -Wall -Werror -std=c++17 $(INCLUDE_DIR)

# Build and run the application as default.
default: build run

# Build the application.
build:
	@$(CXX_COMPILER) $(SOURCE_FILES) -o $(TARGET) $(CXX_FLAGS)

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)
