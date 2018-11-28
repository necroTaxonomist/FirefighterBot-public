
TARGET = ff_bot.out

CC = g++
CFLAGS = --std=c++11 -g
LFLAGS = -pthread -lwiringPi

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
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BINDIR)/$@ $^ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/*.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
