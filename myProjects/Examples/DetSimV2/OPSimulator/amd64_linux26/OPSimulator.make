#-- start of make_header -----------------

#====================================
#  Library OPSimulator
#
#   Generated Fri Nov 20 00:11:18 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_OPSimulator_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_OPSimulator_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_OPSimulator

OPSimulator_tag = $(tag)

#cmt_local_tagfile_OPSimulator = $(OPSimulator_tag)_OPSimulator.make
cmt_local_tagfile_OPSimulator = $(bin)$(OPSimulator_tag)_OPSimulator.make

else

tags      = $(tag),$(CMTEXTRATAGS)

OPSimulator_tag = $(tag)

#cmt_local_tagfile_OPSimulator = $(OPSimulator_tag).make
cmt_local_tagfile_OPSimulator = $(bin)$(OPSimulator_tag).make

endif

include $(cmt_local_tagfile_OPSimulator)
#-include $(cmt_local_tagfile_OPSimulator)

ifdef cmt_OPSimulator_has_target_tag

cmt_final_setup_OPSimulator = $(bin)setup_OPSimulator.make
cmt_dependencies_in_OPSimulator = $(bin)dependencies_OPSimulator.in
#cmt_final_setup_OPSimulator = $(bin)OPSimulator_OPSimulatorsetup.make
cmt_local_OPSimulator_makefile = $(bin)OPSimulator.make

else

cmt_final_setup_OPSimulator = $(bin)setup.make
cmt_dependencies_in_OPSimulator = $(bin)dependencies.in
#cmt_final_setup_OPSimulator = $(bin)OPSimulatorsetup.make
cmt_local_OPSimulator_makefile = $(bin)OPSimulator.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)OPSimulatorsetup.make

#OPSimulator :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'OPSimulator'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = OPSimulator/
#OPSimulator::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

OPSimulatorlibname   = $(bin)$(library_prefix)OPSimulator$(library_suffix)
OPSimulatorlib       = $(OPSimulatorlibname).a
OPSimulatorstamp     = $(bin)OPSimulator.stamp
OPSimulatorshstamp   = $(bin)OPSimulator.shstamp

OPSimulator :: dirs  OPSimulatorLIB
	$(echo) "OPSimulator ok"

cmt_OPSimulator_has_prototypes = 1

#--------------------------------------

ifdef cmt_OPSimulator_has_prototypes

OPSimulatorprototype :  ;

endif

OPSimulatorcompile : $(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

OPSimulatorLIB :: $(OPSimulatorlib) $(OPSimulatorshstamp)
	$(echo) "OPSimulator : library ok"

$(OPSimulatorlib) :: $(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(OPSimulatorlib) $(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o
	$(lib_silent) $(ranlib) $(OPSimulatorlib)
	$(lib_silent) cat /dev/null >$(OPSimulatorstamp)

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

$(OPSimulatorlibname).$(shlibsuffix) :: $(OPSimulatorlib) requirements $(use_requirements) $(OPSimulatorstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" OPSimulator $(OPSimulator_shlibflags)
	$(lib_silent) cat /dev/null >$(OPSimulatorshstamp)

$(OPSimulatorshstamp) :: $(OPSimulatorlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(OPSimulatorlibname).$(shlibsuffix) ; then cat /dev/null >$(OPSimulatorshstamp) ; fi

OPSimulatorclean ::
	$(cleanup_echo) objects OPSimulator
	$(cleanup_silent) /bin/rm -f $(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o) $(patsubst %.o,%.dep,$(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o) $(patsubst %.o,%.d.stamp,$(bin)G4GenStepContainer.o $(bin)OPSimSvc.o $(bin)G4OPSimulator.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf OPSimulator_deps OPSimulator_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
OPSimulatorinstallname = $(library_prefix)OPSimulator$(library_suffix).$(shlibsuffix)

OPSimulator :: OPSimulatorinstall ;

install :: OPSimulatorinstall ;

OPSimulatorinstall :: $(install_dir)/$(OPSimulatorinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(OPSimulatorinstallname) :: $(bin)$(OPSimulatorinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(OPSimulatorinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##OPSimulatorclean :: OPSimulatoruninstall

uninstall :: OPSimulatoruninstall ;

OPSimulatoruninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(OPSimulatorinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),OPSimulatorclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),OPSimulatorprototype)

$(bin)OPSimulator_dependencies.make : $(use_requirements) $(cmt_final_setup_OPSimulator)
	$(echo) "(OPSimulator.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)G4GenStepContainer.cc $(src)OPSimSvc.cc $(src)G4OPSimulator.cc -end_all $(includes) $(app_OPSimulator_cppflags) $(lib_OPSimulator_cppflags) -name=OPSimulator $? -f=$(cmt_dependencies_in_OPSimulator) -without_cmt

-include $(bin)OPSimulator_dependencies.make

endif
endif
endif

OPSimulatorclean ::
	$(cleanup_silent) \rm -rf $(bin)OPSimulator_deps $(bin)OPSimulator_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),OPSimulatorclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4GenStepContainer.d

$(bin)$(binobj)G4GenStepContainer.d :

$(bin)$(binobj)G4GenStepContainer.o : $(cmt_final_setup_OPSimulator)

$(bin)$(binobj)G4GenStepContainer.o : $(src)G4GenStepContainer.cc
	$(cpp_echo) $(src)G4GenStepContainer.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(OPSimulator_pp_cppflags) $(lib_OPSimulator_pp_cppflags) $(G4GenStepContainer_pp_cppflags) $(use_cppflags) $(OPSimulator_cppflags) $(lib_OPSimulator_cppflags) $(G4GenStepContainer_cppflags) $(G4GenStepContainer_cc_cppflags)  $(src)G4GenStepContainer.cc
endif
endif

else
$(bin)OPSimulator_dependencies.make : $(G4GenStepContainer_cc_dependencies)

$(bin)OPSimulator_dependencies.make : $(src)G4GenStepContainer.cc

$(bin)$(binobj)G4GenStepContainer.o : $(G4GenStepContainer_cc_dependencies)
	$(cpp_echo) $(src)G4GenStepContainer.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(OPSimulator_pp_cppflags) $(lib_OPSimulator_pp_cppflags) $(G4GenStepContainer_pp_cppflags) $(use_cppflags) $(OPSimulator_cppflags) $(lib_OPSimulator_cppflags) $(G4GenStepContainer_cppflags) $(G4GenStepContainer_cc_cppflags)  $(src)G4GenStepContainer.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),OPSimulatorclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)OPSimSvc.d

$(bin)$(binobj)OPSimSvc.d :

$(bin)$(binobj)OPSimSvc.o : $(cmt_final_setup_OPSimulator)

$(bin)$(binobj)OPSimSvc.o : $(src)OPSimSvc.cc
	$(cpp_echo) $(src)OPSimSvc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(OPSimulator_pp_cppflags) $(lib_OPSimulator_pp_cppflags) $(OPSimSvc_pp_cppflags) $(use_cppflags) $(OPSimulator_cppflags) $(lib_OPSimulator_cppflags) $(OPSimSvc_cppflags) $(OPSimSvc_cc_cppflags)  $(src)OPSimSvc.cc
endif
endif

else
$(bin)OPSimulator_dependencies.make : $(OPSimSvc_cc_dependencies)

$(bin)OPSimulator_dependencies.make : $(src)OPSimSvc.cc

$(bin)$(binobj)OPSimSvc.o : $(OPSimSvc_cc_dependencies)
	$(cpp_echo) $(src)OPSimSvc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(OPSimulator_pp_cppflags) $(lib_OPSimulator_pp_cppflags) $(OPSimSvc_pp_cppflags) $(use_cppflags) $(OPSimulator_cppflags) $(lib_OPSimulator_cppflags) $(OPSimSvc_cppflags) $(OPSimSvc_cc_cppflags)  $(src)OPSimSvc.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),OPSimulatorclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4OPSimulator.d

$(bin)$(binobj)G4OPSimulator.d :

$(bin)$(binobj)G4OPSimulator.o : $(cmt_final_setup_OPSimulator)

$(bin)$(binobj)G4OPSimulator.o : $(src)G4OPSimulator.cc
	$(cpp_echo) $(src)G4OPSimulator.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(OPSimulator_pp_cppflags) $(lib_OPSimulator_pp_cppflags) $(G4OPSimulator_pp_cppflags) $(use_cppflags) $(OPSimulator_cppflags) $(lib_OPSimulator_cppflags) $(G4OPSimulator_cppflags) $(G4OPSimulator_cc_cppflags)  $(src)G4OPSimulator.cc
endif
endif

else
$(bin)OPSimulator_dependencies.make : $(G4OPSimulator_cc_dependencies)

$(bin)OPSimulator_dependencies.make : $(src)G4OPSimulator.cc

$(bin)$(binobj)G4OPSimulator.o : $(G4OPSimulator_cc_dependencies)
	$(cpp_echo) $(src)G4OPSimulator.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(OPSimulator_pp_cppflags) $(lib_OPSimulator_pp_cppflags) $(G4OPSimulator_pp_cppflags) $(use_cppflags) $(OPSimulator_cppflags) $(lib_OPSimulator_cppflags) $(G4OPSimulator_cppflags) $(G4OPSimulator_cc_cppflags)  $(src)G4OPSimulator.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: OPSimulatorclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(OPSimulator.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

OPSimulatorclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library OPSimulator
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)OPSimulator$(library_suffix).a $(library_prefix)OPSimulator$(library_suffix).$(shlibsuffix) OPSimulator.stamp OPSimulator.shstamp
#-- end of cleanup_library ---------------
