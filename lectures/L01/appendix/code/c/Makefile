TARGET := frame_demo

C_COMPILER := gcc

C_FLAGS := -Wall -Werror -std=c99 -Iinclude

SOURCE_FILES := source/main.c \
                source/comm/frame/frame.c \

default: build run

build:
	@$(C_COMPILER) $(SOURCE_FILES) -o $(TARGET) $(C_FLAGS)

run:
	@./$(TARGET)

clean:
	@rm -f $(TARGET)

