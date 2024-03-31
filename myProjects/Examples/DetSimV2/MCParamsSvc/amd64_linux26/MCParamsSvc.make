#-- start of make_header -----------------

#====================================
#  Library MCParamsSvc
#
#   Generated Fri Nov 20 00:13:20 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MCParamsSvc_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MCParamsSvc_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MCParamsSvc

MCParamsSvc_tag = $(tag)

#cmt_local_tagfile_MCParamsSvc = $(MCParamsSvc_tag)_MCParamsSvc.make
cmt_local_tagfile_MCParamsSvc = $(bin)$(MCParamsSvc_tag)_MCParamsSvc.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MCParamsSvc_tag = $(tag)

#cmt_local_tagfile_MCParamsSvc = $(MCParamsSvc_tag).make
cmt_local_tagfile_MCParamsSvc = $(bin)$(MCParamsSvc_tag).make

endif

include $(cmt_local_tagfile_MCParamsSvc)
#-include $(cmt_local_tagfile_MCParamsSvc)

ifdef cmt_MCParamsSvc_has_target_tag

cmt_final_setup_MCParamsSvc = $(bin)setup_MCParamsSvc.make
cmt_dependencies_in_MCParamsSvc = $(bin)dependencies_MCParamsSvc.in
#cmt_final_setup_MCParamsSvc = $(bin)MCParamsSvc_MCParamsSvcsetup.make
cmt_local_MCParamsSvc_makefile = $(bin)MCParamsSvc.make

else

cmt_final_setup_MCParamsSvc = $(bin)setup.make
cmt_dependencies_in_MCParamsSvc = $(bin)dependencies.in
#cmt_final_setup_MCParamsSvc = $(bin)MCParamsSvcsetup.make
cmt_local_MCParamsSvc_makefile = $(bin)MCParamsSvc.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MCParamsSvcsetup.make

#MCParamsSvc :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MCParamsSvc'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MCParamsSvc/
#MCParamsSvc::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

MCParamsSvclibname   = $(bin)$(library_prefix)MCParamsSvc$(library_suffix)
MCParamsSvclib       = $(MCParamsSvclibname).a
MCParamsSvcstamp     = $(bin)MCParamsSvc.stamp
MCParamsSvcshstamp   = $(bin)MCParamsSvc.shstamp

MCParamsSvc :: dirs  MCParamsSvcLIB
	$(echo) "MCParamsSvc ok"

cmt_MCParamsSvc_has_prototypes = 1

#--------------------------------------

ifdef cmt_MCParamsSvc_has_prototypes

MCParamsSvcprototype :  ;

endif

MCParamsSvccompile : $(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

MCParamsSvcLIB :: $(MCParamsSvclib) $(MCParamsSvcshstamp)
	$(echo) "MCParamsSvc : library ok"

$(MCParamsSvclib) :: $(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(MCParamsSvclib) $(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o
	$(lib_silent) $(ranlib) $(MCParamsSvclib)
	$(lib_silent) cat /dev/null >$(MCParamsSvcstamp)

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

$(MCParamsSvclibname).$(shlibsuffix) :: $(MCParamsSvclib) requirements $(use_requirements) $(MCParamsSvcstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" MCParamsSvc $(MCParamsSvc_shlibflags)
	$(lib_silent) cat /dev/null >$(MCParamsSvcshstamp)

$(MCParamsSvcshstamp) :: $(MCParamsSvclibname).$(shlibsuffix)
	$(lib_silent) if test -f $(MCParamsSvclibname).$(shlibsuffix) ; then cat /dev/null >$(MCParamsSvcshstamp) ; fi

MCParamsSvcclean ::
	$(cleanup_echo) objects MCParamsSvc
	$(cleanup_silent) /bin/rm -f $(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o) $(patsubst %.o,%.dep,$(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o) $(patsubst %.o,%.d.stamp,$(bin)MCParamsCrestSvc.o $(bin)MCParamsDBSvc.o $(bin)MCParamsFileSvc.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf MCParamsSvc_deps MCParamsSvc_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
MCParamsSvcinstallname = $(library_prefix)MCParamsSvc$(library_suffix).$(shlibsuffix)

MCParamsSvc :: MCParamsSvcinstall ;

install :: MCParamsSvcinstall ;

MCParamsSvcinstall :: $(install_dir)/$(MCParamsSvcinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(MCParamsSvcinstallname) :: $(bin)$(MCParamsSvcinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MCParamsSvcinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##MCParamsSvcclean :: MCParamsSvcuninstall

uninstall :: MCParamsSvcuninstall ;

MCParamsSvcuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MCParamsSvcinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),MCParamsSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),MCParamsSvcprototype)

$(bin)MCParamsSvc_dependencies.make : $(use_requirements) $(cmt_final_setup_MCParamsSvc)
	$(echo) "(MCParamsSvc.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)MCParamsCrestSvc.cc $(src)MCParamsDBSvc.cc $(src)MCParamsFileSvc.cc -end_all $(includes) $(app_MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) -name=MCParamsSvc $? -f=$(cmt_dependencies_in_MCParamsSvc) -without_cmt

-include $(bin)MCParamsSvc_dependencies.make

endif
endif
endif

MCParamsSvcclean ::
	$(cleanup_silent) \rm -rf $(bin)MCParamsSvc_deps $(bin)MCParamsSvc_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),MCParamsSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MCParamsCrestSvc.d

$(bin)$(binobj)MCParamsCrestSvc.d :

$(bin)$(binobj)MCParamsCrestSvc.o : $(cmt_final_setup_MCParamsSvc)

$(bin)$(binobj)MCParamsCrestSvc.o : $(src)MCParamsCrestSvc.cc
	$(cpp_echo) $(src)MCParamsCrestSvc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(MCParamsSvc_pp_cppflags) $(lib_MCParamsSvc_pp_cppflags) $(MCParamsCrestSvc_pp_cppflags) $(use_cppflags) $(MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) $(MCParamsCrestSvc_cppflags) $(MCParamsCrestSvc_cc_cppflags)  $(src)MCParamsCrestSvc.cc
endif
endif

else
$(bin)MCParamsSvc_dependencies.make : $(MCParamsCrestSvc_cc_dependencies)

$(bin)MCParamsSvc_dependencies.make : $(src)MCParamsCrestSvc.cc

$(bin)$(binobj)MCParamsCrestSvc.o : $(MCParamsCrestSvc_cc_dependencies)
	$(cpp_echo) $(src)MCParamsCrestSvc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MCParamsSvc_pp_cppflags) $(lib_MCParamsSvc_pp_cppflags) $(MCParamsCrestSvc_pp_cppflags) $(use_cppflags) $(MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) $(MCParamsCrestSvc_cppflags) $(MCParamsCrestSvc_cc_cppflags)  $(src)MCParamsCrestSvc.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),MCParamsSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MCParamsDBSvc.d

$(bin)$(binobj)MCParamsDBSvc.d :

$(bin)$(binobj)MCParamsDBSvc.o : $(cmt_final_setup_MCParamsSvc)

$(bin)$(binobj)MCParamsDBSvc.o : $(src)MCParamsDBSvc.cc
	$(cpp_echo) $(src)MCParamsDBSvc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(MCParamsSvc_pp_cppflags) $(lib_MCParamsSvc_pp_cppflags) $(MCParamsDBSvc_pp_cppflags) $(use_cppflags) $(MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) $(MCParamsDBSvc_cppflags) $(MCParamsDBSvc_cc_cppflags)  $(src)MCParamsDBSvc.cc
endif
endif

else
$(bin)MCParamsSvc_dependencies.make : $(MCParamsDBSvc_cc_dependencies)

$(bin)MCParamsSvc_dependencies.make : $(src)MCParamsDBSvc.cc

$(bin)$(binobj)MCParamsDBSvc.o : $(MCParamsDBSvc_cc_dependencies)
	$(cpp_echo) $(src)MCParamsDBSvc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MCParamsSvc_pp_cppflags) $(lib_MCParamsSvc_pp_cppflags) $(MCParamsDBSvc_pp_cppflags) $(use_cppflags) $(MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) $(MCParamsDBSvc_cppflags) $(MCParamsDBSvc_cc_cppflags)  $(src)MCParamsDBSvc.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),MCParamsSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MCParamsFileSvc.d

$(bin)$(binobj)MCParamsFileSvc.d :

$(bin)$(binobj)MCParamsFileSvc.o : $(cmt_final_setup_MCParamsSvc)

$(bin)$(binobj)MCParamsFileSvc.o : $(src)MCParamsFileSvc.cc
	$(cpp_echo) $(src)MCParamsFileSvc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(MCParamsSvc_pp_cppflags) $(lib_MCParamsSvc_pp_cppflags) $(MCParamsFileSvc_pp_cppflags) $(use_cppflags) $(MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) $(MCParamsFileSvc_cppflags) $(MCParamsFileSvc_cc_cppflags)  $(src)MCParamsFileSvc.cc
endif
endif

else
$(bin)MCParamsSvc_dependencies.make : $(MCParamsFileSvc_cc_dependencies)

$(bin)MCParamsSvc_dependencies.make : $(src)MCParamsFileSvc.cc

$(bin)$(binobj)MCParamsFileSvc.o : $(MCParamsFileSvc_cc_dependencies)
	$(cpp_echo) $(src)MCParamsFileSvc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MCParamsSvc_pp_cppflags) $(lib_MCParamsSvc_pp_cppflags) $(MCParamsFileSvc_pp_cppflags) $(use_cppflags) $(MCParamsSvc_cppflags) $(lib_MCParamsSvc_cppflags) $(MCParamsFileSvc_cppflags) $(MCParamsFileSvc_cc_cppflags)  $(src)MCParamsFileSvc.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: MCParamsSvcclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MCParamsSvc.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

MCParamsSvcclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library MCParamsSvc
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)MCParamsSvc$(library_suffix).a $(library_prefix)MCParamsSvc$(library_suffix).$(shlibsuffix) MCParamsSvc.stamp MCParamsSvc.shstamp
#-- end of cleanup_library ---------------
