
LDNAME=libportable_static.a

####################################################################
# Makefile
####################################################################

PATH_TO_BASE=..

include $(PATH_TO_BASE)/Makefile.target
#CROSS=arm-apple-darwin9
#TARGET_PLATFORM=ASYS_COCOA
include $(PATH_TO_BASE)/Makefile.common

EXTRA_INCS=-I$(PATH_TO_BASE)
EXTRA_LIBS=-L$(PATH_TO_BASE)

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
PROGRAMS=$(SIMPLE_PROGRAMS)

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
#	$(MAKE) -i $(PROGRAMS)


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


install: all
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) install;)


# The different Libraries and Programs...

$(LDNAME):
	$(AR_CMD) */*.o
	$(RANLIB_CMD) $(LDNAME)

nlines:  nlines.$(OFILE)
	$(CLXX) $(LDFLAGS) -o nlines nlines.$(OFILE) $(LDNAME) $(LIBS)
ifeq ($(BUILD_BUNDLE),yes)
	make nlines.app
endif

ntelnet:  ntelnet.$(OFILE)
	$(CLXX) $(LDFLAGS) -o ntelnet ntelnet.$(OFILE) $(LDNAME) $(LIBS)
ifeq ($(BUILD_BUNDLE),yes)
	make ntelnet.app
endif

nupac:  nupac.$(OFILE)
	$(CLXX) $(LDFLAGS) -o nupac nupac.$(OFILE) $(LDNAME) $(LIBS)
ifeq ($(BUILD_BUNDLE),yes)
	make nupac.app
endif

nv:  nv.$(OFILE)
	$(CLXX) $(LDFLAGS) -o nv nv.$(OFILE) $(LDNAME) $(LIBS)
ifeq ($(BUILD_BUNDLE),yes)
	make nv.app
endif

nwire:  nwire.$(OFILE)
	$(CLXX) $(LDFLAGS) -o nwire nwire.$(OFILE) $(LDNAME) $(LIBS)
ifeq ($(BUILD_BUNDLE),yes)
	make nwire.app
endif

ThreeD:  ThreeD.$(OFILE)
	$(CLXX) $(LDFLAGS) -o ThreeD ThreeD.$(OFILE) $(LDNAME) $(LIBS)
ifeq ($(BUILD_BUNDLE),yes)
	make ThreeD.app
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


