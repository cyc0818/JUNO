#-- start of make_header -----------------

#====================================
#  Library MCGlobalTimeSvc
#
#   Generated Fri Nov 20 00:09:26 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MCGlobalTimeSvc_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MCGlobalTimeSvc_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MCGlobalTimeSvc

MCGlobalTimeSvc_tag = $(tag)

#cmt_local_tagfile_MCGlobalTimeSvc = $(MCGlobalTimeSvc_tag)_MCGlobalTimeSvc.make
cmt_local_tagfile_MCGlobalTimeSvc = $(bin)$(MCGlobalTimeSvc_tag)_MCGlobalTimeSvc.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MCGlobalTimeSvc_tag = $(tag)

#cmt_local_tagfile_MCGlobalTimeSvc = $(MCGlobalTimeSvc_tag).make
cmt_local_tagfile_MCGlobalTimeSvc = $(bin)$(MCGlobalTimeSvc_tag).make

endif

include $(cmt_local_tagfile_MCGlobalTimeSvc)
#-include $(cmt_local_tagfile_MCGlobalTimeSvc)

ifdef cmt_MCGlobalTimeSvc_has_target_tag

cmt_final_setup_MCGlobalTimeSvc = $(bin)setup_MCGlobalTimeSvc.make
cmt_dependencies_in_MCGlobalTimeSvc = $(bin)dependencies_MCGlobalTimeSvc.in
#cmt_final_setup_MCGlobalTimeSvc = $(bin)MCGlobalTimeSvc_MCGlobalTimeSvcsetup.make
cmt_local_MCGlobalTimeSvc_makefile = $(bin)MCGlobalTimeSvc.make

else

cmt_final_setup_MCGlobalTimeSvc = $(bin)setup.make
cmt_dependencies_in_MCGlobalTimeSvc = $(bin)dependencies.in
#cmt_final_setup_MCGlobalTimeSvc = $(bin)MCGlobalTimeSvcsetup.make
cmt_local_MCGlobalTimeSvc_makefile = $(bin)MCGlobalTimeSvc.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MCGlobalTimeSvcsetup.make

#MCGlobalTimeSvc :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MCGlobalTimeSvc'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MCGlobalTimeSvc/
#MCGlobalTimeSvc::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

MCGlobalTimeSvclibname   = $(bin)$(library_prefix)MCGlobalTimeSvc$(library_suffix)
MCGlobalTimeSvclib       = $(MCGlobalTimeSvclibname).a
MCGlobalTimeSvcstamp     = $(bin)MCGlobalTimeSvc.stamp
MCGlobalTimeSvcshstamp   = $(bin)MCGlobalTimeSvc.shstamp

MCGlobalTimeSvc :: dirs  MCGlobalTimeSvcLIB
	$(echo) "MCGlobalTimeSvc ok"

cmt_MCGlobalTimeSvc_has_prototypes = 1

#--------------------------------------

ifdef cmt_MCGlobalTimeSvc_has_prototypes

MCGlobalTimeSvcprototype :  ;

endif

MCGlobalTimeSvccompile : $(bin)MCGlobalTimeSvc.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

MCGlobalTimeSvcLIB :: $(MCGlobalTimeSvclib) $(MCGlobalTimeSvcshstamp)
	$(echo) "MCGlobalTimeSvc : library ok"

$(MCGlobalTimeSvclib) :: $(bin)MCGlobalTimeSvc.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(MCGlobalTimeSvclib) $(bin)MCGlobalTimeSvc.o
	$(lib_silent) $(ranlib) $(MCGlobalTimeSvclib)
	$(lib_silent) cat /dev/null >$(MCGlobalTimeSvcstamp)

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

$(MCGlobalTimeSvclibname).$(shlibsuffix) :: $(MCGlobalTimeSvclib) requirements $(use_requirements) $(MCGlobalTimeSvcstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" MCGlobalTimeSvc $(MCGlobalTimeSvc_shlibflags)
	$(lib_silent) cat /dev/null >$(MCGlobalTimeSvcshstamp)

$(MCGlobalTimeSvcshstamp) :: $(MCGlobalTimeSvclibname).$(shlibsuffix)
	$(lib_silent) if test -f $(MCGlobalTimeSvclibname).$(shlibsuffix) ; then cat /dev/null >$(MCGlobalTimeSvcshstamp) ; fi

MCGlobalTimeSvcclean ::
	$(cleanup_echo) objects MCGlobalTimeSvc
	$(cleanup_silent) /bin/rm -f $(bin)MCGlobalTimeSvc.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)MCGlobalTimeSvc.o) $(patsubst %.o,%.dep,$(bin)MCGlobalTimeSvc.o) $(patsubst %.o,%.d.stamp,$(bin)MCGlobalTimeSvc.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf MCGlobalTimeSvc_deps MCGlobalTimeSvc_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
MCGlobalTimeSvcinstallname = $(library_prefix)MCGlobalTimeSvc$(library_suffix).$(shlibsuffix)

MCGlobalTimeSvc :: MCGlobalTimeSvcinstall ;

install :: MCGlobalTimeSvcinstall ;

MCGlobalTimeSvcinstall :: $(install_dir)/$(MCGlobalTimeSvcinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(MCGlobalTimeSvcinstallname) :: $(bin)$(MCGlobalTimeSvcinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MCGlobalTimeSvcinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##MCGlobalTimeSvcclean :: MCGlobalTimeSvcuninstall

uninstall :: MCGlobalTimeSvcuninstall ;

MCGlobalTimeSvcuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MCGlobalTimeSvcinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),MCGlobalTimeSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),MCGlobalTimeSvcprototype)

$(bin)MCGlobalTimeSvc_dependencies.make : $(use_requirements) $(cmt_final_setup_MCGlobalTimeSvc)
	$(echo) "(MCGlobalTimeSvc.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)MCGlobalTimeSvc.cc -end_all $(includes) $(app_MCGlobalTimeSvc_cppflags) $(lib_MCGlobalTimeSvc_cppflags) -name=MCGlobalTimeSvc $? -f=$(cmt_dependencies_in_MCGlobalTimeSvc) -without_cmt

-include $(bin)MCGlobalTimeSvc_dependencies.make

endif
endif
endif

MCGlobalTimeSvcclean ::
	$(cleanup_silent) \rm -rf $(bin)MCGlobalTimeSvc_deps $(bin)MCGlobalTimeSvc_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),MCGlobalTimeSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MCGlobalTimeSvc.d

$(bin)$(binobj)MCGlobalTimeSvc.d :

$(bin)$(binobj)MCGlobalTimeSvc.o : $(cmt_final_setup_MCGlobalTimeSvc)

$(bin)$(binobj)MCGlobalTimeSvc.o : $(src)MCGlobalTimeSvc.cc
	$(cpp_echo) $(src)MCGlobalTimeSvc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(MCGlobalTimeSvc_pp_cppflags) $(lib_MCGlobalTimeSvc_pp_cppflags) $(MCGlobalTimeSvc_pp_cppflags) $(use_cppflags) $(MCGlobalTimeSvc_cppflags) $(lib_MCGlobalTimeSvc_cppflags) $(MCGlobalTimeSvc_cppflags) $(MCGlobalTimeSvc_cc_cppflags)  $(src)MCGlobalTimeSvc.cc
endif
endif

else
$(bin)MCGlobalTimeSvc_dependencies.make : $(MCGlobalTimeSvc_cc_dependencies)

$(bin)MCGlobalTimeSvc_dependencies.make : $(src)MCGlobalTimeSvc.cc

$(bin)$(binobj)MCGlobalTimeSvc.o : $(MCGlobalTimeSvc_cc_dependencies)
	$(cpp_echo) $(src)MCGlobalTimeSvc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MCGlobalTimeSvc_pp_cppflags) $(lib_MCGlobalTimeSvc_pp_cppflags) $(MCGlobalTimeSvc_pp_cppflags) $(use_cppflags) $(MCGlobalTimeSvc_cppflags) $(lib_MCGlobalTimeSvc_cppflags) $(MCGlobalTimeSvc_cppflags) $(MCGlobalTimeSvc_cc_cppflags)  $(src)MCGlobalTimeSvc.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: MCGlobalTimeSvcclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MCGlobalTimeSvc.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

MCGlobalTimeSvcclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library MCGlobalTimeSvc
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)MCGlobalTimeSvc$(library_suffix).a $(library_prefix)MCGlobalTimeSvc$(library_suffix).$(shlibsuffix) MCGlobalTimeSvc.stamp MCGlobalTimeSvc.shstamp
#-- end of cleanup_library ---------------
