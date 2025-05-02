# LuxBracer

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DOCDIR = doc
TSTDIR = test

DOX = doxygen
DOXYFILE = Doxyfile

CC = g++
CFLAGS = -Wall -g -std=c++20 -Wno-psabi
INCLUDES = -I$(INCDIR) \

LIB = -ldpp \
#      -pthread \

FILES = main.cpp discord_bot.cpp discord_channel.cpp discord_guild.cpp discord_commands.cpp lux_logger.cpp planet.cpp system.cpp engine.cpp

TEST_FILES = test_main.cpp test_guild.cpp

SRC = $(addprefix $(SRCDIR)/,$(FILES))
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
BIN = $(BINDIR)/luxbot

TST = $(addprefix $(TSTDIR)/,$(TEST_FILES))
TSTOBJ = $(patsubst $(TSTDIR)/%.cpp,$(OBJDIR)/%.o, $(TST))
TSTBIN = $(BINDIR)/test

.PHONY: all clean dox test

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
	@cpplint $(SRC) $(INCDIR)/*

cppcheck:
	@cppcheck $(SRC) --enable=all --inconclusive --error-exitcode=1 --suppress=missingIncludeSystem --suppress=missingInclude -I$(INCDIR)

$(TSTOBJ): $(OBJDIR)/%.o: $(TSTDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(TSTBIN): $(TSTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ)) | $(BINDIR)
	$(CC) -o $(TSTBIN) $(TSTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ)) $(LIB)

test: $(TSTBIN)

runner:
	@docker build -t github:latest -f cicd/github-runner.Dockerfile .
