# Makefile
#
# $Id:$
#
# $Log:$
#

CXXFILES   = mmrp1210.cxx

HFILES     = mmrp1210.h

RCFILES    =

RHFILES    =

PYFILES    = 

OBJFILES   := $(CXXFILES:.cxx=.o) $(RCFILES:.rc=.o)

.PHONY : all
all : mmrp1210.dll

.PHONY : clean
clean :
	rm -f mmrp1210.dll
	rm -f *.o

.PHONY : checkout
checkout :
	for f in $(CXXFILES) $(HFILES) $(RCFILES) $(RHFILES) $(PYFILES); do \
	  co $$f RCS/$$f.rcs; \
	done

mmrp1210.dll : $(OBJFILES)
	g++ -shared -Wl,--add-stdcall-alias -o mmrp1210.dll $(OBJFILES) -Wl,--out-implib,libmmrp1210.a

resource.o : resource.rc
	windres resource.rc $*.o

%.o : %.cxx
	g++ -c -Wno-deprecated -Wno-invalid-offsetof -Wno-write-strings $< -o $@
