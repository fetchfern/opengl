CC = clang

INCLUDE += -Iinclude
INCLUDE += -Isrc

CCFLAGS = -std=c++20 -O1 -g -Wall -Wextra -Wpedantic -Wno-c99-extensions
CCFLAGS += $(shell pkg-config --cflags "glfw3 >= 3.0.0")
CCFLAGS += $(shell pkg-config --cflags "opengl >= 3.3")
CCFLAGS += $(INCLUDE)

LDFLAGS = -lm -lstdc++
LDFLAGS += $(shell pkg-config --static --libs "glfw3 >= 3.0.0")
LDFLAGS += $(shell pkg-config --static --libs "opengl >= 3.3")
LDFLAGS += $(INCLUDE)

BUILD = build
SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:.cpp=.o)

run: build
	$(BUILD)/minecraft

build: $(OBJ)
	$(CC) -o $(BUILD)/minecraft $(filter %.o,$^) $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CCFLAGS)

clean:
	@rm -r $(BUILD) $(OBJ)
	@mkdir build
