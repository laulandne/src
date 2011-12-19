
LDNAME=libnemu_libs_static.a

####################################################################
# Makefile
####################################################################

PATH_TO_BASE=.

include $(PATH_TO_BASE)/Makefile.target
#CROSS=arm-apple-darwin9
#TARGET_PLATFORM=ASYS_COCOA
include $(PATH_TO_BASE)/Makefile.common

EXTRA_INCS=-I$(PATH_TO_BASE) -I../alib -I../alib -I../cpp
EXTRA_LIBS=-L$(PATH_TO_BASE) -L../alib

#EXTRA_FLAGS=-march=armv6 -mcpu=arm1176jzf-s
#LFRAMEWORKS=-framework CoreFoundation -framework Foundation -framework UIKit -lobjc


####################################################################
# Our objects 
####################################################################

DIRS=$(wildcard */)


####################################################################
# The libraries and programs 
####################################################################

SIMPLE_PROGRAMS1=
SIMPLE_PROGRAMS=$(SIMPLE_PROGRAMS1) $(SIMPLE_PROGRAMS2)
PROGRAMS=$(SIMPLE_PROGRAMS) nemu

BASE_PATHS=$(ELIB_PATH)
OTHER_PATHS=$(CPULIB_PATH) $(DISKLIB_PATH) $(EMU_PATH) $(BINARYLIB_PATH)
OTHER_L=$(EMU_L) $(DISKLIB_L) $(CPULIB_L) $(BINARYLIB_L)
BASE_L=$(ELIB_L)


####################################################################
# The targets 
####################################################################

all:
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) all;)
	make -i $(LDNAME)
# Programs...
	$(MAKE) -i $(PROGRAMS)


clean:
	rm -rf *.o *.obj *.lib *.a *.s *.bak *.log map.f build *Data *.pdb *.lib
	rm -rf $(PROGRAMS) *.html *.exe *.exe.so *68k *.SYM .DS_Store *.ilk *.app *.class
	rm -rf .gdb_history .nautilus-metafile.xml */*Data */Debug */*.ncb */*/Debug Debug
	rm -rf ._* */._* */*/._* */*/*/._*
	rm -rf *~ */*~ */*/*~ */*/*/*~
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) clean;)


fix: fix_perm fix_type
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) fix;)


# The different Libraries and Programs...

$(LDNAME):
	$(AR) rcv $(LDNAME) */*.o
	$(RANLIB) $(LDNAME)

nemu: nemu.$(OFILE) ncmd.$(OFILE) $(LDNAME)
	$(CLXX) $(LDFLAGS) -o nemu nemu.$(OFILE) ncmd.$(OFILE) $(OTHER_L) $(BASE_L) $(LIBS) $(LDNAME) ../cpp/liball_libs_static.a
ifeq ($(BUILD_BUNDLE),yes)
	make nemu.app
endif

# dependencies
# NOTE: its ok to see errors when running makedepend
depend: remove_depends
	makedepend $(DEPENDFLAGS) *.cpp
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) depend;)


remove_depends:
	rm -f Makefile.dep
	touch Makefile.dep
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -ik -C $(DIR) remove_depends;)


include $(PATH_TO_BASE)/Makefile.tail

include Makefile.dep


