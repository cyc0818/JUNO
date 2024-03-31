#-- start of make_header -----------------

#====================================
#  Library PhysiSim
#
#   Generated Fri Nov 20 00:10:04 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_PhysiSim_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_PhysiSim_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_PhysiSim

PhysiSim_tag = $(tag)

#cmt_local_tagfile_PhysiSim = $(PhysiSim_tag)_PhysiSim.make
cmt_local_tagfile_PhysiSim = $(bin)$(PhysiSim_tag)_PhysiSim.make

else

tags      = $(tag),$(CMTEXTRATAGS)

PhysiSim_tag = $(tag)

#cmt_local_tagfile_PhysiSim = $(PhysiSim_tag).make
cmt_local_tagfile_PhysiSim = $(bin)$(PhysiSim_tag).make

endif

include $(cmt_local_tagfile_PhysiSim)
#-include $(cmt_local_tagfile_PhysiSim)

ifdef cmt_PhysiSim_has_target_tag

cmt_final_setup_PhysiSim = $(bin)setup_PhysiSim.make
cmt_dependencies_in_PhysiSim = $(bin)dependencies_PhysiSim.in
#cmt_final_setup_PhysiSim = $(bin)PhysiSim_PhysiSimsetup.make
cmt_local_PhysiSim_makefile = $(bin)PhysiSim.make

else

cmt_final_setup_PhysiSim = $(bin)setup.make
cmt_dependencies_in_PhysiSim = $(bin)dependencies.in
#cmt_final_setup_PhysiSim = $(bin)PhysiSimsetup.make
cmt_local_PhysiSim_makefile = $(bin)PhysiSim.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)PhysiSimsetup.make

#PhysiSim :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'PhysiSim'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = PhysiSim/
#PhysiSim::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

PhysiSimlibname   = $(bin)$(library_prefix)PhysiSim$(library_suffix)
PhysiSimlib       = $(PhysiSimlibname).a
PhysiSimstamp     = $(bin)PhysiSim.stamp
PhysiSimshstamp   = $(bin)PhysiSim.shstamp

PhysiSim :: dirs  PhysiSimLIB
	$(echo) "PhysiSim ok"

cmt_PhysiSim_has_prototypes = 1

#--------------------------------------

ifdef cmt_PhysiSim_has_prototypes

PhysiSimprototype :  ;

endif

PhysiSimcompile : $(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

PhysiSimLIB :: $(PhysiSimlib) $(PhysiSimshstamp)
	$(echo) "PhysiSim : library ok"

$(PhysiSimlib) :: $(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(PhysiSimlib) $(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o
	$(lib_silent) $(ranlib) $(PhysiSimlib)
	$(lib_silent) cat /dev/null >$(PhysiSimstamp)

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

$(PhysiSimlibname).$(shlibsuffix) :: $(PhysiSimlib) requirements $(use_requirements) $(PhysiSimstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" PhysiSim $(PhysiSim_shlibflags)
	$(lib_silent) cat /dev/null >$(PhysiSimshstamp)

$(PhysiSimshstamp) :: $(PhysiSimlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(PhysiSimlibname).$(shlibsuffix) ; then cat /dev/null >$(PhysiSimshstamp) ; fi

PhysiSimclean ::
	$(cleanup_echo) objects PhysiSim
	$(cleanup_silent) /bin/rm -f $(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o) $(patsubst %.o,%.dep,$(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o) $(patsubst %.o,%.d.stamp,$(bin)DsG4Scintillation.o $(bin)LocalG4Cerenkov1042.o $(bin)DsPhysConsOptical.o $(bin)DsG4HadronPhysicsQGSP_BERT_HP.o $(bin)DsG4NeutronPHPBuilder.o $(bin)DsG410NNDCParticleHPCaptureFS.o $(bin)DsG410GdParticleHPCaptureFS.o $(bin)DsG4ParticleHPCapture.o $(bin)DsG4GdCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)G4Positronium.o $(bin)G4PositroniumFormation.o $(bin)G4PositroniumDecayChannel2G.o $(bin)G4PositroniumDecayChannel3G.o $(bin)DsG4NNDCCaptureGammas.o $(bin)DsG4EmLivermorePhysics.o $(bin)DsG4ScintSimple.o $(bin)DsG4OpAbsReemit.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf PhysiSim_deps PhysiSim_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
PhysiSiminstallname = $(library_prefix)PhysiSim$(library_suffix).$(shlibsuffix)

PhysiSim :: PhysiSiminstall ;

install :: PhysiSiminstall ;

PhysiSiminstall :: $(install_dir)/$(PhysiSiminstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(PhysiSiminstallname) :: $(bin)$(PhysiSiminstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(PhysiSiminstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##PhysiSimclean :: PhysiSimuninstall

uninstall :: PhysiSimuninstall ;

PhysiSimuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(PhysiSiminstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),PhysiSimprototype)

$(bin)PhysiSim_dependencies.make : $(use_requirements) $(cmt_final_setup_PhysiSim)
	$(echo) "(PhysiSim.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)DsG4Scintillation.cc $(src)LocalG4Cerenkov1042.cc $(src)DsPhysConsOptical.cc $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc $(src)DsG4NeutronPHPBuilder.cc $(src)DsG410NNDCParticleHPCaptureFS.cc $(src)DsG410GdParticleHPCaptureFS.cc $(src)DsG4ParticleHPCapture.cc $(src)DsG4GdCaptureGammas.cc $(src)DsG4EmLivermorePhysics.cc $(src)G4Positronium.cc $(src)G4PositroniumFormation.cc $(src)G4PositroniumDecayChannel2G.cc $(src)G4PositroniumDecayChannel3G.cc $(src)DsG4NNDCCaptureGammas.cc $(src)DsG4EmLivermorePhysics.cc $(src)DsG4ScintSimple.cc $(src)DsG4OpAbsReemit.cc -end_all $(includes) $(app_PhysiSim_cppflags) $(lib_PhysiSim_cppflags) -name=PhysiSim $? -f=$(cmt_dependencies_in_PhysiSim) -without_cmt

-include $(bin)PhysiSim_dependencies.make

endif
endif
endif

PhysiSimclean ::
	$(cleanup_silent) \rm -rf $(bin)PhysiSim_deps $(bin)PhysiSim_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4Scintillation.d

$(bin)$(binobj)DsG4Scintillation.d :

$(bin)$(binobj)DsG4Scintillation.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4Scintillation.o : $(src)DsG4Scintillation.cc
	$(cpp_echo) $(src)DsG4Scintillation.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4Scintillation_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4Scintillation_cppflags) $(DsG4Scintillation_cc_cppflags)  $(src)DsG4Scintillation.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4Scintillation_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4Scintillation.cc

$(bin)$(binobj)DsG4Scintillation.o : $(DsG4Scintillation_cc_dependencies)
	$(cpp_echo) $(src)DsG4Scintillation.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4Scintillation_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4Scintillation_cppflags) $(DsG4Scintillation_cc_cppflags)  $(src)DsG4Scintillation.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)LocalG4Cerenkov1042.d

$(bin)$(binobj)LocalG4Cerenkov1042.d :

$(bin)$(binobj)LocalG4Cerenkov1042.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)LocalG4Cerenkov1042.o : $(src)LocalG4Cerenkov1042.cc
	$(cpp_echo) $(src)LocalG4Cerenkov1042.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(LocalG4Cerenkov1042_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(LocalG4Cerenkov1042_cppflags) $(LocalG4Cerenkov1042_cc_cppflags)  $(src)LocalG4Cerenkov1042.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(LocalG4Cerenkov1042_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)LocalG4Cerenkov1042.cc

$(bin)$(binobj)LocalG4Cerenkov1042.o : $(LocalG4Cerenkov1042_cc_dependencies)
	$(cpp_echo) $(src)LocalG4Cerenkov1042.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(LocalG4Cerenkov1042_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(LocalG4Cerenkov1042_cppflags) $(LocalG4Cerenkov1042_cc_cppflags)  $(src)LocalG4Cerenkov1042.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsPhysConsOptical.d

$(bin)$(binobj)DsPhysConsOptical.d :

$(bin)$(binobj)DsPhysConsOptical.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsPhysConsOptical.o : $(src)DsPhysConsOptical.cc
	$(cpp_echo) $(src)DsPhysConsOptical.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsPhysConsOptical_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsPhysConsOptical_cppflags) $(DsPhysConsOptical_cc_cppflags)  $(src)DsPhysConsOptical.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsPhysConsOptical_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsPhysConsOptical.cc

$(bin)$(binobj)DsPhysConsOptical.o : $(DsPhysConsOptical_cc_dependencies)
	$(cpp_echo) $(src)DsPhysConsOptical.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsPhysConsOptical_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsPhysConsOptical_cppflags) $(DsPhysConsOptical_cc_cppflags)  $(src)DsPhysConsOptical.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4HadronPhysicsQGSP_BERT_HP.d

$(bin)$(binobj)DsG4HadronPhysicsQGSP_BERT_HP.d :

$(bin)$(binobj)DsG4HadronPhysicsQGSP_BERT_HP.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4HadronPhysicsQGSP_BERT_HP.o : $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc
	$(cpp_echo) $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4HadronPhysicsQGSP_BERT_HP_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4HadronPhysicsQGSP_BERT_HP_cppflags) $(DsG4HadronPhysicsQGSP_BERT_HP_cc_cppflags)  $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4HadronPhysicsQGSP_BERT_HP_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc

$(bin)$(binobj)DsG4HadronPhysicsQGSP_BERT_HP.o : $(DsG4HadronPhysicsQGSP_BERT_HP_cc_dependencies)
	$(cpp_echo) $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4HadronPhysicsQGSP_BERT_HP_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4HadronPhysicsQGSP_BERT_HP_cppflags) $(DsG4HadronPhysicsQGSP_BERT_HP_cc_cppflags)  $(src)DsG4HadronPhysicsQGSP_BERT_HP.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4NeutronPHPBuilder.d

$(bin)$(binobj)DsG4NeutronPHPBuilder.d :

$(bin)$(binobj)DsG4NeutronPHPBuilder.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4NeutronPHPBuilder.o : $(src)DsG4NeutronPHPBuilder.cc
	$(cpp_echo) $(src)DsG4NeutronPHPBuilder.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4NeutronPHPBuilder_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4NeutronPHPBuilder_cppflags) $(DsG4NeutronPHPBuilder_cc_cppflags)  $(src)DsG4NeutronPHPBuilder.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4NeutronPHPBuilder_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4NeutronPHPBuilder.cc

$(bin)$(binobj)DsG4NeutronPHPBuilder.o : $(DsG4NeutronPHPBuilder_cc_dependencies)
	$(cpp_echo) $(src)DsG4NeutronPHPBuilder.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4NeutronPHPBuilder_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4NeutronPHPBuilder_cppflags) $(DsG4NeutronPHPBuilder_cc_cppflags)  $(src)DsG4NeutronPHPBuilder.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG410NNDCParticleHPCaptureFS.d

$(bin)$(binobj)DsG410NNDCParticleHPCaptureFS.d :

$(bin)$(binobj)DsG410NNDCParticleHPCaptureFS.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG410NNDCParticleHPCaptureFS.o : $(src)DsG410NNDCParticleHPCaptureFS.cc
	$(cpp_echo) $(src)DsG410NNDCParticleHPCaptureFS.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG410NNDCParticleHPCaptureFS_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG410NNDCParticleHPCaptureFS_cppflags) $(DsG410NNDCParticleHPCaptureFS_cc_cppflags)  $(src)DsG410NNDCParticleHPCaptureFS.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG410NNDCParticleHPCaptureFS_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG410NNDCParticleHPCaptureFS.cc

$(bin)$(binobj)DsG410NNDCParticleHPCaptureFS.o : $(DsG410NNDCParticleHPCaptureFS_cc_dependencies)
	$(cpp_echo) $(src)DsG410NNDCParticleHPCaptureFS.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG410NNDCParticleHPCaptureFS_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG410NNDCParticleHPCaptureFS_cppflags) $(DsG410NNDCParticleHPCaptureFS_cc_cppflags)  $(src)DsG410NNDCParticleHPCaptureFS.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG410GdParticleHPCaptureFS.d

$(bin)$(binobj)DsG410GdParticleHPCaptureFS.d :

$(bin)$(binobj)DsG410GdParticleHPCaptureFS.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG410GdParticleHPCaptureFS.o : $(src)DsG410GdParticleHPCaptureFS.cc
	$(cpp_echo) $(src)DsG410GdParticleHPCaptureFS.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG410GdParticleHPCaptureFS_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG410GdParticleHPCaptureFS_cppflags) $(DsG410GdParticleHPCaptureFS_cc_cppflags)  $(src)DsG410GdParticleHPCaptureFS.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG410GdParticleHPCaptureFS_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG410GdParticleHPCaptureFS.cc

$(bin)$(binobj)DsG410GdParticleHPCaptureFS.o : $(DsG410GdParticleHPCaptureFS_cc_dependencies)
	$(cpp_echo) $(src)DsG410GdParticleHPCaptureFS.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG410GdParticleHPCaptureFS_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG410GdParticleHPCaptureFS_cppflags) $(DsG410GdParticleHPCaptureFS_cc_cppflags)  $(src)DsG410GdParticleHPCaptureFS.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4ParticleHPCapture.d

$(bin)$(binobj)DsG4ParticleHPCapture.d :

$(bin)$(binobj)DsG4ParticleHPCapture.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4ParticleHPCapture.o : $(src)DsG4ParticleHPCapture.cc
	$(cpp_echo) $(src)DsG4ParticleHPCapture.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4ParticleHPCapture_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4ParticleHPCapture_cppflags) $(DsG4ParticleHPCapture_cc_cppflags)  $(src)DsG4ParticleHPCapture.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4ParticleHPCapture_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4ParticleHPCapture.cc

$(bin)$(binobj)DsG4ParticleHPCapture.o : $(DsG4ParticleHPCapture_cc_dependencies)
	$(cpp_echo) $(src)DsG4ParticleHPCapture.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4ParticleHPCapture_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4ParticleHPCapture_cppflags) $(DsG4ParticleHPCapture_cc_cppflags)  $(src)DsG4ParticleHPCapture.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4GdCaptureGammas.d

$(bin)$(binobj)DsG4GdCaptureGammas.d :

$(bin)$(binobj)DsG4GdCaptureGammas.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4GdCaptureGammas.o : $(src)DsG4GdCaptureGammas.cc
	$(cpp_echo) $(src)DsG4GdCaptureGammas.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4GdCaptureGammas_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4GdCaptureGammas_cppflags) $(DsG4GdCaptureGammas_cc_cppflags)  $(src)DsG4GdCaptureGammas.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4GdCaptureGammas_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4GdCaptureGammas.cc

$(bin)$(binobj)DsG4GdCaptureGammas.o : $(DsG4GdCaptureGammas_cc_dependencies)
	$(cpp_echo) $(src)DsG4GdCaptureGammas.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4GdCaptureGammas_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4GdCaptureGammas_cppflags) $(DsG4GdCaptureGammas_cc_cppflags)  $(src)DsG4GdCaptureGammas.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4EmLivermorePhysics.d

$(bin)$(binobj)DsG4EmLivermorePhysics.d :

$(bin)$(binobj)DsG4EmLivermorePhysics.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4EmLivermorePhysics.o : $(src)DsG4EmLivermorePhysics.cc
	$(cpp_echo) $(src)DsG4EmLivermorePhysics.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4EmLivermorePhysics_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4EmLivermorePhysics_cppflags) $(DsG4EmLivermorePhysics_cc_cppflags)  $(src)DsG4EmLivermorePhysics.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4EmLivermorePhysics_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4EmLivermorePhysics.cc

$(bin)$(binobj)DsG4EmLivermorePhysics.o : $(DsG4EmLivermorePhysics_cc_dependencies)
	$(cpp_echo) $(src)DsG4EmLivermorePhysics.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4EmLivermorePhysics_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4EmLivermorePhysics_cppflags) $(DsG4EmLivermorePhysics_cc_cppflags)  $(src)DsG4EmLivermorePhysics.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4Positronium.d

$(bin)$(binobj)G4Positronium.d :

$(bin)$(binobj)G4Positronium.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)G4Positronium.o : $(src)G4Positronium.cc
	$(cpp_echo) $(src)G4Positronium.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4Positronium_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4Positronium_cppflags) $(G4Positronium_cc_cppflags)  $(src)G4Positronium.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(G4Positronium_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)G4Positronium.cc

$(bin)$(binobj)G4Positronium.o : $(G4Positronium_cc_dependencies)
	$(cpp_echo) $(src)G4Positronium.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4Positronium_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4Positronium_cppflags) $(G4Positronium_cc_cppflags)  $(src)G4Positronium.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4PositroniumFormation.d

$(bin)$(binobj)G4PositroniumFormation.d :

$(bin)$(binobj)G4PositroniumFormation.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)G4PositroniumFormation.o : $(src)G4PositroniumFormation.cc
	$(cpp_echo) $(src)G4PositroniumFormation.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4PositroniumFormation_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4PositroniumFormation_cppflags) $(G4PositroniumFormation_cc_cppflags)  $(src)G4PositroniumFormation.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(G4PositroniumFormation_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)G4PositroniumFormation.cc

$(bin)$(binobj)G4PositroniumFormation.o : $(G4PositroniumFormation_cc_dependencies)
	$(cpp_echo) $(src)G4PositroniumFormation.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4PositroniumFormation_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4PositroniumFormation_cppflags) $(G4PositroniumFormation_cc_cppflags)  $(src)G4PositroniumFormation.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4PositroniumDecayChannel2G.d

$(bin)$(binobj)G4PositroniumDecayChannel2G.d :

$(bin)$(binobj)G4PositroniumDecayChannel2G.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)G4PositroniumDecayChannel2G.o : $(src)G4PositroniumDecayChannel2G.cc
	$(cpp_echo) $(src)G4PositroniumDecayChannel2G.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4PositroniumDecayChannel2G_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4PositroniumDecayChannel2G_cppflags) $(G4PositroniumDecayChannel2G_cc_cppflags)  $(src)G4PositroniumDecayChannel2G.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(G4PositroniumDecayChannel2G_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)G4PositroniumDecayChannel2G.cc

$(bin)$(binobj)G4PositroniumDecayChannel2G.o : $(G4PositroniumDecayChannel2G_cc_dependencies)
	$(cpp_echo) $(src)G4PositroniumDecayChannel2G.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4PositroniumDecayChannel2G_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4PositroniumDecayChannel2G_cppflags) $(G4PositroniumDecayChannel2G_cc_cppflags)  $(src)G4PositroniumDecayChannel2G.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)G4PositroniumDecayChannel3G.d

$(bin)$(binobj)G4PositroniumDecayChannel3G.d :

$(bin)$(binobj)G4PositroniumDecayChannel3G.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)G4PositroniumDecayChannel3G.o : $(src)G4PositroniumDecayChannel3G.cc
	$(cpp_echo) $(src)G4PositroniumDecayChannel3G.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4PositroniumDecayChannel3G_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4PositroniumDecayChannel3G_cppflags) $(G4PositroniumDecayChannel3G_cc_cppflags)  $(src)G4PositroniumDecayChannel3G.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(G4PositroniumDecayChannel3G_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)G4PositroniumDecayChannel3G.cc

$(bin)$(binobj)G4PositroniumDecayChannel3G.o : $(G4PositroniumDecayChannel3G_cc_dependencies)
	$(cpp_echo) $(src)G4PositroniumDecayChannel3G.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(G4PositroniumDecayChannel3G_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(G4PositroniumDecayChannel3G_cppflags) $(G4PositroniumDecayChannel3G_cc_cppflags)  $(src)G4PositroniumDecayChannel3G.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4NNDCCaptureGammas.d

$(bin)$(binobj)DsG4NNDCCaptureGammas.d :

$(bin)$(binobj)DsG4NNDCCaptureGammas.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4NNDCCaptureGammas.o : $(src)DsG4NNDCCaptureGammas.cc
	$(cpp_echo) $(src)DsG4NNDCCaptureGammas.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4NNDCCaptureGammas_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4NNDCCaptureGammas_cppflags) $(DsG4NNDCCaptureGammas_cc_cppflags)  $(src)DsG4NNDCCaptureGammas.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4NNDCCaptureGammas_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4NNDCCaptureGammas.cc

$(bin)$(binobj)DsG4NNDCCaptureGammas.o : $(DsG4NNDCCaptureGammas_cc_dependencies)
	$(cpp_echo) $(src)DsG4NNDCCaptureGammas.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4NNDCCaptureGammas_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4NNDCCaptureGammas_cppflags) $(DsG4NNDCCaptureGammas_cc_cppflags)  $(src)DsG4NNDCCaptureGammas.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4EmLivermorePhysics.d

$(bin)$(binobj)DsG4EmLivermorePhysics.d :

$(bin)$(binobj)DsG4EmLivermorePhysics.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4EmLivermorePhysics.o : $(src)DsG4EmLivermorePhysics.cc
	$(cpp_echo) $(src)DsG4EmLivermorePhysics.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4EmLivermorePhysics_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4EmLivermorePhysics_cppflags) $(DsG4EmLivermorePhysics_cc_cppflags)  $(src)DsG4EmLivermorePhysics.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4EmLivermorePhysics_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4EmLivermorePhysics.cc

$(bin)$(binobj)DsG4EmLivermorePhysics.o : $(DsG4EmLivermorePhysics_cc_dependencies)
	$(cpp_echo) $(src)DsG4EmLivermorePhysics.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4EmLivermorePhysics_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4EmLivermorePhysics_cppflags) $(DsG4EmLivermorePhysics_cc_cppflags)  $(src)DsG4EmLivermorePhysics.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4ScintSimple.d

$(bin)$(binobj)DsG4ScintSimple.d :

$(bin)$(binobj)DsG4ScintSimple.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4ScintSimple.o : $(src)DsG4ScintSimple.cc
	$(cpp_echo) $(src)DsG4ScintSimple.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4ScintSimple_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4ScintSimple_cppflags) $(DsG4ScintSimple_cc_cppflags)  $(src)DsG4ScintSimple.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4ScintSimple_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4ScintSimple.cc

$(bin)$(binobj)DsG4ScintSimple.o : $(DsG4ScintSimple_cc_dependencies)
	$(cpp_echo) $(src)DsG4ScintSimple.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4ScintSimple_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4ScintSimple_cppflags) $(DsG4ScintSimple_cc_cppflags)  $(src)DsG4ScintSimple.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),PhysiSimclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DsG4OpAbsReemit.d

$(bin)$(binobj)DsG4OpAbsReemit.d :

$(bin)$(binobj)DsG4OpAbsReemit.o : $(cmt_final_setup_PhysiSim)

$(bin)$(binobj)DsG4OpAbsReemit.o : $(src)DsG4OpAbsReemit.cc
	$(cpp_echo) $(src)DsG4OpAbsReemit.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4OpAbsReemit_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4OpAbsReemit_cppflags) $(DsG4OpAbsReemit_cc_cppflags)  $(src)DsG4OpAbsReemit.cc
endif
endif

else
$(bin)PhysiSim_dependencies.make : $(DsG4OpAbsReemit_cc_dependencies)

$(bin)PhysiSim_dependencies.make : $(src)DsG4OpAbsReemit.cc

$(bin)$(binobj)DsG4OpAbsReemit.o : $(DsG4OpAbsReemit_cc_dependencies)
	$(cpp_echo) $(src)DsG4OpAbsReemit.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(PhysiSim_pp_cppflags) $(lib_PhysiSim_pp_cppflags) $(DsG4OpAbsReemit_pp_cppflags) $(use_cppflags) $(PhysiSim_cppflags) $(lib_PhysiSim_cppflags) $(DsG4OpAbsReemit_cppflags) $(DsG4OpAbsReemit_cc_cppflags)  $(src)DsG4OpAbsReemit.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: PhysiSimclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(PhysiSim.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

PhysiSimclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library PhysiSim
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)PhysiSim$(library_suffix).a $(library_prefix)PhysiSim$(library_suffix).$(shlibsuffix) PhysiSim.stamp PhysiSim.shstamp
#-- end of cleanup_library ---------------
