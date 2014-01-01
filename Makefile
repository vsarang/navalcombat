EXENAME = navalcombat

COMPILER = g++
WARNINGS = -Wchar-subscripts -Wparentheses -Wreturn-type -Wmissing-braces -Wundef -Wshadow
COMPILER_OPTS = -c -g -O0 -Wfatal-errors -Werror $(WARNINGS)
LSDL = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

all: $(EXENAME)

navalcombat: main.o battlefieldgui.o battlefield.o  warship.o
	$(COMPILER) -o $@ $^ $(LSDL)

main.o: main.cpp warship.o battlefield.o
	$(COMPILER) $(COMPILER_OPTS) $<

battlefieldgui.o: battlefieldgui.cpp battlefieldgui.h battlefield.o warship.o
	$(COMPILER) $(COMPILER_OPTS) $<

battlefield.o: battlefield.cpp battlefield.h warship.o
	$(COMPILER) $(COMPILER_OPTS) $<

warship.o: warship.cpp warship.h
	$(COMPILER) $(COMPILER_OPTS) $<

clean:
	-rm -f *.o *.h.gch $(EXENAME)
