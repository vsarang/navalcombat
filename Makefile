EXENAME = navalcombat

COMPILER = g++
WARNINGS = -Wchar-subscripts -Wparentheses -Wreturn-type -Wmissing-braces -Wundef -Wshadow
COMPILER_OPTS = -c -g -O0 -Wfatal-errors -Werror -lSDL_image -lSDL_ttf -lSDL_mixer $(WARNINGS)

all: $(EXENAME)

navalcombat: main.o battlefield.o warship.o
	$(COMPILER) -o $@ $^

main.o: main.cpp warship.h warship.cpp battlefield.o
	$(COMPILER) $(COMPILER_OPTS) $<

battlefield.o: battlefield.cpp battlefield.h warship.h warship.cpp
	$(COMPILER) $(COMPILER_OPTS) $<

warship.o: warship.cpp warship.h
	$(COMPILER) $(COMPILER_OPTS) $<

clean:
	-rm -f *.o *.h.gch $(EXENAME)
