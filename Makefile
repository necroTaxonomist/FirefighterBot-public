
TARGET = ff_bot

CC = g++
CFLAGS = --std=c++11 -g
LFLAGS =

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = .

SRC := $(wildcard $(SRCDIR)/*.cpp)
INC := $(wildcard $(INCDIR)/*.h)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

INCFLAGS := $(patsubst %/,-I%,$(dir $(wildcard $(INCDIR)/.)))

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -o $(BINDIR)/$@ $^ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
