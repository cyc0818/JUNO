#-- start of make_header -----------------

#====================================
#  Library SimUtil
#
#   Generated Fri Nov 20 00:25:36 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_SimUtil_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_SimUtil_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_SimUtil

SimUtil_tag = $(tag)

#cmt_local_tagfile_SimUtil = $(SimUtil_tag)_SimUtil.make
cmt_local_tagfile_SimUtil = $(bin)$(SimUtil_tag)_SimUtil.make

else

tags      = $(tag),$(CMTEXTRATAGS)

SimUtil_tag = $(tag)

#cmt_local_tagfile_SimUtil = $(SimUtil_tag).make
cmt_local_tagfile_SimUtil = $(bin)$(SimUtil_tag).make

endif

include $(cmt_local_tagfile_SimUtil)
#-include $(cmt_local_tagfile_SimUtil)

ifdef cmt_SimUtil_has_target_tag

cmt_final_setup_SimUtil = $(bin)setup_SimUtil.make
cmt_dependencies_in_SimUtil = $(bin)dependencies_SimUtil.in
#cmt_final_setup_SimUtil = $(bin)SimUtil_SimUtilsetup.make
cmt_local_SimUtil_makefile = $(bin)SimUtil.make

else

cmt_final_setup_SimUtil = $(bin)setup.make
cmt_dependencies_in_SimUtil = $(bin)dependencies.in
#cmt_final_setup_SimUtil = $(bin)SimUtilsetup.make
cmt_local_SimUtil_makefile = $(bin)SimUtil.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)SimUtilsetup.make

#SimUtil :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'SimUtil'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = SimUtil/
#SimUtil::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

SimUtillibname   = $(bin)$(library_prefix)SimUtil$(library_suffix)
SimUtillib       = $(SimUtillibname).a
SimUtilstamp     = $(bin)SimUtil.stamp
SimUtilshstamp   = $(bin)SimUtil.shstamp

SimUtil :: dirs  SimUtilLIB
	$(echo) "SimUtil ok"

cmt_SimUtil_has_prototypes = 1

#--------------------------------------

ifdef cmt_SimUtil_has_prototypes

SimUtilprototype :  ;

endif

SimUtilcompile : $(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

SimUtilLIB :: $(SimUtillib) $(SimUtilshstamp)
	$(echo) "SimUtil : library ok"

$(SimUtillib) :: $(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(SimUtillib) $(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o
	$(lib_silent) $(ranlib) $(SimUtillib)
	$(lib_silent) cat /dev/null >$(SimUtilstamp)

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

$(SimUtillibname).$(shlibsuffix) :: $(SimUtillib) requirements $(use_requirements) $(SimUtilstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" SimUtil $(SimUtil_shlibflags)
	$(lib_silent) cat /dev/null >$(SimUtilshstamp)

$(SimUtilshstamp) :: $(SimUtillibname).$(shlibsuffix)
	$(lib_silent) if test -f $(SimUtillibname).$(shlibsuffix) ; then cat /dev/null >$(SimUtilshstamp) ; fi

SimUtilclean ::
	$(cleanup_echo) objects SimUtil
	$(cleanup_silent) /bin/rm -f $(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o) $(patsubst %.o,%.dep,$(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o) $(patsubst %.o,%.d.stamp,$(bin)EDepByParticle.o $(bin)CalPositionCylinder.o $(bin)GDMLDetElemConstruction.o $(bin)CalPositionBall.o $(bin)PMTinPrototypePos.o $(bin)MuonAnalysis.o $(bin)DYB2CalPositionInterface.o $(bin)HexagonPosBall.o $(bin)CalPositionBallStake.o $(bin)DetectionContructionUtils.o $(bin)GDMLMaterialBuilder.o $(bin)NormalTrackInfo.o $(bin)SimTrackSvc.o $(bin)EnergyDepositedCal.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf SimUtil_deps SimUtil_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
SimUtilinstallname = $(library_prefix)SimUtil$(library_suffix).$(shlibsuffix)

SimUtil :: SimUtilinstall ;

install :: SimUtilinstall ;

SimUtilinstall :: $(install_dir)/$(SimUtilinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(SimUtilinstallname) :: $(bin)$(SimUtilinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(SimUtilinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##SimUtilclean :: SimUtiluninstall

uninstall :: SimUtiluninstall ;

SimUtiluninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(SimUtilinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),SimUtilprototype)

$(bin)SimUtil_dependencies.make : $(use_requirements) $(cmt_final_setup_SimUtil)
	$(echo) "(SimUtil.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)EDepByParticle.cc $(src)CalPositionCylinder.cc $(src)GDMLDetElemConstruction.cc $(src)CalPositionBall.cc $(src)PMTinPrototypePos.cc $(src)MuonAnalysis.cc $(src)DYB2CalPositionInterface.cc $(src)HexagonPosBall.cc $(src)CalPositionBallStake.cc $(src)DetectionContructionUtils.cc $(src)GDMLMaterialBuilder.cc $(src)NormalTrackInfo.cc $(src)SimTrackSvc.cc $(src)EnergyDepositedCal.cc -end_all $(includes) $(app_SimUtil_cppflags) $(lib_SimUtil_cppflags) -name=SimUtil $? -f=$(cmt_dependencies_in_SimUtil) -without_cmt

-include $(bin)SimUtil_dependencies.make

endif
endif
endif

SimUtilclean ::
	$(cleanup_silent) \rm -rf $(bin)SimUtil_deps $(bin)SimUtil_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)EDepByParticle.d

$(bin)$(binobj)EDepByParticle.d :

$(bin)$(binobj)EDepByParticle.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)EDepByParticle.o : $(src)EDepByParticle.cc
	$(cpp_echo) $(src)EDepByParticle.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(EDepByParticle_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(EDepByParticle_cppflags) $(EDepByParticle_cc_cppflags)  $(src)EDepByParticle.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(EDepByParticle_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)EDepByParticle.cc

$(bin)$(binobj)EDepByParticle.o : $(EDepByParticle_cc_dependencies)
	$(cpp_echo) $(src)EDepByParticle.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(EDepByParticle_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(EDepByParticle_cppflags) $(EDepByParticle_cc_cppflags)  $(src)EDepByParticle.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalPositionCylinder.d

$(bin)$(binobj)CalPositionCylinder.d :

$(bin)$(binobj)CalPositionCylinder.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)CalPositionCylinder.o : $(src)CalPositionCylinder.cc
	$(cpp_echo) $(src)CalPositionCylinder.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(CalPositionCylinder_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(CalPositionCylinder_cppflags) $(CalPositionCylinder_cc_cppflags)  $(src)CalPositionCylinder.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(CalPositionCylinder_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)CalPositionCylinder.cc

$(bin)$(binobj)CalPositionCylinder.o : $(CalPositionCylinder_cc_dependencies)
	$(cpp_echo) $(src)CalPositionCylinder.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(CalPositionCylinder_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(CalPositionCylinder_cppflags) $(CalPositionCylinder_cc_cppflags)  $(src)CalPositionCylinder.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)GDMLDetElemConstruction.d

$(bin)$(binobj)GDMLDetElemConstruction.d :

$(bin)$(binobj)GDMLDetElemConstruction.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)GDMLDetElemConstruction.o : $(src)GDMLDetElemConstruction.cc
	$(cpp_echo) $(src)GDMLDetElemConstruction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(GDMLDetElemConstruction_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(GDMLDetElemConstruction_cppflags) $(GDMLDetElemConstruction_cc_cppflags)  $(src)GDMLDetElemConstruction.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(GDMLDetElemConstruction_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)GDMLDetElemConstruction.cc

$(bin)$(binobj)GDMLDetElemConstruction.o : $(GDMLDetElemConstruction_cc_dependencies)
	$(cpp_echo) $(src)GDMLDetElemConstruction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(GDMLDetElemConstruction_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(GDMLDetElemConstruction_cppflags) $(GDMLDetElemConstruction_cc_cppflags)  $(src)GDMLDetElemConstruction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalPositionBall.d

$(bin)$(binobj)CalPositionBall.d :

$(bin)$(binobj)CalPositionBall.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)CalPositionBall.o : $(src)CalPositionBall.cc
	$(cpp_echo) $(src)CalPositionBall.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(CalPositionBall_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(CalPositionBall_cppflags) $(CalPositionBall_cc_cppflags)  $(src)CalPositionBall.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(CalPositionBall_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)CalPositionBall.cc

$(bin)$(binobj)CalPositionBall.o : $(CalPositionBall_cc_dependencies)
	$(cpp_echo) $(src)CalPositionBall.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(CalPositionBall_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(CalPositionBall_cppflags) $(CalPositionBall_cc_cppflags)  $(src)CalPositionBall.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PMTinPrototypePos.d

$(bin)$(binobj)PMTinPrototypePos.d :

$(bin)$(binobj)PMTinPrototypePos.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)PMTinPrototypePos.o : $(src)PMTinPrototypePos.cc
	$(cpp_echo) $(src)PMTinPrototypePos.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(PMTinPrototypePos_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(PMTinPrototypePos_cppflags) $(PMTinPrototypePos_cc_cppflags)  $(src)PMTinPrototypePos.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(PMTinPrototypePos_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)PMTinPrototypePos.cc

$(bin)$(binobj)PMTinPrototypePos.o : $(PMTinPrototypePos_cc_dependencies)
	$(cpp_echo) $(src)PMTinPrototypePos.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(PMTinPrototypePos_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(PMTinPrototypePos_cppflags) $(PMTinPrototypePos_cc_cppflags)  $(src)PMTinPrototypePos.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MuonAnalysis.d

$(bin)$(binobj)MuonAnalysis.d :

$(bin)$(binobj)MuonAnalysis.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)MuonAnalysis.o : $(src)MuonAnalysis.cc
	$(cpp_echo) $(src)MuonAnalysis.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(MuonAnalysis_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(MuonAnalysis_cppflags) $(MuonAnalysis_cc_cppflags)  $(src)MuonAnalysis.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(MuonAnalysis_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)MuonAnalysis.cc

$(bin)$(binobj)MuonAnalysis.o : $(MuonAnalysis_cc_dependencies)
	$(cpp_echo) $(src)MuonAnalysis.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(MuonAnalysis_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(MuonAnalysis_cppflags) $(MuonAnalysis_cc_cppflags)  $(src)MuonAnalysis.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DYB2CalPositionInterface.d

$(bin)$(binobj)DYB2CalPositionInterface.d :

$(bin)$(binobj)DYB2CalPositionInterface.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)DYB2CalPositionInterface.o : $(src)DYB2CalPositionInterface.cc
	$(cpp_echo) $(src)DYB2CalPositionInterface.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(DYB2CalPositionInterface_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(DYB2CalPositionInterface_cppflags) $(DYB2CalPositionInterface_cc_cppflags)  $(src)DYB2CalPositionInterface.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(DYB2CalPositionInterface_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)DYB2CalPositionInterface.cc

$(bin)$(binobj)DYB2CalPositionInterface.o : $(DYB2CalPositionInterface_cc_dependencies)
	$(cpp_echo) $(src)DYB2CalPositionInterface.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(DYB2CalPositionInterface_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(DYB2CalPositionInterface_cppflags) $(DYB2CalPositionInterface_cc_cppflags)  $(src)DYB2CalPositionInterface.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HexagonPosBall.d

$(bin)$(binobj)HexagonPosBall.d :

$(bin)$(binobj)HexagonPosBall.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)HexagonPosBall.o : $(src)HexagonPosBall.cc
	$(cpp_echo) $(src)HexagonPosBall.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(HexagonPosBall_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(HexagonPosBall_cppflags) $(HexagonPosBall_cc_cppflags)  $(src)HexagonPosBall.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(HexagonPosBall_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)HexagonPosBall.cc

$(bin)$(binobj)HexagonPosBall.o : $(HexagonPosBall_cc_dependencies)
	$(cpp_echo) $(src)HexagonPosBall.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(HexagonPosBall_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(HexagonPosBall_cppflags) $(HexagonPosBall_cc_cppflags)  $(src)HexagonPosBall.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)CalPositionBallStake.d

$(bin)$(binobj)CalPositionBallStake.d :

$(bin)$(binobj)CalPositionBallStake.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)CalPositionBallStake.o : $(src)CalPositionBallStake.cc
	$(cpp_echo) $(src)CalPositionBallStake.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(CalPositionBallStake_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(CalPositionBallStake_cppflags) $(CalPositionBallStake_cc_cppflags)  $(src)CalPositionBallStake.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(CalPositionBallStake_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)CalPositionBallStake.cc

$(bin)$(binobj)CalPositionBallStake.o : $(CalPositionBallStake_cc_dependencies)
	$(cpp_echo) $(src)CalPositionBallStake.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(CalPositionBallStake_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(CalPositionBallStake_cppflags) $(CalPositionBallStake_cc_cppflags)  $(src)CalPositionBallStake.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DetectionContructionUtils.d

$(bin)$(binobj)DetectionContructionUtils.d :

$(bin)$(binobj)DetectionContructionUtils.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)DetectionContructionUtils.o : $(src)DetectionContructionUtils.cc
	$(cpp_echo) $(src)DetectionContructionUtils.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(DetectionContructionUtils_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(DetectionContructionUtils_cppflags) $(DetectionContructionUtils_cc_cppflags)  $(src)DetectionContructionUtils.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(DetectionContructionUtils_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)DetectionContructionUtils.cc

$(bin)$(binobj)DetectionContructionUtils.o : $(DetectionContructionUtils_cc_dependencies)
	$(cpp_echo) $(src)DetectionContructionUtils.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(DetectionContructionUtils_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(DetectionContructionUtils_cppflags) $(DetectionContructionUtils_cc_cppflags)  $(src)DetectionContructionUtils.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)GDMLMaterialBuilder.d

$(bin)$(binobj)GDMLMaterialBuilder.d :

$(bin)$(binobj)GDMLMaterialBuilder.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)GDMLMaterialBuilder.o : $(src)GDMLMaterialBuilder.cc
	$(cpp_echo) $(src)GDMLMaterialBuilder.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(GDMLMaterialBuilder_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(GDMLMaterialBuilder_cppflags) $(GDMLMaterialBuilder_cc_cppflags)  $(src)GDMLMaterialBuilder.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(GDMLMaterialBuilder_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)GDMLMaterialBuilder.cc

$(bin)$(binobj)GDMLMaterialBuilder.o : $(GDMLMaterialBuilder_cc_dependencies)
	$(cpp_echo) $(src)GDMLMaterialBuilder.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(GDMLMaterialBuilder_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(GDMLMaterialBuilder_cppflags) $(GDMLMaterialBuilder_cc_cppflags)  $(src)GDMLMaterialBuilder.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)NormalTrackInfo.d

$(bin)$(binobj)NormalTrackInfo.d :

$(bin)$(binobj)NormalTrackInfo.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)NormalTrackInfo.o : $(src)NormalTrackInfo.cc
	$(cpp_echo) $(src)NormalTrackInfo.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(NormalTrackInfo_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(NormalTrackInfo_cppflags) $(NormalTrackInfo_cc_cppflags)  $(src)NormalTrackInfo.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(NormalTrackInfo_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)NormalTrackInfo.cc

$(bin)$(binobj)NormalTrackInfo.o : $(NormalTrackInfo_cc_dependencies)
	$(cpp_echo) $(src)NormalTrackInfo.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(NormalTrackInfo_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(NormalTrackInfo_cppflags) $(NormalTrackInfo_cc_cppflags)  $(src)NormalTrackInfo.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)SimTrackSvc.d

$(bin)$(binobj)SimTrackSvc.d :

$(bin)$(binobj)SimTrackSvc.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)SimTrackSvc.o : $(src)SimTrackSvc.cc
	$(cpp_echo) $(src)SimTrackSvc.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(SimTrackSvc_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(SimTrackSvc_cppflags) $(SimTrackSvc_cc_cppflags)  $(src)SimTrackSvc.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(SimTrackSvc_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)SimTrackSvc.cc

$(bin)$(binobj)SimTrackSvc.o : $(SimTrackSvc_cc_dependencies)
	$(cpp_echo) $(src)SimTrackSvc.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(SimTrackSvc_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(SimTrackSvc_cppflags) $(SimTrackSvc_cc_cppflags)  $(src)SimTrackSvc.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),SimUtilclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)EnergyDepositedCal.d

$(bin)$(binobj)EnergyDepositedCal.d :

$(bin)$(binobj)EnergyDepositedCal.o : $(cmt_final_setup_SimUtil)

$(bin)$(binobj)EnergyDepositedCal.o : $(src)EnergyDepositedCal.cc
	$(cpp_echo) $(src)EnergyDepositedCal.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(EnergyDepositedCal_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(EnergyDepositedCal_cppflags) $(EnergyDepositedCal_cc_cppflags)  $(src)EnergyDepositedCal.cc
endif
endif

else
$(bin)SimUtil_dependencies.make : $(EnergyDepositedCal_cc_dependencies)

$(bin)SimUtil_dependencies.make : $(src)EnergyDepositedCal.cc

$(bin)$(binobj)EnergyDepositedCal.o : $(EnergyDepositedCal_cc_dependencies)
	$(cpp_echo) $(src)EnergyDepositedCal.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(SimUtil_pp_cppflags) $(lib_SimUtil_pp_cppflags) $(EnergyDepositedCal_pp_cppflags) $(use_cppflags) $(SimUtil_cppflags) $(lib_SimUtil_cppflags) $(EnergyDepositedCal_cppflags) $(EnergyDepositedCal_cc_cppflags)  $(src)EnergyDepositedCal.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: SimUtilclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(SimUtil.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

SimUtilclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library SimUtil
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)SimUtil$(library_suffix).a $(library_prefix)SimUtil$(library_suffix).$(shlibsuffix) SimUtil.stamp SimUtil.shstamp
#-- end of cleanup_library ---------------
