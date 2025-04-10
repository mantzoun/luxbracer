# LuxBracer

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DOCDIR = doc

DOX = doxygen
DOXYFILE = Doxyfile

CC = g++
CFLAGS = -Wall -g -std=c++20 -Wno-psabi
INCLUDES = -I$(INCDIR) \

LIB = -ldpp \
#      -pthread \

FILES = main.cpp discord_bot.cpp discord_channel.cpp discord_guild.cpp lux_logger.cpp planet.cpp system.cpp engine.cpp

SRC = $(addprefix $(SRCDIR)/,$(FILES))
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
BIN = $(BINDIR)/luxbot

.PHONY: all clean dox

all: $(BIN)

$(BIN): $(OBJ) | $(BINDIR)
	$(CC) -o $(BIN) $(OBJ) $(LIB)

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(DOCDIR):
	@mkdir -p $(DOCDIR)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

dox:  | $(DOCDIR)
	@rm -rf $(DOCDIR)
	$(DOX) $(DOXYFILE)

cpplint:
	@cpplint $(SRCDIR)/* $(INCDIR)/*

cppcheck:
	@cppcheck $(SRCDIR) --enable=all --inconclusive --suppress=missingIncludeSystem --suppress=missingInclude -I$(INCDIR)

test:

