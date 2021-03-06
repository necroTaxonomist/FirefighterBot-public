
TARGET = ff_bot.out

CC = g++
CFLAGS = --std=c++14 -O2 -g
LFLAGS = -pthread
CAMFLAGS = $(shell pkg-config --libs --cflags opencv)
PIFLAGS = -lwiringPi $(CAMFLAGS)
#PIFLAGS = -D NO_PI

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = .

SRC := $(wildcard $(SRCDIR)/*.cpp)
INC := $(wildcard $(INCDIR)/*.h)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

INCFLAGS := -iquote $(INCDIR)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BINDIR)/$@ $^ $(LFLAGS) $(PIFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/*.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ $< $(LFLAGS) $(PIFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
