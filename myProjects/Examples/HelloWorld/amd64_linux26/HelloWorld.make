#-- start of make_header -----------------

#====================================
#  Library HelloWorld
#
#   Generated Thu Nov 26 15:03:45 2020  by yuchincheng
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_HelloWorld_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_HelloWorld_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_HelloWorld

HelloWorld_tag = $(tag)

#cmt_local_tagfile_HelloWorld = $(HelloWorld_tag)_HelloWorld.make
cmt_local_tagfile_HelloWorld = $(bin)$(HelloWorld_tag)_HelloWorld.make

else

tags      = $(tag),$(CMTEXTRATAGS)

HelloWorld_tag = $(tag)

#cmt_local_tagfile_HelloWorld = $(HelloWorld_tag).make
cmt_local_tagfile_HelloWorld = $(bin)$(HelloWorld_tag).make

endif

include $(cmt_local_tagfile_HelloWorld)
#-include $(cmt_local_tagfile_HelloWorld)

ifdef cmt_HelloWorld_has_target_tag

cmt_final_setup_HelloWorld = $(bin)setup_HelloWorld.make
cmt_dependencies_in_HelloWorld = $(bin)dependencies_HelloWorld.in
#cmt_final_setup_HelloWorld = $(bin)HelloWorld_HelloWorldsetup.make
cmt_local_HelloWorld_makefile = $(bin)HelloWorld.make

else

cmt_final_setup_HelloWorld = $(bin)setup.make
cmt_dependencies_in_HelloWorld = $(bin)dependencies.in
#cmt_final_setup_HelloWorld = $(bin)HelloWorldsetup.make
cmt_local_HelloWorld_makefile = $(bin)HelloWorld.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)HelloWorldsetup.make

#HelloWorld :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'HelloWorld'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = HelloWorld/
#HelloWorld::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

HelloWorldlibname   = $(bin)$(library_prefix)HelloWorld$(library_suffix)
HelloWorldlib       = $(HelloWorldlibname).a
HelloWorldstamp     = $(bin)HelloWorld.stamp
HelloWorldshstamp   = $(bin)HelloWorld.shstamp

HelloWorld :: dirs  HelloWorldLIB
	$(echo) "HelloWorld ok"

cmt_HelloWorld_has_prototypes = 1

#--------------------------------------

ifdef cmt_HelloWorld_has_prototypes

HelloWorldprototype :  ;

endif

HelloWorldcompile : $(bin)Hello.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

HelloWorldLIB :: $(HelloWorldlib) $(HelloWorldshstamp)
	$(echo) "HelloWorld : library ok"

$(HelloWorldlib) :: $(bin)Hello.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(HelloWorldlib) $(bin)Hello.o
	$(lib_silent) $(ranlib) $(HelloWorldlib)
	$(lib_silent) cat /dev/null >$(HelloWorldstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(HelloWorldlibname).$(shlibsuffix) :: $(HelloWorldlib) requirements $(use_requirements) $(HelloWorldstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" HelloWorld $(HelloWorld_shlibflags)
	$(lib_silent) cat /dev/null >$(HelloWorldshstamp)

$(HelloWorldshstamp) :: $(HelloWorldlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(HelloWorldlibname).$(shlibsuffix) ; then cat /dev/null >$(HelloWorldshstamp) ; fi

HelloWorldclean ::
	$(cleanup_echo) objects HelloWorld
	$(cleanup_silent) /bin/rm -f $(bin)Hello.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)Hello.o) $(patsubst %.o,%.dep,$(bin)Hello.o) $(patsubst %.o,%.d.stamp,$(bin)Hello.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf HelloWorld_deps HelloWorld_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
HelloWorldinstallname = $(library_prefix)HelloWorld$(library_suffix).$(shlibsuffix)

HelloWorld :: HelloWorldinstall ;

install :: HelloWorldinstall ;

HelloWorldinstall :: $(install_dir)/$(HelloWorldinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(HelloWorldinstallname) :: $(bin)$(HelloWorldinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(HelloWorldinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##HelloWorldclean :: HelloWorlduninstall

uninstall :: HelloWorlduninstall ;

HelloWorlduninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(HelloWorldinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),HelloWorldclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),HelloWorldprototype)

$(bin)HelloWorld_dependencies.make : $(use_requirements) $(cmt_final_setup_HelloWorld)
	$(echo) "(HelloWorld.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)Hello.cc -end_all $(includes) $(app_HelloWorld_cppflags) $(lib_HelloWorld_cppflags) -name=HelloWorld $? -f=$(cmt_dependencies_in_HelloWorld) -without_cmt

-include $(bin)HelloWorld_dependencies.make

endif
endif
endif

HelloWorldclean ::
	$(cleanup_silent) \rm -rf $(bin)HelloWorld_deps $(bin)HelloWorld_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),HelloWorldclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Hello.d

$(bin)$(binobj)Hello.d :

$(bin)$(binobj)Hello.o : $(cmt_final_setup_HelloWorld)

$(bin)$(binobj)Hello.o : $(src)Hello.cc
	$(cpp_echo) $(src)Hello.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(HelloWorld_pp_cppflags) $(lib_HelloWorld_pp_cppflags) $(Hello_pp_cppflags) $(use_cppflags) $(HelloWorld_cppflags) $(lib_HelloWorld_cppflags) $(Hello_cppflags) $(Hello_cc_cppflags)  $(src)Hello.cc
endif
endif

else
$(bin)HelloWorld_dependencies.make : $(Hello_cc_dependencies)

$(bin)HelloWorld_dependencies.make : $(src)Hello.cc

$(bin)$(binobj)Hello.o : $(Hello_cc_dependencies)
	$(cpp_echo) $(src)Hello.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(HelloWorld_pp_cppflags) $(lib_HelloWorld_pp_cppflags) $(Hello_pp_cppflags) $(use_cppflags) $(HelloWorld_cppflags) $(lib_HelloWorld_cppflags) $(Hello_cppflags) $(Hello_cc_cppflags)  $(src)Hello.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: HelloWorldclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(HelloWorld.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

HelloWorldclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library HelloWorld
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)HelloWorld$(library_suffix).a $(library_prefix)HelloWorld$(library_suffix).$(shlibsuffix) HelloWorld.stamp HelloWorld.shstamp
#-- end of cleanup_library ---------------
