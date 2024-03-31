#-- start of make_header -----------------

#====================================
#  Library G4Svc
#
#   Generated Fri Nov 20 00:09:11 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_G4Svc_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_G4Svc_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_G4Svc

G4Svc_tag = $(tag)

#cmt_local_tagfile_G4Svc = $(G4Svc_tag)_G4Svc.make
cmt_local_tagfile_G4Svc = $(bin)$(G4Svc_tag)_G4Svc.make

else

tags      = $(tag),$(CMTEXTRATAGS)

G4Svc_tag = $(tag)

#cmt_local_tagfile_G4Svc = $(G4Svc_tag).make
cmt_local_tagfile_G4Svc = $(bin)$(G4Svc_tag).make

endif

include $(cmt_local_tagfile_G4Svc)
#-include $(cmt_local_tagfile_G4Svc)

ifdef cmt_G4Svc_has_target_tag

cmt_final_setup_G4Svc = $(bin)setup_G4Svc.make
cmt_dependencies_in_G4Svc = $(bin)dependencies_G4Svc.in
#cmt_final_setup_G4Svc = $(bin)G4Svc_G4Svcsetup.make
cmt_local_G4Svc_makefile = $(bin)G4Svc.make

else

cmt_final_setup_G4Svc = $(bin)setup.make
cmt_dependencies_in_G4Svc = $(bin)dependencies.in
#cmt_final_setup_G4Svc = $(bin)G4Svcsetup.make
cmt_local_G4Svc_makefile = $(bin)G4Svc.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)G4Svcsetup.make

#G4Svc :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'G4Svc'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = G4Svc/
#G4Svc::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

G4Svclibname   = $(bin)$(library_prefix)G4Svc$(library_suffix)
G4Svclib       = $(G4Svclibname).a
G4Svcstamp     = $(bin)G4Svc.stamp
G4Svcshstamp   = $(bin)G4Svc.shstamp

G4Svc :: dirs  G4SvcLIB
	$(echo) "G4Svc ok"

cmt_G4Svc_has_prototypes = 1

#--------------------------------------

ifdef cmt_G4Svc_has_prototypes

G4Svcprototype :  ;

endif

G4Svccompile : $(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

G4SvcLIB :: $(G4Svclib) $(G4Svcshstamp)
	$(echo) "G4Svc : library ok"

$(G4Svclib) :: $(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(G4Svclib) $(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o
	$(lib_silent) $(ranlib) $(G4Svclib)
	$(lib_silent) cat /dev/null >$(G4Svcstamp)

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

$(G4Svclibname).$(shlibsuffix) :: $(G4Svclib) requirements $(use_requirements) $(G4Svcstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" G4Svc $(G4Svc_shlibflags)
	$(lib_silent) cat /dev/null >$(G4Svcshstamp)

$(G4Svcshstamp) :: $(G4Svclibname).$(shlibsuffix)
	$(lib_silent) if test -f $(G4Svclibname).$(shlibsuffix) ; then cat /dev/null >$(G4Svcshstamp) ; fi

G4Svcclean ::
	$(cleanup_echo) objects G4Svc
	$(cleanup_silent) /bin/rm -f $(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o) $(patsubst %.o,%.dep,$(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o) $(patsubst %.o,%.d.stamp,$(bin)IG4Svc.o $(bin)G4Svc.o $(bin)G4SvcRunManager.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf G4Svc_deps G4Svc_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
G4Svcinstallname = $(library_prefix)G4Svc$(library_suffix).$(shlibsuffix)

G4Svc :: G4Svcinstall ;

install :: G4Svcinstall ;

G4Svcinstall :: $(install_dir)/$(G4Svcinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(G4Svcinstallname) :: $(bin)$(G4Svcinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(G4Svcinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##G4Svcclean :: G4Svcuninstall

uninstall :: G4Svcuninstall ;

G4Svcuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(G4Svcinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),G4Svcclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),G4Svcprototype)

$(bin)G4Svc_dependencies.make : $(use_requirements) $(cmt_final_setup_G4Svc)
	$(echo) "(G4Svc.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)IG4Svc.cc $(src)G4Svc.cc $(src)G4SvcRunManager.cc -end_all $(includes) $(app_G4Svc_cppflags) $(lib_G4Svc_cppflags) -name=G4Svc $? -f=$(cmt_dependencies_in_G4Svc) -without_cmt

-include $(bin)G4Svc_dependencies.make

endif
endif
endif

G4Svcclean ::
	$(cleanup_silent) \rm -rf $(bin)G4Svc_deps $(bin)G4Svc_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),G4Svcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)IG4Svc.d

$(bin)$(binobj)IG4Svc.d :

$(bin)$(binobj)IG4Svc.o : $(cmt_final_setup_G4Svc)

$(bin)$(binobj)IG4Svc.o : $(src)IG4Svc.cc
	$(cpp_echo) $(src)IG4Svc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(G4Svc_pp_cppflags) $(lib_G4Svc_pp_cppflags) $(IG4Svc_pp_cppflags) $(use_cppflags) $(G4Svc_cppflags) $(lib_G4Svc_cppflags) $(IG4Svc_cppflags) $(IG4Svc_cc_cppflags)  $(src)IG4Svc.cc
endif
endif

else
$(bin)G4Svc_dependencies.make : $(IG4Svc_cc_dependencies)

$(bin)G4Svc_dependencies.make : $(src)IG4Svc.cc

$(bin)$(binobj)IG4Svc.o : $(IG4Svc_cc_dependencies)
	$(cpp_echo) $(src)IG4Svc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(G4Svc_pp_cppflags) $(lib_G4Svc_pp_cppflags) $(IG4Svc_pp_cppflags) $(use_cppflags) $(G4Svc_cppflags) $(lib_G4Svc_cppflags) $(IG4Svc_cppflags) $(IG4Svc_cc_cppflags)  $(src)IG4Svc.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),G4Svcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4Svc.d

$(bin)$(binobj)G4Svc.d :

$(bin)$(binobj)G4Svc.o : $(cmt_final_setup_G4Svc)

$(bin)$(binobj)G4Svc.o : $(src)G4Svc.cc
	$(cpp_echo) $(src)G4Svc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(G4Svc_pp_cppflags) $(lib_G4Svc_pp_cppflags) $(G4Svc_pp_cppflags) $(use_cppflags) $(G4Svc_cppflags) $(lib_G4Svc_cppflags) $(G4Svc_cppflags) $(G4Svc_cc_cppflags)  $(src)G4Svc.cc
endif
endif

else
$(bin)G4Svc_dependencies.make : $(G4Svc_cc_dependencies)

$(bin)G4Svc_dependencies.make : $(src)G4Svc.cc

$(bin)$(binobj)G4Svc.o : $(G4Svc_cc_dependencies)
	$(cpp_echo) $(src)G4Svc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(G4Svc_pp_cppflags) $(lib_G4Svc_pp_cppflags) $(G4Svc_pp_cppflags) $(use_cppflags) $(G4Svc_cppflags) $(lib_G4Svc_cppflags) $(G4Svc_cppflags) $(G4Svc_cc_cppflags)  $(src)G4Svc.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),G4Svcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4SvcRunManager.d

$(bin)$(binobj)G4SvcRunManager.d :

$(bin)$(binobj)G4SvcRunManager.o : $(cmt_final_setup_G4Svc)

$(bin)$(binobj)G4SvcRunManager.o : $(src)G4SvcRunManager.cc
	$(cpp_echo) $(src)G4SvcRunManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(G4Svc_pp_cppflags) $(lib_G4Svc_pp_cppflags) $(G4SvcRunManager_pp_cppflags) $(use_cppflags) $(G4Svc_cppflags) $(lib_G4Svc_cppflags) $(G4SvcRunManager_cppflags) $(G4SvcRunManager_cc_cppflags)  $(src)G4SvcRunManager.cc
endif
endif

else
$(bin)G4Svc_dependencies.make : $(G4SvcRunManager_cc_dependencies)

$(bin)G4Svc_dependencies.make : $(src)G4SvcRunManager.cc

$(bin)$(binobj)G4SvcRunManager.o : $(G4SvcRunManager_cc_dependencies)
	$(cpp_echo) $(src)G4SvcRunManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(G4Svc_pp_cppflags) $(lib_G4Svc_pp_cppflags) $(G4SvcRunManager_pp_cppflags) $(use_cppflags) $(G4Svc_cppflags) $(lib_G4Svc_cppflags) $(G4SvcRunManager_cppflags) $(G4SvcRunManager_cc_cppflags)  $(src)G4SvcRunManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: G4Svcclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(G4Svc.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

G4Svcclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library G4Svc
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)G4Svc$(library_suffix).a $(library_prefix)G4Svc$(library_suffix).$(shlibsuffix) G4Svc.stamp G4Svc.shstamp
#-- end of cleanup_library ---------------
