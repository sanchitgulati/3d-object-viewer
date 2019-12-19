ifeq ($(OS),Windows_NT)
	LIBS += -lGL -lGLU
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		LIBS += -lGL -lGLU
    endif
    ifeq ($(UNAME_S),Darwin)
		LINKS += -L/usr/X11/lib
        LIBS += -framework OpenGL
	endif
endif


# Directories variables
INCLUDE_DIR = ./src/include
SOURCE_DIR= ./src/cpp
SOURCE_FILES=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJECT_DIR= ./obj
OBJECT_FILES=$(patsubst $(SOURCE_DIR)/%.cpp,$(OBJECT_DIR)/%.o,$(SOURCE_FILES))
BINARY_DIR= ./bin
DATA_DIR= ./data
LDFLAGS = -lSDL -lSDL_Image -lglui -lglut

# Compiling related variables
CC = g++
CFLAGS = -I$(INCLUDE_DIR) -std=c++14
FLAGS = -I$(INCLUDE_DIR) -std=c++14

main: clean
	g++ $(CFLAGS) $(LDFLAGS) $(SOURCE_FILES) $(LIBS) -o $(BINARY_DIR)/main.out
	$(BINARY_DIR)/main.out -f ./data/12140_Skull_v3_L2.obj -vs ./shaders/lambertian.vs -fs ./shaders/lambertian.fs 

.PHONY: clean

clean:
	$(RM) $(BINARY_DIR)/main

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
