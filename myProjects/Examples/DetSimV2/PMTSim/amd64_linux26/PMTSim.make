#-- start of make_header -----------------

#====================================
#  Library PMTSim
#
#   Generated Fri Nov 20 00:25:53 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_PMTSim_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_PMTSim_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_PMTSim

PMTSim_tag = $(tag)

#cmt_local_tagfile_PMTSim = $(PMTSim_tag)_PMTSim.make
cmt_local_tagfile_PMTSim = $(bin)$(PMTSim_tag)_PMTSim.make

else

tags      = $(tag),$(CMTEXTRATAGS)

PMTSim_tag = $(tag)

#cmt_local_tagfile_PMTSim = $(PMTSim_tag).make
cmt_local_tagfile_PMTSim = $(bin)$(PMTSim_tag).make

endif

include $(cmt_local_tagfile_PMTSim)
#-include $(cmt_local_tagfile_PMTSim)

ifdef cmt_PMTSim_has_target_tag

cmt_final_setup_PMTSim = $(bin)setup_PMTSim.make
cmt_dependencies_in_PMTSim = $(bin)dependencies_PMTSim.in
#cmt_final_setup_PMTSim = $(bin)PMTSim_PMTSimsetup.make
cmt_local_PMTSim_makefile = $(bin)PMTSim.make

else

cmt_final_setup_PMTSim = $(bin)setup.make
cmt_dependencies_in_PMTSim = $(bin)dependencies.in
#cmt_final_setup_PMTSim = $(bin)PMTSimsetup.make
cmt_local_PMTSim_makefile = $(bin)PMTSim.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)PMTSimsetup.make

#PMTSim :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'PMTSim'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = PMTSim/
#PMTSim::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

PMTSimlibname   = $(bin)$(library_prefix)PMTSim$(library_suffix)
PMTSimlib       = $(PMTSimlibname).a
PMTSimstamp     = $(bin)PMTSim.stamp
PMTSimshstamp   = $(bin)PMTSim.shstamp

PMTSim :: dirs  PMTSimLIB
	$(echo) "PMTSim ok"

cmt_PMTSim_has_prototypes = 1

#--------------------------------------

ifdef cmt_PMTSim_has_prototypes

PMTSimprototype :  ;

endif

PMTSimcompile : $(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

PMTSimLIB :: $(PMTSimlib) $(PMTSimshstamp)
	$(echo) "PMTSim : library ok"

$(PMTSimlib) :: $(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(PMTSimlib) $(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o
	$(lib_silent) $(ranlib) $(PMTSimlib)
	$(lib_silent) cat /dev/null >$(PMTSimstamp)

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

$(PMTSimlibname).$(shlibsuffix) :: $(PMTSimlib) requirements $(use_requirements) $(PMTSimstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" PMTSim $(PMTSim_shlibflags)
	$(lib_silent) cat /dev/null >$(PMTSimshstamp)

$(PMTSimshstamp) :: $(PMTSimlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(PMTSimlibname).$(shlibsuffix) ; then cat /dev/null >$(PMTSimshstamp) ; fi

PMTSimclean ::
	$(cleanup_echo) objects PMTSim
	$(cleanup_silent) /bin/rm -f $(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o) $(patsubst %.o,%.dep,$(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o) $(patsubst %.o,%.d.stamp,$(bin)PMTSDMgr.o $(bin)PMTEfficiencyTable.o $(bin)junoHit_PMT.o $(bin)Tub3inchPMTV2Manager.o $(bin)Hello3inchPMTManager.o $(bin)Tub3inchPMTV3Manager.o $(bin)dyw_PMT_LogicalVolume.o $(bin)R3600PMTTubeManager.o $(bin)PMTHitMerger.o $(bin)dywEllipsoid.o $(bin)Ham8inchPMTManager.o $(bin)R12860_PMTSolid.o $(bin)HamamatsuR12860PMTManager.o $(bin)NNVT_MCPPMT_PMTSolid.o $(bin)ExplosionProofManager.o $(bin)junoSD_PMT.o $(bin)PMTEfficiency.o $(bin)MCP8inchPMTManager.o $(bin)PMTMaskConstruction.o $(bin)HamamatsuMaskManager.o $(bin)R12860OnlyFront_PMTSolid.o $(bin)R12860OnlyFrontPMTManager.o $(bin)junoPMTOpticalModel.o $(bin)HZC9inchPMTManager.o $(bin)Tub3inchPMTSolid.o $(bin)NNVTMaskManager.o $(bin)junoHit_PMT_muon.o $(bin)MCP20inchPMTManager.o $(bin)NNVTMCPPMTManager.o $(bin)Tub3inchPMTV2Solid.o $(bin)Tub3inchPMTV3Solid.o $(bin)R12860MaskManager.o $(bin)Hello8inchPMTManager.o $(bin)R12860_TorusPMTSolid.o $(bin)R12860TorusPMTManager.o $(bin)Hamamatsu_R12860_PMTSolid.o $(bin)R12860PMTManager.o $(bin)HelloPMTCoverManager.o $(bin)Hello_PMTSolid.o $(bin)HelloPMTManager.o $(bin)ExplosionProofSolid.o $(bin)dywPMTOpticalModel.o $(bin)R12860OnlyFrontMaskManager.o $(bin)R3600PMTManager.o $(bin)dywTorusStack.o $(bin)junoSD_PMT_v2.o $(bin)Tub3inchPMTManager.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf PMTSim_deps PMTSim_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
PMTSiminstallname = $(library_prefix)PMTSim$(library_suffix).$(shlibsuffix)

PMTSim :: PMTSiminstall ;

install :: PMTSiminstall ;

PMTSiminstall :: $(install_dir)/$(PMTSiminstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(PMTSiminstallname) :: $(bin)$(PMTSiminstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(PMTSiminstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##PMTSimclean :: PMTSimuninstall

uninstall :: PMTSimuninstall ;

PMTSimuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(PMTSiminstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),PMTSimprototype)

$(bin)PMTSim_dependencies.make : $(use_requirements) $(cmt_final_setup_PMTSim)
	$(echo) "(PMTSim.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)PMTSDMgr.cc $(src)PMTEfficiencyTable.cc $(src)junoHit_PMT.cc $(src)Tub3inchPMTV2Manager.cc $(src)Hello3inchPMTManager.cc $(src)Tub3inchPMTV3Manager.cc $(src)dyw_PMT_LogicalVolume.cc $(src)R3600PMTTubeManager.cc $(src)PMTHitMerger.cc $(src)dywEllipsoid.cc $(src)Ham8inchPMTManager.cc $(src)R12860_PMTSolid.cc $(src)HamamatsuR12860PMTManager.cc $(src)NNVT_MCPPMT_PMTSolid.cc $(src)ExplosionProofManager.cc $(src)junoSD_PMT.cc $(src)PMTEfficiency.cc $(src)MCP8inchPMTManager.cc $(src)PMTMaskConstruction.cc $(src)HamamatsuMaskManager.cc $(src)R12860OnlyFront_PMTSolid.cc $(src)R12860OnlyFrontPMTManager.cc $(src)junoPMTOpticalModel.cc $(src)HZC9inchPMTManager.cc $(src)Tub3inchPMTSolid.cc $(src)NNVTMaskManager.cc $(src)junoHit_PMT_muon.cc $(src)MCP20inchPMTManager.cc $(src)NNVTMCPPMTManager.cc $(src)Tub3inchPMTV2Solid.cc $(src)Tub3inchPMTV3Solid.cc $(src)R12860MaskManager.cc $(src)Hello8inchPMTManager.cc $(src)R12860_TorusPMTSolid.cc $(src)R12860TorusPMTManager.cc $(src)Hamamatsu_R12860_PMTSolid.cc $(src)R12860PMTManager.cc $(src)HelloPMTCoverManager.cc $(src)Hello_PMTSolid.cc $(src)HelloPMTManager.cc $(src)ExplosionProofSolid.cc $(src)dywPMTOpticalModel.cc $(src)R12860OnlyFrontMaskManager.cc $(src)R3600PMTManager.cc $(src)dywTorusStack.cc $(src)junoSD_PMT_v2.cc $(src)Tub3inchPMTManager.cc -end_all $(includes) $(app_PMTSim_cppflags) $(lib_PMTSim_cppflags) -name=PMTSim $? -f=$(cmt_dependencies_in_PMTSim) -without_cmt

-include $(bin)PMTSim_dependencies.make

endif
endif
endif

PMTSimclean ::
	$(cleanup_silent) \rm -rf $(bin)PMTSim_deps $(bin)PMTSim_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PMTSDMgr.d

$(bin)$(binobj)PMTSDMgr.d :

$(bin)$(binobj)PMTSDMgr.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)PMTSDMgr.o : $(src)PMTSDMgr.cc
	$(cpp_echo) $(src)PMTSDMgr.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTSDMgr_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTSDMgr_cppflags) $(PMTSDMgr_cc_cppflags)  $(src)PMTSDMgr.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(PMTSDMgr_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)PMTSDMgr.cc

$(bin)$(binobj)PMTSDMgr.o : $(PMTSDMgr_cc_dependencies)
	$(cpp_echo) $(src)PMTSDMgr.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTSDMgr_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTSDMgr_cppflags) $(PMTSDMgr_cc_cppflags)  $(src)PMTSDMgr.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PMTEfficiencyTable.d

$(bin)$(binobj)PMTEfficiencyTable.d :

$(bin)$(binobj)PMTEfficiencyTable.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)PMTEfficiencyTable.o : $(src)PMTEfficiencyTable.cc
	$(cpp_echo) $(src)PMTEfficiencyTable.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTEfficiencyTable_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTEfficiencyTable_cppflags) $(PMTEfficiencyTable_cc_cppflags)  $(src)PMTEfficiencyTable.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(PMTEfficiencyTable_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)PMTEfficiencyTable.cc

$(bin)$(binobj)PMTEfficiencyTable.o : $(PMTEfficiencyTable_cc_dependencies)
	$(cpp_echo) $(src)PMTEfficiencyTable.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTEfficiencyTable_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTEfficiencyTable_cppflags) $(PMTEfficiencyTable_cc_cppflags)  $(src)PMTEfficiencyTable.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)junoHit_PMT.d

$(bin)$(binobj)junoHit_PMT.d :

$(bin)$(binobj)junoHit_PMT.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)junoHit_PMT.o : $(src)junoHit_PMT.cc
	$(cpp_echo) $(src)junoHit_PMT.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoHit_PMT_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoHit_PMT_cppflags) $(junoHit_PMT_cc_cppflags)  $(src)junoHit_PMT.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(junoHit_PMT_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)junoHit_PMT.cc

$(bin)$(binobj)junoHit_PMT.o : $(junoHit_PMT_cc_dependencies)
	$(cpp_echo) $(src)junoHit_PMT.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoHit_PMT_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoHit_PMT_cppflags) $(junoHit_PMT_cc_cppflags)  $(src)junoHit_PMT.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Tub3inchPMTV2Manager.d

$(bin)$(binobj)Tub3inchPMTV2Manager.d :

$(bin)$(binobj)Tub3inchPMTV2Manager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Tub3inchPMTV2Manager.o : $(src)Tub3inchPMTV2Manager.cc
	$(cpp_echo) $(src)Tub3inchPMTV2Manager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV2Manager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV2Manager_cppflags) $(Tub3inchPMTV2Manager_cc_cppflags)  $(src)Tub3inchPMTV2Manager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Tub3inchPMTV2Manager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Tub3inchPMTV2Manager.cc

$(bin)$(binobj)Tub3inchPMTV2Manager.o : $(Tub3inchPMTV2Manager_cc_dependencies)
	$(cpp_echo) $(src)Tub3inchPMTV2Manager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV2Manager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV2Manager_cppflags) $(Tub3inchPMTV2Manager_cc_cppflags)  $(src)Tub3inchPMTV2Manager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Hello3inchPMTManager.d

$(bin)$(binobj)Hello3inchPMTManager.d :

$(bin)$(binobj)Hello3inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Hello3inchPMTManager.o : $(src)Hello3inchPMTManager.cc
	$(cpp_echo) $(src)Hello3inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hello3inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hello3inchPMTManager_cppflags) $(Hello3inchPMTManager_cc_cppflags)  $(src)Hello3inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Hello3inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Hello3inchPMTManager.cc

$(bin)$(binobj)Hello3inchPMTManager.o : $(Hello3inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)Hello3inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hello3inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hello3inchPMTManager_cppflags) $(Hello3inchPMTManager_cc_cppflags)  $(src)Hello3inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Tub3inchPMTV3Manager.d

$(bin)$(binobj)Tub3inchPMTV3Manager.d :

$(bin)$(binobj)Tub3inchPMTV3Manager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Tub3inchPMTV3Manager.o : $(src)Tub3inchPMTV3Manager.cc
	$(cpp_echo) $(src)Tub3inchPMTV3Manager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV3Manager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV3Manager_cppflags) $(Tub3inchPMTV3Manager_cc_cppflags)  $(src)Tub3inchPMTV3Manager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Tub3inchPMTV3Manager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Tub3inchPMTV3Manager.cc

$(bin)$(binobj)Tub3inchPMTV3Manager.o : $(Tub3inchPMTV3Manager_cc_dependencies)
	$(cpp_echo) $(src)Tub3inchPMTV3Manager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV3Manager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV3Manager_cppflags) $(Tub3inchPMTV3Manager_cc_cppflags)  $(src)Tub3inchPMTV3Manager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)dyw_PMT_LogicalVolume.d

$(bin)$(binobj)dyw_PMT_LogicalVolume.d :

$(bin)$(binobj)dyw_PMT_LogicalVolume.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)dyw_PMT_LogicalVolume.o : $(src)dyw_PMT_LogicalVolume.cc
	$(cpp_echo) $(src)dyw_PMT_LogicalVolume.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dyw_PMT_LogicalVolume_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dyw_PMT_LogicalVolume_cppflags) $(dyw_PMT_LogicalVolume_cc_cppflags)  $(src)dyw_PMT_LogicalVolume.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(dyw_PMT_LogicalVolume_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)dyw_PMT_LogicalVolume.cc

$(bin)$(binobj)dyw_PMT_LogicalVolume.o : $(dyw_PMT_LogicalVolume_cc_dependencies)
	$(cpp_echo) $(src)dyw_PMT_LogicalVolume.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dyw_PMT_LogicalVolume_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dyw_PMT_LogicalVolume_cppflags) $(dyw_PMT_LogicalVolume_cc_cppflags)  $(src)dyw_PMT_LogicalVolume.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R3600PMTTubeManager.d

$(bin)$(binobj)R3600PMTTubeManager.d :

$(bin)$(binobj)R3600PMTTubeManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R3600PMTTubeManager.o : $(src)R3600PMTTubeManager.cc
	$(cpp_echo) $(src)R3600PMTTubeManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R3600PMTTubeManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R3600PMTTubeManager_cppflags) $(R3600PMTTubeManager_cc_cppflags)  $(src)R3600PMTTubeManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R3600PMTTubeManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R3600PMTTubeManager.cc

$(bin)$(binobj)R3600PMTTubeManager.o : $(R3600PMTTubeManager_cc_dependencies)
	$(cpp_echo) $(src)R3600PMTTubeManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R3600PMTTubeManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R3600PMTTubeManager_cppflags) $(R3600PMTTubeManager_cc_cppflags)  $(src)R3600PMTTubeManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PMTHitMerger.d

$(bin)$(binobj)PMTHitMerger.d :

$(bin)$(binobj)PMTHitMerger.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)PMTHitMerger.o : $(src)PMTHitMerger.cc
	$(cpp_echo) $(src)PMTHitMerger.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTHitMerger_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTHitMerger_cppflags) $(PMTHitMerger_cc_cppflags)  $(src)PMTHitMerger.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(PMTHitMerger_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)PMTHitMerger.cc

$(bin)$(binobj)PMTHitMerger.o : $(PMTHitMerger_cc_dependencies)
	$(cpp_echo) $(src)PMTHitMerger.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTHitMerger_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTHitMerger_cppflags) $(PMTHitMerger_cc_cppflags)  $(src)PMTHitMerger.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)dywEllipsoid.d

$(bin)$(binobj)dywEllipsoid.d :

$(bin)$(binobj)dywEllipsoid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)dywEllipsoid.o : $(src)dywEllipsoid.cc
	$(cpp_echo) $(src)dywEllipsoid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dywEllipsoid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dywEllipsoid_cppflags) $(dywEllipsoid_cc_cppflags)  $(src)dywEllipsoid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(dywEllipsoid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)dywEllipsoid.cc

$(bin)$(binobj)dywEllipsoid.o : $(dywEllipsoid_cc_dependencies)
	$(cpp_echo) $(src)dywEllipsoid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dywEllipsoid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dywEllipsoid_cppflags) $(dywEllipsoid_cc_cppflags)  $(src)dywEllipsoid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Ham8inchPMTManager.d

$(bin)$(binobj)Ham8inchPMTManager.d :

$(bin)$(binobj)Ham8inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Ham8inchPMTManager.o : $(src)Ham8inchPMTManager.cc
	$(cpp_echo) $(src)Ham8inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Ham8inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Ham8inchPMTManager_cppflags) $(Ham8inchPMTManager_cc_cppflags)  $(src)Ham8inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Ham8inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Ham8inchPMTManager.cc

$(bin)$(binobj)Ham8inchPMTManager.o : $(Ham8inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)Ham8inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Ham8inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Ham8inchPMTManager_cppflags) $(Ham8inchPMTManager_cc_cppflags)  $(src)Ham8inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860_PMTSolid.d

$(bin)$(binobj)R12860_PMTSolid.d :

$(bin)$(binobj)R12860_PMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860_PMTSolid.o : $(src)R12860_PMTSolid.cc
	$(cpp_echo) $(src)R12860_PMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860_PMTSolid_cppflags) $(R12860_PMTSolid_cc_cppflags)  $(src)R12860_PMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860_PMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860_PMTSolid.cc

$(bin)$(binobj)R12860_PMTSolid.o : $(R12860_PMTSolid_cc_dependencies)
	$(cpp_echo) $(src)R12860_PMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860_PMTSolid_cppflags) $(R12860_PMTSolid_cc_cppflags)  $(src)R12860_PMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HamamatsuR12860PMTManager.d

$(bin)$(binobj)HamamatsuR12860PMTManager.d :

$(bin)$(binobj)HamamatsuR12860PMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)HamamatsuR12860PMTManager.o : $(src)HamamatsuR12860PMTManager.cc
	$(cpp_echo) $(src)HamamatsuR12860PMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HamamatsuR12860PMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HamamatsuR12860PMTManager_cppflags) $(HamamatsuR12860PMTManager_cc_cppflags)  $(src)HamamatsuR12860PMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(HamamatsuR12860PMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)HamamatsuR12860PMTManager.cc

$(bin)$(binobj)HamamatsuR12860PMTManager.o : $(HamamatsuR12860PMTManager_cc_dependencies)
	$(cpp_echo) $(src)HamamatsuR12860PMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HamamatsuR12860PMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HamamatsuR12860PMTManager_cppflags) $(HamamatsuR12860PMTManager_cc_cppflags)  $(src)HamamatsuR12860PMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)NNVT_MCPPMT_PMTSolid.d

$(bin)$(binobj)NNVT_MCPPMT_PMTSolid.d :

$(bin)$(binobj)NNVT_MCPPMT_PMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)NNVT_MCPPMT_PMTSolid.o : $(src)NNVT_MCPPMT_PMTSolid.cc
	$(cpp_echo) $(src)NNVT_MCPPMT_PMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(NNVT_MCPPMT_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(NNVT_MCPPMT_PMTSolid_cppflags) $(NNVT_MCPPMT_PMTSolid_cc_cppflags)  $(src)NNVT_MCPPMT_PMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(NNVT_MCPPMT_PMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)NNVT_MCPPMT_PMTSolid.cc

$(bin)$(binobj)NNVT_MCPPMT_PMTSolid.o : $(NNVT_MCPPMT_PMTSolid_cc_dependencies)
	$(cpp_echo) $(src)NNVT_MCPPMT_PMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(NNVT_MCPPMT_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(NNVT_MCPPMT_PMTSolid_cppflags) $(NNVT_MCPPMT_PMTSolid_cc_cppflags)  $(src)NNVT_MCPPMT_PMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)ExplosionProofManager.d

$(bin)$(binobj)ExplosionProofManager.d :

$(bin)$(binobj)ExplosionProofManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)ExplosionProofManager.o : $(src)ExplosionProofManager.cc
	$(cpp_echo) $(src)ExplosionProofManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(ExplosionProofManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(ExplosionProofManager_cppflags) $(ExplosionProofManager_cc_cppflags)  $(src)ExplosionProofManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(ExplosionProofManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)ExplosionProofManager.cc

$(bin)$(binobj)ExplosionProofManager.o : $(ExplosionProofManager_cc_dependencies)
	$(cpp_echo) $(src)ExplosionProofManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(ExplosionProofManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(ExplosionProofManager_cppflags) $(ExplosionProofManager_cc_cppflags)  $(src)ExplosionProofManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)junoSD_PMT.d

$(bin)$(binobj)junoSD_PMT.d :

$(bin)$(binobj)junoSD_PMT.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)junoSD_PMT.o : $(src)junoSD_PMT.cc
	$(cpp_echo) $(src)junoSD_PMT.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoSD_PMT_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoSD_PMT_cppflags) $(junoSD_PMT_cc_cppflags)  $(src)junoSD_PMT.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(junoSD_PMT_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)junoSD_PMT.cc

$(bin)$(binobj)junoSD_PMT.o : $(junoSD_PMT_cc_dependencies)
	$(cpp_echo) $(src)junoSD_PMT.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoSD_PMT_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoSD_PMT_cppflags) $(junoSD_PMT_cc_cppflags)  $(src)junoSD_PMT.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PMTEfficiency.d

$(bin)$(binobj)PMTEfficiency.d :

$(bin)$(binobj)PMTEfficiency.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)PMTEfficiency.o : $(src)PMTEfficiency.cc
	$(cpp_echo) $(src)PMTEfficiency.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTEfficiency_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTEfficiency_cppflags) $(PMTEfficiency_cc_cppflags)  $(src)PMTEfficiency.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(PMTEfficiency_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)PMTEfficiency.cc

$(bin)$(binobj)PMTEfficiency.o : $(PMTEfficiency_cc_dependencies)
	$(cpp_echo) $(src)PMTEfficiency.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTEfficiency_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTEfficiency_cppflags) $(PMTEfficiency_cc_cppflags)  $(src)PMTEfficiency.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MCP8inchPMTManager.d

$(bin)$(binobj)MCP8inchPMTManager.d :

$(bin)$(binobj)MCP8inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)MCP8inchPMTManager.o : $(src)MCP8inchPMTManager.cc
	$(cpp_echo) $(src)MCP8inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(MCP8inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(MCP8inchPMTManager_cppflags) $(MCP8inchPMTManager_cc_cppflags)  $(src)MCP8inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(MCP8inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)MCP8inchPMTManager.cc

$(bin)$(binobj)MCP8inchPMTManager.o : $(MCP8inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)MCP8inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(MCP8inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(MCP8inchPMTManager_cppflags) $(MCP8inchPMTManager_cc_cppflags)  $(src)MCP8inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PMTMaskConstruction.d

$(bin)$(binobj)PMTMaskConstruction.d :

$(bin)$(binobj)PMTMaskConstruction.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)PMTMaskConstruction.o : $(src)PMTMaskConstruction.cc
	$(cpp_echo) $(src)PMTMaskConstruction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTMaskConstruction_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTMaskConstruction_cppflags) $(PMTMaskConstruction_cc_cppflags)  $(src)PMTMaskConstruction.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(PMTMaskConstruction_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)PMTMaskConstruction.cc

$(bin)$(binobj)PMTMaskConstruction.o : $(PMTMaskConstruction_cc_dependencies)
	$(cpp_echo) $(src)PMTMaskConstruction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(PMTMaskConstruction_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(PMTMaskConstruction_cppflags) $(PMTMaskConstruction_cc_cppflags)  $(src)PMTMaskConstruction.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HamamatsuMaskManager.d

$(bin)$(binobj)HamamatsuMaskManager.d :

$(bin)$(binobj)HamamatsuMaskManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)HamamatsuMaskManager.o : $(src)HamamatsuMaskManager.cc
	$(cpp_echo) $(src)HamamatsuMaskManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HamamatsuMaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HamamatsuMaskManager_cppflags) $(HamamatsuMaskManager_cc_cppflags)  $(src)HamamatsuMaskManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(HamamatsuMaskManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)HamamatsuMaskManager.cc

$(bin)$(binobj)HamamatsuMaskManager.o : $(HamamatsuMaskManager_cc_dependencies)
	$(cpp_echo) $(src)HamamatsuMaskManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HamamatsuMaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HamamatsuMaskManager_cppflags) $(HamamatsuMaskManager_cc_cppflags)  $(src)HamamatsuMaskManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860OnlyFront_PMTSolid.d

$(bin)$(binobj)R12860OnlyFront_PMTSolid.d :

$(bin)$(binobj)R12860OnlyFront_PMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860OnlyFront_PMTSolid.o : $(src)R12860OnlyFront_PMTSolid.cc
	$(cpp_echo) $(src)R12860OnlyFront_PMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860OnlyFront_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860OnlyFront_PMTSolid_cppflags) $(R12860OnlyFront_PMTSolid_cc_cppflags)  $(src)R12860OnlyFront_PMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860OnlyFront_PMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860OnlyFront_PMTSolid.cc

$(bin)$(binobj)R12860OnlyFront_PMTSolid.o : $(R12860OnlyFront_PMTSolid_cc_dependencies)
	$(cpp_echo) $(src)R12860OnlyFront_PMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860OnlyFront_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860OnlyFront_PMTSolid_cppflags) $(R12860OnlyFront_PMTSolid_cc_cppflags)  $(src)R12860OnlyFront_PMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860OnlyFrontPMTManager.d

$(bin)$(binobj)R12860OnlyFrontPMTManager.d :

$(bin)$(binobj)R12860OnlyFrontPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860OnlyFrontPMTManager.o : $(src)R12860OnlyFrontPMTManager.cc
	$(cpp_echo) $(src)R12860OnlyFrontPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860OnlyFrontPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860OnlyFrontPMTManager_cppflags) $(R12860OnlyFrontPMTManager_cc_cppflags)  $(src)R12860OnlyFrontPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860OnlyFrontPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860OnlyFrontPMTManager.cc

$(bin)$(binobj)R12860OnlyFrontPMTManager.o : $(R12860OnlyFrontPMTManager_cc_dependencies)
	$(cpp_echo) $(src)R12860OnlyFrontPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860OnlyFrontPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860OnlyFrontPMTManager_cppflags) $(R12860OnlyFrontPMTManager_cc_cppflags)  $(src)R12860OnlyFrontPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)junoPMTOpticalModel.d

$(bin)$(binobj)junoPMTOpticalModel.d :

$(bin)$(binobj)junoPMTOpticalModel.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)junoPMTOpticalModel.o : $(src)junoPMTOpticalModel.cc
	$(cpp_echo) $(src)junoPMTOpticalModel.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoPMTOpticalModel_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoPMTOpticalModel_cppflags) $(junoPMTOpticalModel_cc_cppflags)  $(src)junoPMTOpticalModel.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(junoPMTOpticalModel_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)junoPMTOpticalModel.cc

$(bin)$(binobj)junoPMTOpticalModel.o : $(junoPMTOpticalModel_cc_dependencies)
	$(cpp_echo) $(src)junoPMTOpticalModel.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoPMTOpticalModel_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoPMTOpticalModel_cppflags) $(junoPMTOpticalModel_cc_cppflags)  $(src)junoPMTOpticalModel.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HZC9inchPMTManager.d

$(bin)$(binobj)HZC9inchPMTManager.d :

$(bin)$(binobj)HZC9inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)HZC9inchPMTManager.o : $(src)HZC9inchPMTManager.cc
	$(cpp_echo) $(src)HZC9inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HZC9inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HZC9inchPMTManager_cppflags) $(HZC9inchPMTManager_cc_cppflags)  $(src)HZC9inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(HZC9inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)HZC9inchPMTManager.cc

$(bin)$(binobj)HZC9inchPMTManager.o : $(HZC9inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)HZC9inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HZC9inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HZC9inchPMTManager_cppflags) $(HZC9inchPMTManager_cc_cppflags)  $(src)HZC9inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Tub3inchPMTSolid.d

$(bin)$(binobj)Tub3inchPMTSolid.d :

$(bin)$(binobj)Tub3inchPMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Tub3inchPMTSolid.o : $(src)Tub3inchPMTSolid.cc
	$(cpp_echo) $(src)Tub3inchPMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTSolid_cppflags) $(Tub3inchPMTSolid_cc_cppflags)  $(src)Tub3inchPMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Tub3inchPMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Tub3inchPMTSolid.cc

$(bin)$(binobj)Tub3inchPMTSolid.o : $(Tub3inchPMTSolid_cc_dependencies)
	$(cpp_echo) $(src)Tub3inchPMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTSolid_cppflags) $(Tub3inchPMTSolid_cc_cppflags)  $(src)Tub3inchPMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)NNVTMaskManager.d

$(bin)$(binobj)NNVTMaskManager.d :

$(bin)$(binobj)NNVTMaskManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)NNVTMaskManager.o : $(src)NNVTMaskManager.cc
	$(cpp_echo) $(src)NNVTMaskManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(NNVTMaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(NNVTMaskManager_cppflags) $(NNVTMaskManager_cc_cppflags)  $(src)NNVTMaskManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(NNVTMaskManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)NNVTMaskManager.cc

$(bin)$(binobj)NNVTMaskManager.o : $(NNVTMaskManager_cc_dependencies)
	$(cpp_echo) $(src)NNVTMaskManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(NNVTMaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(NNVTMaskManager_cppflags) $(NNVTMaskManager_cc_cppflags)  $(src)NNVTMaskManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)junoHit_PMT_muon.d

$(bin)$(binobj)junoHit_PMT_muon.d :

$(bin)$(binobj)junoHit_PMT_muon.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)junoHit_PMT_muon.o : $(src)junoHit_PMT_muon.cc
	$(cpp_echo) $(src)junoHit_PMT_muon.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoHit_PMT_muon_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoHit_PMT_muon_cppflags) $(junoHit_PMT_muon_cc_cppflags)  $(src)junoHit_PMT_muon.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(junoHit_PMT_muon_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)junoHit_PMT_muon.cc

$(bin)$(binobj)junoHit_PMT_muon.o : $(junoHit_PMT_muon_cc_dependencies)
	$(cpp_echo) $(src)junoHit_PMT_muon.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoHit_PMT_muon_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoHit_PMT_muon_cppflags) $(junoHit_PMT_muon_cc_cppflags)  $(src)junoHit_PMT_muon.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MCP20inchPMTManager.d

$(bin)$(binobj)MCP20inchPMTManager.d :

$(bin)$(binobj)MCP20inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)MCP20inchPMTManager.o : $(src)MCP20inchPMTManager.cc
	$(cpp_echo) $(src)MCP20inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(MCP20inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(MCP20inchPMTManager_cppflags) $(MCP20inchPMTManager_cc_cppflags)  $(src)MCP20inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(MCP20inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)MCP20inchPMTManager.cc

$(bin)$(binobj)MCP20inchPMTManager.o : $(MCP20inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)MCP20inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(MCP20inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(MCP20inchPMTManager_cppflags) $(MCP20inchPMTManager_cc_cppflags)  $(src)MCP20inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)NNVTMCPPMTManager.d

$(bin)$(binobj)NNVTMCPPMTManager.d :

$(bin)$(binobj)NNVTMCPPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)NNVTMCPPMTManager.o : $(src)NNVTMCPPMTManager.cc
	$(cpp_echo) $(src)NNVTMCPPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(NNVTMCPPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(NNVTMCPPMTManager_cppflags) $(NNVTMCPPMTManager_cc_cppflags)  $(src)NNVTMCPPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(NNVTMCPPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)NNVTMCPPMTManager.cc

$(bin)$(binobj)NNVTMCPPMTManager.o : $(NNVTMCPPMTManager_cc_dependencies)
	$(cpp_echo) $(src)NNVTMCPPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(NNVTMCPPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(NNVTMCPPMTManager_cppflags) $(NNVTMCPPMTManager_cc_cppflags)  $(src)NNVTMCPPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Tub3inchPMTV2Solid.d

$(bin)$(binobj)Tub3inchPMTV2Solid.d :

$(bin)$(binobj)Tub3inchPMTV2Solid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Tub3inchPMTV2Solid.o : $(src)Tub3inchPMTV2Solid.cc
	$(cpp_echo) $(src)Tub3inchPMTV2Solid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV2Solid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV2Solid_cppflags) $(Tub3inchPMTV2Solid_cc_cppflags)  $(src)Tub3inchPMTV2Solid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Tub3inchPMTV2Solid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Tub3inchPMTV2Solid.cc

$(bin)$(binobj)Tub3inchPMTV2Solid.o : $(Tub3inchPMTV2Solid_cc_dependencies)
	$(cpp_echo) $(src)Tub3inchPMTV2Solid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV2Solid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV2Solid_cppflags) $(Tub3inchPMTV2Solid_cc_cppflags)  $(src)Tub3inchPMTV2Solid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Tub3inchPMTV3Solid.d

$(bin)$(binobj)Tub3inchPMTV3Solid.d :

$(bin)$(binobj)Tub3inchPMTV3Solid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Tub3inchPMTV3Solid.o : $(src)Tub3inchPMTV3Solid.cc
	$(cpp_echo) $(src)Tub3inchPMTV3Solid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV3Solid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV3Solid_cppflags) $(Tub3inchPMTV3Solid_cc_cppflags)  $(src)Tub3inchPMTV3Solid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Tub3inchPMTV3Solid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Tub3inchPMTV3Solid.cc

$(bin)$(binobj)Tub3inchPMTV3Solid.o : $(Tub3inchPMTV3Solid_cc_dependencies)
	$(cpp_echo) $(src)Tub3inchPMTV3Solid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTV3Solid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTV3Solid_cppflags) $(Tub3inchPMTV3Solid_cc_cppflags)  $(src)Tub3inchPMTV3Solid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860MaskManager.d

$(bin)$(binobj)R12860MaskManager.d :

$(bin)$(binobj)R12860MaskManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860MaskManager.o : $(src)R12860MaskManager.cc
	$(cpp_echo) $(src)R12860MaskManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860MaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860MaskManager_cppflags) $(R12860MaskManager_cc_cppflags)  $(src)R12860MaskManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860MaskManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860MaskManager.cc

$(bin)$(binobj)R12860MaskManager.o : $(R12860MaskManager_cc_dependencies)
	$(cpp_echo) $(src)R12860MaskManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860MaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860MaskManager_cppflags) $(R12860MaskManager_cc_cppflags)  $(src)R12860MaskManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Hello8inchPMTManager.d

$(bin)$(binobj)Hello8inchPMTManager.d :

$(bin)$(binobj)Hello8inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Hello8inchPMTManager.o : $(src)Hello8inchPMTManager.cc
	$(cpp_echo) $(src)Hello8inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hello8inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hello8inchPMTManager_cppflags) $(Hello8inchPMTManager_cc_cppflags)  $(src)Hello8inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Hello8inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Hello8inchPMTManager.cc

$(bin)$(binobj)Hello8inchPMTManager.o : $(Hello8inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)Hello8inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hello8inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hello8inchPMTManager_cppflags) $(Hello8inchPMTManager_cc_cppflags)  $(src)Hello8inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860_TorusPMTSolid.d

$(bin)$(binobj)R12860_TorusPMTSolid.d :

$(bin)$(binobj)R12860_TorusPMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860_TorusPMTSolid.o : $(src)R12860_TorusPMTSolid.cc
	$(cpp_echo) $(src)R12860_TorusPMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860_TorusPMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860_TorusPMTSolid_cppflags) $(R12860_TorusPMTSolid_cc_cppflags)  $(src)R12860_TorusPMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860_TorusPMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860_TorusPMTSolid.cc

$(bin)$(binobj)R12860_TorusPMTSolid.o : $(R12860_TorusPMTSolid_cc_dependencies)
	$(cpp_echo) $(src)R12860_TorusPMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860_TorusPMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860_TorusPMTSolid_cppflags) $(R12860_TorusPMTSolid_cc_cppflags)  $(src)R12860_TorusPMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860TorusPMTManager.d

$(bin)$(binobj)R12860TorusPMTManager.d :

$(bin)$(binobj)R12860TorusPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860TorusPMTManager.o : $(src)R12860TorusPMTManager.cc
	$(cpp_echo) $(src)R12860TorusPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860TorusPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860TorusPMTManager_cppflags) $(R12860TorusPMTManager_cc_cppflags)  $(src)R12860TorusPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860TorusPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860TorusPMTManager.cc

$(bin)$(binobj)R12860TorusPMTManager.o : $(R12860TorusPMTManager_cc_dependencies)
	$(cpp_echo) $(src)R12860TorusPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860TorusPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860TorusPMTManager_cppflags) $(R12860TorusPMTManager_cc_cppflags)  $(src)R12860TorusPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Hamamatsu_R12860_PMTSolid.d

$(bin)$(binobj)Hamamatsu_R12860_PMTSolid.d :

$(bin)$(binobj)Hamamatsu_R12860_PMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Hamamatsu_R12860_PMTSolid.o : $(src)Hamamatsu_R12860_PMTSolid.cc
	$(cpp_echo) $(src)Hamamatsu_R12860_PMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hamamatsu_R12860_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hamamatsu_R12860_PMTSolid_cppflags) $(Hamamatsu_R12860_PMTSolid_cc_cppflags)  $(src)Hamamatsu_R12860_PMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Hamamatsu_R12860_PMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Hamamatsu_R12860_PMTSolid.cc

$(bin)$(binobj)Hamamatsu_R12860_PMTSolid.o : $(Hamamatsu_R12860_PMTSolid_cc_dependencies)
	$(cpp_echo) $(src)Hamamatsu_R12860_PMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hamamatsu_R12860_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hamamatsu_R12860_PMTSolid_cppflags) $(Hamamatsu_R12860_PMTSolid_cc_cppflags)  $(src)Hamamatsu_R12860_PMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860PMTManager.d

$(bin)$(binobj)R12860PMTManager.d :

$(bin)$(binobj)R12860PMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860PMTManager.o : $(src)R12860PMTManager.cc
	$(cpp_echo) $(src)R12860PMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860PMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860PMTManager_cppflags) $(R12860PMTManager_cc_cppflags)  $(src)R12860PMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860PMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860PMTManager.cc

$(bin)$(binobj)R12860PMTManager.o : $(R12860PMTManager_cc_dependencies)
	$(cpp_echo) $(src)R12860PMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860PMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860PMTManager_cppflags) $(R12860PMTManager_cc_cppflags)  $(src)R12860PMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HelloPMTCoverManager.d

$(bin)$(binobj)HelloPMTCoverManager.d :

$(bin)$(binobj)HelloPMTCoverManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)HelloPMTCoverManager.o : $(src)HelloPMTCoverManager.cc
	$(cpp_echo) $(src)HelloPMTCoverManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HelloPMTCoverManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HelloPMTCoverManager_cppflags) $(HelloPMTCoverManager_cc_cppflags)  $(src)HelloPMTCoverManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(HelloPMTCoverManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)HelloPMTCoverManager.cc

$(bin)$(binobj)HelloPMTCoverManager.o : $(HelloPMTCoverManager_cc_dependencies)
	$(cpp_echo) $(src)HelloPMTCoverManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HelloPMTCoverManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HelloPMTCoverManager_cppflags) $(HelloPMTCoverManager_cc_cppflags)  $(src)HelloPMTCoverManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Hello_PMTSolid.d

$(bin)$(binobj)Hello_PMTSolid.d :

$(bin)$(binobj)Hello_PMTSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Hello_PMTSolid.o : $(src)Hello_PMTSolid.cc
	$(cpp_echo) $(src)Hello_PMTSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hello_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hello_PMTSolid_cppflags) $(Hello_PMTSolid_cc_cppflags)  $(src)Hello_PMTSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Hello_PMTSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Hello_PMTSolid.cc

$(bin)$(binobj)Hello_PMTSolid.o : $(Hello_PMTSolid_cc_dependencies)
	$(cpp_echo) $(src)Hello_PMTSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Hello_PMTSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Hello_PMTSolid_cppflags) $(Hello_PMTSolid_cc_cppflags)  $(src)Hello_PMTSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HelloPMTManager.d

$(bin)$(binobj)HelloPMTManager.d :

$(bin)$(binobj)HelloPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)HelloPMTManager.o : $(src)HelloPMTManager.cc
	$(cpp_echo) $(src)HelloPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HelloPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HelloPMTManager_cppflags) $(HelloPMTManager_cc_cppflags)  $(src)HelloPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(HelloPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)HelloPMTManager.cc

$(bin)$(binobj)HelloPMTManager.o : $(HelloPMTManager_cc_dependencies)
	$(cpp_echo) $(src)HelloPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(HelloPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(HelloPMTManager_cppflags) $(HelloPMTManager_cc_cppflags)  $(src)HelloPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)ExplosionProofSolid.d

$(bin)$(binobj)ExplosionProofSolid.d :

$(bin)$(binobj)ExplosionProofSolid.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)ExplosionProofSolid.o : $(src)ExplosionProofSolid.cc
	$(cpp_echo) $(src)ExplosionProofSolid.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(ExplosionProofSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(ExplosionProofSolid_cppflags) $(ExplosionProofSolid_cc_cppflags)  $(src)ExplosionProofSolid.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(ExplosionProofSolid_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)ExplosionProofSolid.cc

$(bin)$(binobj)ExplosionProofSolid.o : $(ExplosionProofSolid_cc_dependencies)
	$(cpp_echo) $(src)ExplosionProofSolid.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(ExplosionProofSolid_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(ExplosionProofSolid_cppflags) $(ExplosionProofSolid_cc_cppflags)  $(src)ExplosionProofSolid.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)dywPMTOpticalModel.d

$(bin)$(binobj)dywPMTOpticalModel.d :

$(bin)$(binobj)dywPMTOpticalModel.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)dywPMTOpticalModel.o : $(src)dywPMTOpticalModel.cc
	$(cpp_echo) $(src)dywPMTOpticalModel.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dywPMTOpticalModel_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dywPMTOpticalModel_cppflags) $(dywPMTOpticalModel_cc_cppflags)  $(src)dywPMTOpticalModel.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(dywPMTOpticalModel_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)dywPMTOpticalModel.cc

$(bin)$(binobj)dywPMTOpticalModel.o : $(dywPMTOpticalModel_cc_dependencies)
	$(cpp_echo) $(src)dywPMTOpticalModel.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dywPMTOpticalModel_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dywPMTOpticalModel_cppflags) $(dywPMTOpticalModel_cc_cppflags)  $(src)dywPMTOpticalModel.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R12860OnlyFrontMaskManager.d

$(bin)$(binobj)R12860OnlyFrontMaskManager.d :

$(bin)$(binobj)R12860OnlyFrontMaskManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R12860OnlyFrontMaskManager.o : $(src)R12860OnlyFrontMaskManager.cc
	$(cpp_echo) $(src)R12860OnlyFrontMaskManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860OnlyFrontMaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860OnlyFrontMaskManager_cppflags) $(R12860OnlyFrontMaskManager_cc_cppflags)  $(src)R12860OnlyFrontMaskManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R12860OnlyFrontMaskManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R12860OnlyFrontMaskManager.cc

$(bin)$(binobj)R12860OnlyFrontMaskManager.o : $(R12860OnlyFrontMaskManager_cc_dependencies)
	$(cpp_echo) $(src)R12860OnlyFrontMaskManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R12860OnlyFrontMaskManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R12860OnlyFrontMaskManager_cppflags) $(R12860OnlyFrontMaskManager_cc_cppflags)  $(src)R12860OnlyFrontMaskManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)R3600PMTManager.d

$(bin)$(binobj)R3600PMTManager.d :

$(bin)$(binobj)R3600PMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)R3600PMTManager.o : $(src)R3600PMTManager.cc
	$(cpp_echo) $(src)R3600PMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R3600PMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R3600PMTManager_cppflags) $(R3600PMTManager_cc_cppflags)  $(src)R3600PMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(R3600PMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)R3600PMTManager.cc

$(bin)$(binobj)R3600PMTManager.o : $(R3600PMTManager_cc_dependencies)
	$(cpp_echo) $(src)R3600PMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(R3600PMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(R3600PMTManager_cppflags) $(R3600PMTManager_cc_cppflags)  $(src)R3600PMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)dywTorusStack.d

$(bin)$(binobj)dywTorusStack.d :

$(bin)$(binobj)dywTorusStack.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)dywTorusStack.o : $(src)dywTorusStack.cc
	$(cpp_echo) $(src)dywTorusStack.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dywTorusStack_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dywTorusStack_cppflags) $(dywTorusStack_cc_cppflags)  $(src)dywTorusStack.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(dywTorusStack_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)dywTorusStack.cc

$(bin)$(binobj)dywTorusStack.o : $(dywTorusStack_cc_dependencies)
	$(cpp_echo) $(src)dywTorusStack.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(dywTorusStack_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(dywTorusStack_cppflags) $(dywTorusStack_cc_cppflags)  $(src)dywTorusStack.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)junoSD_PMT_v2.d

$(bin)$(binobj)junoSD_PMT_v2.d :

$(bin)$(binobj)junoSD_PMT_v2.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)junoSD_PMT_v2.o : $(src)junoSD_PMT_v2.cc
	$(cpp_echo) $(src)junoSD_PMT_v2.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoSD_PMT_v2_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoSD_PMT_v2_cppflags) $(junoSD_PMT_v2_cc_cppflags)  $(src)junoSD_PMT_v2.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(junoSD_PMT_v2_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)junoSD_PMT_v2.cc

$(bin)$(binobj)junoSD_PMT_v2.o : $(junoSD_PMT_v2_cc_dependencies)
	$(cpp_echo) $(src)junoSD_PMT_v2.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(junoSD_PMT_v2_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(junoSD_PMT_v2_cppflags) $(junoSD_PMT_v2_cc_cppflags)  $(src)junoSD_PMT_v2.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PMTSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Tub3inchPMTManager.d

$(bin)$(binobj)Tub3inchPMTManager.d :

$(bin)$(binobj)Tub3inchPMTManager.o : $(cmt_final_setup_PMTSim)

$(bin)$(binobj)Tub3inchPMTManager.o : $(src)Tub3inchPMTManager.cc
	$(cpp_echo) $(src)Tub3inchPMTManager.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTManager_cppflags) $(Tub3inchPMTManager_cc_cppflags)  $(src)Tub3inchPMTManager.cc
endif
endif

else
$(bin)PMTSim_dependencies.make : $(Tub3inchPMTManager_cc_dependencies)

$(bin)PMTSim_dependencies.make : $(src)Tub3inchPMTManager.cc

$(bin)$(binobj)Tub3inchPMTManager.o : $(Tub3inchPMTManager_cc_dependencies)
	$(cpp_echo) $(src)Tub3inchPMTManager.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PMTSim_pp_cppflags) $(lib_PMTSim_pp_cppflags) $(Tub3inchPMTManager_pp_cppflags) $(use_cppflags) $(PMTSim_cppflags) $(lib_PMTSim_cppflags) $(Tub3inchPMTManager_cppflags) $(Tub3inchPMTManager_cc_cppflags)  $(src)Tub3inchPMTManager.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: PMTSimclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(PMTSim.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

PMTSimclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library PMTSim
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)PMTSim$(library_suffix).a $(library_prefix)PMTSim$(library_suffix).$(shlibsuffix) PMTSim.stamp PMTSim.shstamp
#-- end of cleanup_library ---------------
