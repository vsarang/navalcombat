EXENAME = navalcombat

COMPILER = g++
WARNINGS = -Wchar-subscripts -Wparentheses -Wreturn-type -Wmissing-braces -Wundef -Wshadow
COMPILER_OPTS = -c -g -O0 -Wfatal-errors -Werror $(WARNINGS)
LSDL = -lSDL -lSDL_image

all: $(EXENAME)

navalcombat: main.o battlefieldgui.o battlefield.o warship.o warship_corvette.o timer.o
	$(COMPILER) -o $@ $^ $(LSDL)

main.o: main.cpp battlefieldgui.o warship.o battlefield.o
	$(COMPILER) $(COMPILER_OPTS) $<

battlefieldgui.o: battlefieldgui.cpp battlefieldgui.h battlefield.o warship.o timer.o
	$(COMPILER) $(COMPILER_OPTS) $<

battlefield.o: battlefield.cpp battlefield.h warship.o
	$(COMPILER) $(COMPILER_OPTS) $<

warship.o: warship.cpp warship.h
	$(COMPILER) $(COMPILER_OPTS) $<

warship_corvette.o: warship_corvette.cpp warship_corvette.h warship.o
	$(COMPILER) $(COMPILER_OPTS) $<

timer.o: timer.cpp timer.h
	$(COMPILER) $(COMPILER_OPTS) $<

clean:
	-rm -f *.o *.h.gch $(EXENAME)
