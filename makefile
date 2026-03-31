CC = g++
CFLAGS = -std=c++23 \


INCLUDE = -I/usr/include -I/usr/include/GL -I/usr/include/glm -I/usr/local/include
LIBS = -lglfw -lGLEW -lGL -lpugixml -lm -ldl -pthread


SOURCES = main.cpp src/graphics.cpp src/utils.cpp src/loader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
BINFILE = prgm.x86_64


all: release

release: CFLAGS += -O2 -ffast-math
release: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) $(INCLUDE) -o $(BINFILE)

debug: CFLAGS += -g -DDEBUG
debug: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) $(INCLUDE) -o $(BINFILE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(BINFILE)

