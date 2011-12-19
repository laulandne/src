
APPNAME=$(shell basename $(shell pwd))

####################################################################
# Makefile
####################################################################

PATH_TO_BASE=..

include $(PATH_TO_BASE)/Makefile.target
include $(PATH_TO_BASE)/Makefile.common

#EXTRA_INCS=-I$(PATH_TO_BASE)
#EXTRA_LIBS=-L$(PATH_TO_BASE)


####################################################################
# Our objects 
####################################################################

OBJS1=$(subst .m,.$(OFILE),$(wildcard *.m))
#OBJS2=$(subst .m,.$(OFILE),$(wildcard *.m))
OBJS=$(OBJS1)	$(OBJS2)
DIRS=$(wildcard Classes)


####################################################################
# The libraries and programs 
####################################################################

PROGRAMS=$(APPNAME)


####################################################################
# The targets 
####################################################################

all:	dist


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


dist:	$(APPNAME)
	rm -rf $(APPNAME).app
	mkdir $(APPNAME).app
	cp $(APPNAME) PkgInfo $(APPNAME).app
	if test "$(wildcard *.nib)" ; then cp *.nib $(APPNAME).app ; fi
	cp  *.png $(APPNAME).app
	if test "$(wildcard Resources/Shaders/*.?sh)" ; then cp Resources/Shaders/*.?sh $(APPNAME).app ; fi
	if test "$(wildcard Resources/*.png)" ; then cp Resources/*.png $(APPNAME).app ; fi
	cp iphone.Info.plist $(APPNAME).app/Info.plist


$(APPNAME):	$(OBJS)
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) all;)
# Programs...
	$(CLXX) -o $(APPNAME) $(OBJS) $(LFRAMEWORKS) $(EXTRA_FLAGS)
	ldid $(APPNAME)


#nlines:  nlines.$(OFILE)
#	$(CLXX) $(LDFLAGS) -o nlines nlines.$(OFILE) ./guilib/libguilib_static.a ./alib/libalib_static.a ./nickcpp/libnickcpp_static.a ./platform/libplatform_static.a $(LIBS)
#ifeq ($(BUILD_BUNDLE),yes)
#	make nlines.app
#endif


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


