COMPILER := gcc
FLAGS := -lm

EXECUTABLE := dist/main

SRCS := src
SOURCES := $(wildcard $(SRCS)/*.c)

OBJS := 'objs'
OBJECTS := $(patsubst $(SRCS)/%, $(OBJS)/%, $(SOURCES:.c=.o))

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) -o $@ $^ $(FLAGS)

$(OBJS)/%.o: $(SRCS)/%.c
	$(COMPILER) -c $< -o $@

clear:
	rm -f $(OBJECTS) $(EXECUTABLE)

test: $(EXECUTABLE)
	$< 1000000