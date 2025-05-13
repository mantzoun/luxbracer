# LuxBracer

SRCDIR = src
INCDIR = include
EXTDIR = ext
OBJDIR = obj
BINDIR = bin
DOCDIR = doc
TSTDIR = test
COVERAGEDIR = coverage

DOX = doxygen
DOXYFILE = Doxyfile

CC = g++
CFLAGS = -Wall -g -std=c++20 -Wno-psabi -O0 -fprofile-arcs -ftest-coverage
INCLUDES = -I$(INCDIR) \
           -I$(EXTDIR) \

LIB = -ldpp \
      -lgcov \

FILES = main.cpp discord_bot.cpp discord_channel.cpp discord_guild.cpp discord_commands.cpp lux_logger.cpp planet.cpp system.cpp engine.cpp

TEST_FILES = test_main.cpp test_guild.cpp test_engine.cpp

SRC = $(addprefix $(SRCDIR)/,$(FILES))
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
BIN = $(BINDIR)/luxbot

TST = $(addprefix $(TSTDIR)/,$(TEST_FILES))
TSTOBJ = $(patsubst $(TSTDIR)/%.cpp,$(OBJDIR)/%.o, $(TST))
TSTBIN = $(BINDIR)/test

.PHONY: all clean dox test test_ci

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
	@cpplint --linelength=120 --filter=-build/include_subdir,-build/c++11 $(SRC) $(INC)

cppcheck:
	@cppcheck $(SRC) --force --enable=all --inconclusive --error-exitcode=1 --suppress=missingIncludeSystem --suppress=missingInclude -I$(INCDIR) $(EXTDIR)

$(TSTOBJ): $(OBJDIR)/%.o: $(TSTDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(TSTBIN): $(TSTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ)) | $(BINDIR)
	$(CC) -o $(TSTBIN) $(TSTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ)) $(LIB)

test_ci: $(TSTBIN)
	bin/test --reporter junit --out results.xml

test: $(TSTBIN)
	bin/test

coverage: test
	@rm -rf ./$(COVERAGEDIR)
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/test/test_*' '*/ext/*' --output-file coverage_clean.info
	genhtml coverage_clean.info --output-directory $(COVERAGEDIR)


runner:
	@docker build -t github:latest -f cicd/github-runner.Dockerfile .
