
####################################################################
# Makefile 
####################################################################


PATH_TO_BASE=../..

include $(PATH_TO_BASE)/Makefile.target
include $(PATH_TO_BASE)/Makefile.common

####################################################################
# Our objects 
####################################################################

####################################################################
# The libraries and programs 
####################################################################

####################################################################
# The targets 
####################################################################


all:

clean:
	rm -rf *.o *.obj *.lib *.a *.s *.bak *.log map.f build *Data *.pdb *.lib
	rm -rf $(PROGRAMS) *.html *.exe *.exe.so *68k *.SYM .DS_Store *.ilk *.app
	rm -rf .gdb_history .nautilus-metafile.xml */Debug */*.ncb
	rm -rf ._* */._* */*/._* */*/*/._*
	rm -rf *~ */*~ */*/*~ */*/*/*~
#	rm -f *.h *.tab *.empty *.proto

fix: fix_perm fix_type


# dependencies
# NOTE: its ok to see errors when running makedepend
depend:
	makedepend -fMakefile.dep $(CFLAGS) *.cpp


remove_depends:
	rm -f Makefile.dep
	touch Makefile.dep


include $(PATH_TO_BASE)/Makefile.tail

include Makefile.dep


####################################################################
# Dependencies, if any
#
# Despite the warning, feel free to delete anything below and
# regenerate with a "make depend"
####################################################################
