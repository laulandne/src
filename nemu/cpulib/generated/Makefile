

###################################################################
# Makefile
###################################################################

PATH_TO_BASE=../..

include $(PATH_TO_BASE)/Makefile.target
include $(PATH_TO_BASE)/Makefile.common

EXTRA_INCS=-I$(PATH_TO_BASE)


###################################################################
# Our objects
###################################################################

.SUFFIXES: txt

TXTS=$(wildcard *.txt)
OBJS=$(subst .txt,_tab.h,$(wildcard *.txt))


###################################################################
# The libraries and programs
###################################################################



###################################################################
# The targets
###################################################################

all:


clean:
	rm -rf *.o *.obj *.lib *.a *.s *.bak *.log map.f build *Data *.pdb *.lib
	rm -rf $(PROGRAMS) *.html *.exe *.exe.so *68k *.SYM .DS_Store *.ilk *.app
	rm -rf .gdb_history .nautilus-metafile.xml */Debug */*.ncb
	rm -rf ._* */._* */*/._* */*/*/._*
	rm -rf *~ */*~ */*/*~ */*/*/*~
#	rm -f *.h *.tab *.empty *.proto

fix: fix_perm fix_type


# The different libraries and programs


# dependencies
# NOTE: its ok to see errors when running makedepend
depend:
	makedepend $(DEPENDFLAGS) *.cpp


remove_depends:
	rm -f Makefile.dep
	touch Makefile.dep
	

include $(PATH_TO_BASE)/Makefile.tail

include Makefile.dep


