COMPILER = g++
WARNINGS = -Wchar-subscripts -Wparentheses -Wreturn-type -Wmissing-braces -Wundef -Wshadow
COMPILER_OPTS = -c -g -O0 -Wfatal-errors -Werror -lSDL_image -lSDL_ttf -lSDL_mixer $(WARNINGS)

all: navalcombat

navalcombat: main.o
	$(COMPILER) -o $@ $^

main.o: main.cpp warship.o
	$(COMPILER) $(COMPILER_OPTS) $<

warship.o: warship.cpp warship.h
	$(COMPILER) $(COMPILER_OPTS) $<
