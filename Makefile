
####################################################################
# Makefile
####################################################################

PATH_TO_BASE=.

include $(PATH_TO_BASE)/Makefile.target
include $(PATH_TO_BASE)/Makefile.common

EXTRA_INCS=-I$(PATH_TO_BASE)
EXTRA_LIBS=-L$(PATH_TO_BASE)


####################################################################
# Our objects 
####################################################################

DIRS=$(wildcard */)


####################################################################
# The libraries and programs 
####################################################################

PROGRAMS=$(SIMPLE_PROGRAMS)


####################################################################
# The targets 
####################################################################

all:
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) all;)
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


install: all
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) install;)


# The different Libraries and Programs...


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


