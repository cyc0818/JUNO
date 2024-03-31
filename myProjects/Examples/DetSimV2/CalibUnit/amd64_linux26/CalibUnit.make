#-- start of make_header -----------------

#====================================
#  Library CalibUnit
#
#   Generated Fri Nov 20 00:10:55 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_CalibUnit_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_CalibUnit_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_CalibUnit

CalibUnit_tag = $(tag)

#cmt_local_tagfile_CalibUnit = $(CalibUnit_tag)_CalibUnit.make
cmt_local_tagfile_CalibUnit = $(bin)$(CalibUnit_tag)_CalibUnit.make

else

tags      = $(tag),$(CMTEXTRATAGS)

CalibUnit_tag = $(tag)

#cmt_local_tagfile_CalibUnit = $(CalibUnit_tag).make
cmt_local_tagfile_CalibUnit = $(bin)$(CalibUnit_tag).make

endif

include $(cmt_local_tagfile_CalibUnit)
#-include $(cmt_local_tagfile_CalibUnit)

ifdef cmt_CalibUnit_has_target_tag

cmt_final_setup_CalibUnit = $(bin)setup_CalibUnit.make
cmt_dependencies_in_CalibUnit = $(bin)dependencies_CalibUnit.in
#cmt_final_setup_CalibUnit = $(bin)CalibUnit_CalibUnitsetup.make
cmt_local_CalibUnit_makefile = $(bin)CalibUnit.make

else

cmt_final_setup_CalibUnit = $(bin)setup.make
cmt_dependencies_in_CalibUnit = $(bin)dependencies.in
#cmt_final_setup_CalibUnit = $(bin)CalibUnitsetup.make
cmt_local_CalibUnit_makefile = $(bin)CalibUnit.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)CalibUnitsetup.make

#CalibUnit :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'CalibUnit'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = CalibUnit/
#CalibUnit::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

CalibUnitlibname   = $(bin)$(library_prefix)CalibUnit$(library_suffix)
CalibUnitlib       = $(CalibUnitlibname).a
CalibUnitstamp     = $(bin)CalibUnit.stamp
CalibUnitshstamp   = $(bin)CalibUnit.shstamp

CalibUnit :: dirs  CalibUnitLIB
	$(echo) "CalibUnit ok"

cmt_CalibUnit_has_prototypes = 1

#--------------------------------------

ifdef cmt_CalibUnit_has_prototypes

CalibUnitprototype :  ;

endif

CalibUnitcompile : $(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

CalibUnitLIB :: $(CalibUnitlib) $(CalibUnitshstamp)
	$(echo) "CalibUnit : library ok"

$(CalibUnitlib) :: $(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(CalibUnitlib) $(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o
	$(lib_silent) $(ranlib) $(CalibUnitlib)
	$(lib_silent) cat /dev/null >$(CalibUnitstamp)

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

$(CalibUnitlibname).$(shlibsuffix) :: $(CalibUnitlib) requirements $(use_requirements) $(CalibUnitstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" CalibUnit $(CalibUnit_shlibflags)
	$(lib_silent) cat /dev/null >$(CalibUnitshstamp)

$(CalibUnitshstamp) :: $(CalibUnitlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(CalibUnitlibname).$(shlibsuffix) ; then cat /dev/null >$(CalibUnitshstamp) ; fi

CalibUnitclean ::
	$(cleanup_echo) objects CalibUnit
	$(cleanup_silent) /bin/rm -f $(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o) $(patsubst %.o,%.dep,$(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o) $(patsubst %.o,%.d.stamp,$(bin)CalibTube_offcenter_twocone_Construction.o $(bin)CalibTube_convexwindow_Construction.o $(bin)CalibTube_offcenter_Construction.o $(bin)Calib_GuideTube_Placement.o $(bin)CalibTube_reflectwindow_Construction.o $(bin)Calib_GuideTube_Construction.o $(bin)CalibTube_flatwindow_Construction.o $(bin)CalibTube_twocone_Construction.o $(bin)CalibTubePlacement.o $(bin)CalibSourcePlacement.o $(bin)CalibTubeConstruction.o $(bin)CalibSourceConstruction.o $(bin)CalibTube_onecone_Construction.o $(bin)CalibTube_offcenter_reflectwindow_Construction.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf CalibUnit_deps CalibUnit_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
CalibUnitinstallname = $(library_prefix)CalibUnit$(library_suffix).$(shlibsuffix)

CalibUnit :: CalibUnitinstall ;

install :: CalibUnitinstall ;

CalibUnitinstall :: $(install_dir)/$(CalibUnitinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(CalibUnitinstallname) :: $(bin)$(CalibUnitinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(CalibUnitinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##CalibUnitclean :: CalibUnituninstall

uninstall :: CalibUnituninstall ;

CalibUnituninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(CalibUnitinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),CalibUnitprototype)

$(bin)CalibUnit_dependencies.make : $(use_requirements) $(cmt_final_setup_CalibUnit)
	$(echo) "(CalibUnit.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)CalibTube_offcenter_twocone_Construction.cc $(src)CalibTube_convexwindow_Construction.cc $(src)CalibTube_offcenter_Construction.cc $(src)Calib_GuideTube_Placement.cc $(src)CalibTube_reflectwindow_Construction.cc $(src)Calib_GuideTube_Construction.cc $(src)CalibTube_flatwindow_Construction.cc $(src)CalibTube_twocone_Construction.cc $(src)CalibTubePlacement.cc $(src)CalibSourcePlacement.cc $(src)CalibTubeConstruction.cc $(src)CalibSourceConstruction.cc $(src)CalibTube_onecone_Construction.cc $(src)CalibTube_offcenter_reflectwindow_Construction.cc -end_all $(includes) $(app_CalibUnit_cppflags) $(lib_CalibUnit_cppflags) -name=CalibUnit $? -f=$(cmt_dependencies_in_CalibUnit) -without_cmt

-include $(bin)CalibUnit_dependencies.make

endif
endif
endif

CalibUnitclean ::
	$(cleanup_silent) \rm -rf $(bin)CalibUnit_deps $(bin)CalibUnit_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_offcenter_twocone_Construction.d

$(bin)$(binobj)CalibTube_offcenter_twocone_Construction.d :

$(bin)$(binobj)CalibTube_offcenter_twocone_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_offcenter_twocone_Construction.o : $(src)CalibTube_offcenter_twocone_Construction.cc
	$(cpp_echo) $(src)CalibTube_offcenter_twocone_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_offcenter_twocone_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_offcenter_twocone_Construction_cppflags) $(CalibTube_offcenter_twocone_Construction_cc_cppflags)  $(src)CalibTube_offcenter_twocone_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_offcenter_twocone_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_offcenter_twocone_Construction.cc

$(bin)$(binobj)CalibTube_offcenter_twocone_Construction.o : $(CalibTube_offcenter_twocone_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_offcenter_twocone_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_offcenter_twocone_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_offcenter_twocone_Construction_cppflags) $(CalibTube_offcenter_twocone_Construction_cc_cppflags)  $(src)CalibTube_offcenter_twocone_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_convexwindow_Construction.d

$(bin)$(binobj)CalibTube_convexwindow_Construction.d :

$(bin)$(binobj)CalibTube_convexwindow_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_convexwindow_Construction.o : $(src)CalibTube_convexwindow_Construction.cc
	$(cpp_echo) $(src)CalibTube_convexwindow_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_convexwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_convexwindow_Construction_cppflags) $(CalibTube_convexwindow_Construction_cc_cppflags)  $(src)CalibTube_convexwindow_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_convexwindow_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_convexwindow_Construction.cc

$(bin)$(binobj)CalibTube_convexwindow_Construction.o : $(CalibTube_convexwindow_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_convexwindow_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_convexwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_convexwindow_Construction_cppflags) $(CalibTube_convexwindow_Construction_cc_cppflags)  $(src)CalibTube_convexwindow_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_offcenter_Construction.d

$(bin)$(binobj)CalibTube_offcenter_Construction.d :

$(bin)$(binobj)CalibTube_offcenter_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_offcenter_Construction.o : $(src)CalibTube_offcenter_Construction.cc
	$(cpp_echo) $(src)CalibTube_offcenter_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_offcenter_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_offcenter_Construction_cppflags) $(CalibTube_offcenter_Construction_cc_cppflags)  $(src)CalibTube_offcenter_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_offcenter_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_offcenter_Construction.cc

$(bin)$(binobj)CalibTube_offcenter_Construction.o : $(CalibTube_offcenter_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_offcenter_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_offcenter_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_offcenter_Construction_cppflags) $(CalibTube_offcenter_Construction_cc_cppflags)  $(src)CalibTube_offcenter_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Calib_GuideTube_Placement.d

$(bin)$(binobj)Calib_GuideTube_Placement.d :

$(bin)$(binobj)Calib_GuideTube_Placement.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)Calib_GuideTube_Placement.o : $(src)Calib_GuideTube_Placement.cc
	$(cpp_echo) $(src)Calib_GuideTube_Placement.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(Calib_GuideTube_Placement_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(Calib_GuideTube_Placement_cppflags) $(Calib_GuideTube_Placement_cc_cppflags)  $(src)Calib_GuideTube_Placement.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(Calib_GuideTube_Placement_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)Calib_GuideTube_Placement.cc

$(bin)$(binobj)Calib_GuideTube_Placement.o : $(Calib_GuideTube_Placement_cc_dependencies)
	$(cpp_echo) $(src)Calib_GuideTube_Placement.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(Calib_GuideTube_Placement_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(Calib_GuideTube_Placement_cppflags) $(Calib_GuideTube_Placement_cc_cppflags)  $(src)Calib_GuideTube_Placement.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_reflectwindow_Construction.d

$(bin)$(binobj)CalibTube_reflectwindow_Construction.d :

$(bin)$(binobj)CalibTube_reflectwindow_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_reflectwindow_Construction.o : $(src)CalibTube_reflectwindow_Construction.cc
	$(cpp_echo) $(src)CalibTube_reflectwindow_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_reflectwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_reflectwindow_Construction_cppflags) $(CalibTube_reflectwindow_Construction_cc_cppflags)  $(src)CalibTube_reflectwindow_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_reflectwindow_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_reflectwindow_Construction.cc

$(bin)$(binobj)CalibTube_reflectwindow_Construction.o : $(CalibTube_reflectwindow_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_reflectwindow_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_reflectwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_reflectwindow_Construction_cppflags) $(CalibTube_reflectwindow_Construction_cc_cppflags)  $(src)CalibTube_reflectwindow_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Calib_GuideTube_Construction.d

$(bin)$(binobj)Calib_GuideTube_Construction.d :

$(bin)$(binobj)Calib_GuideTube_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)Calib_GuideTube_Construction.o : $(src)Calib_GuideTube_Construction.cc
	$(cpp_echo) $(src)Calib_GuideTube_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(Calib_GuideTube_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(Calib_GuideTube_Construction_cppflags) $(Calib_GuideTube_Construction_cc_cppflags)  $(src)Calib_GuideTube_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(Calib_GuideTube_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)Calib_GuideTube_Construction.cc

$(bin)$(binobj)Calib_GuideTube_Construction.o : $(Calib_GuideTube_Construction_cc_dependencies)
	$(cpp_echo) $(src)Calib_GuideTube_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(Calib_GuideTube_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(Calib_GuideTube_Construction_cppflags) $(Calib_GuideTube_Construction_cc_cppflags)  $(src)Calib_GuideTube_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_flatwindow_Construction.d

$(bin)$(binobj)CalibTube_flatwindow_Construction.d :

$(bin)$(binobj)CalibTube_flatwindow_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_flatwindow_Construction.o : $(src)CalibTube_flatwindow_Construction.cc
	$(cpp_echo) $(src)CalibTube_flatwindow_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_flatwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_flatwindow_Construction_cppflags) $(CalibTube_flatwindow_Construction_cc_cppflags)  $(src)CalibTube_flatwindow_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_flatwindow_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_flatwindow_Construction.cc

$(bin)$(binobj)CalibTube_flatwindow_Construction.o : $(CalibTube_flatwindow_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_flatwindow_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_flatwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_flatwindow_Construction_cppflags) $(CalibTube_flatwindow_Construction_cc_cppflags)  $(src)CalibTube_flatwindow_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_twocone_Construction.d

$(bin)$(binobj)CalibTube_twocone_Construction.d :

$(bin)$(binobj)CalibTube_twocone_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_twocone_Construction.o : $(src)CalibTube_twocone_Construction.cc
	$(cpp_echo) $(src)CalibTube_twocone_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_twocone_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_twocone_Construction_cppflags) $(CalibTube_twocone_Construction_cc_cppflags)  $(src)CalibTube_twocone_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_twocone_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_twocone_Construction.cc

$(bin)$(binobj)CalibTube_twocone_Construction.o : $(CalibTube_twocone_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_twocone_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_twocone_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_twocone_Construction_cppflags) $(CalibTube_twocone_Construction_cc_cppflags)  $(src)CalibTube_twocone_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTubePlacement.d

$(bin)$(binobj)CalibTubePlacement.d :

$(bin)$(binobj)CalibTubePlacement.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTubePlacement.o : $(src)CalibTubePlacement.cc
	$(cpp_echo) $(src)CalibTubePlacement.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTubePlacement_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTubePlacement_cppflags) $(CalibTubePlacement_cc_cppflags)  $(src)CalibTubePlacement.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTubePlacement_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTubePlacement.cc

$(bin)$(binobj)CalibTubePlacement.o : $(CalibTubePlacement_cc_dependencies)
	$(cpp_echo) $(src)CalibTubePlacement.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTubePlacement_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTubePlacement_cppflags) $(CalibTubePlacement_cc_cppflags)  $(src)CalibTubePlacement.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibSourcePlacement.d

$(bin)$(binobj)CalibSourcePlacement.d :

$(bin)$(binobj)CalibSourcePlacement.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibSourcePlacement.o : $(src)CalibSourcePlacement.cc
	$(cpp_echo) $(src)CalibSourcePlacement.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibSourcePlacement_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibSourcePlacement_cppflags) $(CalibSourcePlacement_cc_cppflags)  $(src)CalibSourcePlacement.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibSourcePlacement_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibSourcePlacement.cc

$(bin)$(binobj)CalibSourcePlacement.o : $(CalibSourcePlacement_cc_dependencies)
	$(cpp_echo) $(src)CalibSourcePlacement.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibSourcePlacement_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibSourcePlacement_cppflags) $(CalibSourcePlacement_cc_cppflags)  $(src)CalibSourcePlacement.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTubeConstruction.d

$(bin)$(binobj)CalibTubeConstruction.d :

$(bin)$(binobj)CalibTubeConstruction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTubeConstruction.o : $(src)CalibTubeConstruction.cc
	$(cpp_echo) $(src)CalibTubeConstruction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTubeConstruction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTubeConstruction_cppflags) $(CalibTubeConstruction_cc_cppflags)  $(src)CalibTubeConstruction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTubeConstruction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTubeConstruction.cc

$(bin)$(binobj)CalibTubeConstruction.o : $(CalibTubeConstruction_cc_dependencies)
	$(cpp_echo) $(src)CalibTubeConstruction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTubeConstruction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTubeConstruction_cppflags) $(CalibTubeConstruction_cc_cppflags)  $(src)CalibTubeConstruction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibSourceConstruction.d

$(bin)$(binobj)CalibSourceConstruction.d :

$(bin)$(binobj)CalibSourceConstruction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibSourceConstruction.o : $(src)CalibSourceConstruction.cc
	$(cpp_echo) $(src)CalibSourceConstruction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibSourceConstruction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibSourceConstruction_cppflags) $(CalibSourceConstruction_cc_cppflags)  $(src)CalibSourceConstruction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibSourceConstruction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibSourceConstruction.cc

$(bin)$(binobj)CalibSourceConstruction.o : $(CalibSourceConstruction_cc_dependencies)
	$(cpp_echo) $(src)CalibSourceConstruction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibSourceConstruction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibSourceConstruction_cppflags) $(CalibSourceConstruction_cc_cppflags)  $(src)CalibSourceConstruction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_onecone_Construction.d

$(bin)$(binobj)CalibTube_onecone_Construction.d :

$(bin)$(binobj)CalibTube_onecone_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_onecone_Construction.o : $(src)CalibTube_onecone_Construction.cc
	$(cpp_echo) $(src)CalibTube_onecone_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_onecone_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_onecone_Construction_cppflags) $(CalibTube_onecone_Construction_cc_cppflags)  $(src)CalibTube_onecone_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_onecone_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_onecone_Construction.cc

$(bin)$(binobj)CalibTube_onecone_Construction.o : $(CalibTube_onecone_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_onecone_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_onecone_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_onecone_Construction_cppflags) $(CalibTube_onecone_Construction_cc_cppflags)  $(src)CalibTube_onecone_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),CalibUnitclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalibTube_offcenter_reflectwindow_Construction.d

$(bin)$(binobj)CalibTube_offcenter_reflectwindow_Construction.d :

$(bin)$(binobj)CalibTube_offcenter_reflectwindow_Construction.o : $(cmt_final_setup_CalibUnit)

$(bin)$(binobj)CalibTube_offcenter_reflectwindow_Construction.o : $(src)CalibTube_offcenter_reflectwindow_Construction.cc
	$(cpp_echo) $(src)CalibTube_offcenter_reflectwindow_Construction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_offcenter_reflectwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_offcenter_reflectwindow_Construction_cppflags) $(CalibTube_offcenter_reflectwindow_Construction_cc_cppflags)  $(src)CalibTube_offcenter_reflectwindow_Construction.cc
endif
endif

else
$(bin)CalibUnit_dependencies.make : $(CalibTube_offcenter_reflectwindow_Construction_cc_dependencies)

$(bin)CalibUnit_dependencies.make : $(src)CalibTube_offcenter_reflectwindow_Construction.cc

$(bin)$(binobj)CalibTube_offcenter_reflectwindow_Construction.o : $(CalibTube_offcenter_reflectwindow_Construction_cc_dependencies)
	$(cpp_echo) $(src)CalibTube_offcenter_reflectwindow_Construction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(CalibUnit_pp_cppflags) $(lib_CalibUnit_pp_cppflags) $(CalibTube_offcenter_reflectwindow_Construction_pp_cppflags) $(use_cppflags) $(CalibUnit_cppflags) $(lib_CalibUnit_cppflags) $(CalibTube_offcenter_reflectwindow_Construction_cppflags) $(CalibTube_offcenter_reflectwindow_Construction_cc_cppflags)  $(src)CalibTube_offcenter_reflectwindow_Construction.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: CalibUnitclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(CalibUnit.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

CalibUnitclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library CalibUnit
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)CalibUnit$(library_suffix).a $(library_prefix)CalibUnit$(library_suffix).$(shlibsuffix) CalibUnit.stamp CalibUnit.shstamp
#-- end of cleanup_library ---------------
