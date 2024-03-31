#-- start of make_header -----------------

#====================================
#  Library Chimney
#
#   Generated Fri Nov 20 00:10:38 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_Chimney_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Chimney_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Chimney

Chimney_tag = $(tag)

#cmt_local_tagfile_Chimney = $(Chimney_tag)_Chimney.make
cmt_local_tagfile_Chimney = $(bin)$(Chimney_tag)_Chimney.make

else

tags      = $(tag),$(CMTEXTRATAGS)

Chimney_tag = $(tag)

#cmt_local_tagfile_Chimney = $(Chimney_tag).make
cmt_local_tagfile_Chimney = $(bin)$(Chimney_tag).make

endif

include $(cmt_local_tagfile_Chimney)
#-include $(cmt_local_tagfile_Chimney)

ifdef cmt_Chimney_has_target_tag

cmt_final_setup_Chimney = $(bin)setup_Chimney.make
cmt_dependencies_in_Chimney = $(bin)dependencies_Chimney.in
#cmt_final_setup_Chimney = $(bin)Chimney_Chimneysetup.make
cmt_local_Chimney_makefile = $(bin)Chimney.make

else

cmt_final_setup_Chimney = $(bin)setup.make
cmt_dependencies_in_Chimney = $(bin)dependencies.in
#cmt_final_setup_Chimney = $(bin)Chimneysetup.make
cmt_local_Chimney_makefile = $(bin)Chimney.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)Chimneysetup.make

#Chimney :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'Chimney'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Chimney/
#Chimney::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

Chimneylibname   = $(bin)$(library_prefix)Chimney$(library_suffix)
Chimneylib       = $(Chimneylibname).a
Chimneystamp     = $(bin)Chimney.stamp
Chimneyshstamp   = $(bin)Chimney.shstamp

Chimney :: dirs  ChimneyLIB
	$(echo) "Chimney ok"

cmt_Chimney_has_prototypes = 1

#--------------------------------------

ifdef cmt_Chimney_has_prototypes

Chimneyprototype :  ;

endif

Chimneycompile : $(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

ChimneyLIB :: $(Chimneylib) $(Chimneyshstamp)
	$(echo) "Chimney : library ok"

$(Chimneylib) :: $(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(Chimneylib) $(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o
	$(lib_silent) $(ranlib) $(Chimneylib)
	$(lib_silent) cat /dev/null >$(Chimneystamp)

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

$(Chimneylibname).$(shlibsuffix) :: $(Chimneylib) requirements $(use_requirements) $(Chimneystamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" Chimney $(Chimney_shlibflags)
	$(lib_silent) cat /dev/null >$(Chimneyshstamp)

$(Chimneyshstamp) :: $(Chimneylibname).$(shlibsuffix)
	$(lib_silent) if test -f $(Chimneylibname).$(shlibsuffix) ; then cat /dev/null >$(Chimneyshstamp) ; fi

Chimneyclean ::
	$(cleanup_echo) objects Chimney
	$(cleanup_silent) /bin/rm -f $(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o) $(patsubst %.o,%.dep,$(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o) $(patsubst %.o,%.d.stamp,$(bin)LowerChimneyPlacement.o $(bin)UpperChimneyPlacement.o $(bin)LowerChimney.o $(bin)LowerChimneyMaker.o $(bin)UpperChimney.o $(bin)UpperChimneyMaker.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf Chimney_deps Chimney_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
Chimneyinstallname = $(library_prefix)Chimney$(library_suffix).$(shlibsuffix)

Chimney :: Chimneyinstall ;

install :: Chimneyinstall ;

Chimneyinstall :: $(install_dir)/$(Chimneyinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(Chimneyinstallname) :: $(bin)$(Chimneyinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Chimneyinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##Chimneyclean :: Chimneyuninstall

uninstall :: Chimneyuninstall ;

Chimneyuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Chimneyinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),Chimneyprototype)

$(bin)Chimney_dependencies.make : $(use_requirements) $(cmt_final_setup_Chimney)
	$(echo) "(Chimney.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)LowerChimneyPlacement.cc $(src)UpperChimneyPlacement.cc $(src)LowerChimney.cc $(src)LowerChimneyMaker.cc $(src)UpperChimney.cc $(src)UpperChimneyMaker.cc -end_all $(includes) $(app_Chimney_cppflags) $(lib_Chimney_cppflags) -name=Chimney $? -f=$(cmt_dependencies_in_Chimney) -without_cmt

-include $(bin)Chimney_dependencies.make

endif
endif
endif

Chimneyclean ::
	$(cleanup_silent) \rm -rf $(bin)Chimney_deps $(bin)Chimney_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)LowerChimneyPlacement.d

$(bin)$(binobj)LowerChimneyPlacement.d :

$(bin)$(binobj)LowerChimneyPlacement.o : $(cmt_final_setup_Chimney)

$(bin)$(binobj)LowerChimneyPlacement.o : $(src)LowerChimneyPlacement.cc
	$(cpp_echo) $(src)LowerChimneyPlacement.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(LowerChimneyPlacement_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(LowerChimneyPlacement_cppflags) $(LowerChimneyPlacement_cc_cppflags)  $(src)LowerChimneyPlacement.cc
endif
endif

else
$(bin)Chimney_dependencies.make : $(LowerChimneyPlacement_cc_dependencies)

$(bin)Chimney_dependencies.make : $(src)LowerChimneyPlacement.cc

$(bin)$(binobj)LowerChimneyPlacement.o : $(LowerChimneyPlacement_cc_dependencies)
	$(cpp_echo) $(src)LowerChimneyPlacement.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(LowerChimneyPlacement_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(LowerChimneyPlacement_cppflags) $(LowerChimneyPlacement_cc_cppflags)  $(src)LowerChimneyPlacement.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)UpperChimneyPlacement.d

$(bin)$(binobj)UpperChimneyPlacement.d :

$(bin)$(binobj)UpperChimneyPlacement.o : $(cmt_final_setup_Chimney)

$(bin)$(binobj)UpperChimneyPlacement.o : $(src)UpperChimneyPlacement.cc
	$(cpp_echo) $(src)UpperChimneyPlacement.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(UpperChimneyPlacement_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(UpperChimneyPlacement_cppflags) $(UpperChimneyPlacement_cc_cppflags)  $(src)UpperChimneyPlacement.cc
endif
endif

else
$(bin)Chimney_dependencies.make : $(UpperChimneyPlacement_cc_dependencies)

$(bin)Chimney_dependencies.make : $(src)UpperChimneyPlacement.cc

$(bin)$(binobj)UpperChimneyPlacement.o : $(UpperChimneyPlacement_cc_dependencies)
	$(cpp_echo) $(src)UpperChimneyPlacement.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(UpperChimneyPlacement_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(UpperChimneyPlacement_cppflags) $(UpperChimneyPlacement_cc_cppflags)  $(src)UpperChimneyPlacement.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)LowerChimney.d

$(bin)$(binobj)LowerChimney.d :

$(bin)$(binobj)LowerChimney.o : $(cmt_final_setup_Chimney)

$(bin)$(binobj)LowerChimney.o : $(src)LowerChimney.cc
	$(cpp_echo) $(src)LowerChimney.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(LowerChimney_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(LowerChimney_cppflags) $(LowerChimney_cc_cppflags)  $(src)LowerChimney.cc
endif
endif

else
$(bin)Chimney_dependencies.make : $(LowerChimney_cc_dependencies)

$(bin)Chimney_dependencies.make : $(src)LowerChimney.cc

$(bin)$(binobj)LowerChimney.o : $(LowerChimney_cc_dependencies)
	$(cpp_echo) $(src)LowerChimney.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(LowerChimney_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(LowerChimney_cppflags) $(LowerChimney_cc_cppflags)  $(src)LowerChimney.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)LowerChimneyMaker.d

$(bin)$(binobj)LowerChimneyMaker.d :

$(bin)$(binobj)LowerChimneyMaker.o : $(cmt_final_setup_Chimney)

$(bin)$(binobj)LowerChimneyMaker.o : $(src)LowerChimneyMaker.cc
	$(cpp_echo) $(src)LowerChimneyMaker.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(LowerChimneyMaker_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(LowerChimneyMaker_cppflags) $(LowerChimneyMaker_cc_cppflags)  $(src)LowerChimneyMaker.cc
endif
endif

else
$(bin)Chimney_dependencies.make : $(LowerChimneyMaker_cc_dependencies)

$(bin)Chimney_dependencies.make : $(src)LowerChimneyMaker.cc

$(bin)$(binobj)LowerChimneyMaker.o : $(LowerChimneyMaker_cc_dependencies)
	$(cpp_echo) $(src)LowerChimneyMaker.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(LowerChimneyMaker_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(LowerChimneyMaker_cppflags) $(LowerChimneyMaker_cc_cppflags)  $(src)LowerChimneyMaker.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)UpperChimney.d

$(bin)$(binobj)UpperChimney.d :

$(bin)$(binobj)UpperChimney.o : $(cmt_final_setup_Chimney)

$(bin)$(binobj)UpperChimney.o : $(src)UpperChimney.cc
	$(cpp_echo) $(src)UpperChimney.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(UpperChimney_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(UpperChimney_cppflags) $(UpperChimney_cc_cppflags)  $(src)UpperChimney.cc
endif
endif

else
$(bin)Chimney_dependencies.make : $(UpperChimney_cc_dependencies)

$(bin)Chimney_dependencies.make : $(src)UpperChimney.cc

$(bin)$(binobj)UpperChimney.o : $(UpperChimney_cc_dependencies)
	$(cpp_echo) $(src)UpperChimney.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(UpperChimney_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(UpperChimney_cppflags) $(UpperChimney_cc_cppflags)  $(src)UpperChimney.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),Chimneyclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)UpperChimneyMaker.d

$(bin)$(binobj)UpperChimneyMaker.d :

$(bin)$(binobj)UpperChimneyMaker.o : $(cmt_final_setup_Chimney)

$(bin)$(binobj)UpperChimneyMaker.o : $(src)UpperChimneyMaker.cc
	$(cpp_echo) $(src)UpperChimneyMaker.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(UpperChimneyMaker_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(UpperChimneyMaker_cppflags) $(UpperChimneyMaker_cc_cppflags)  $(src)UpperChimneyMaker.cc
endif
endif

else
$(bin)Chimney_dependencies.make : $(UpperChimneyMaker_cc_dependencies)

$(bin)Chimney_dependencies.make : $(src)UpperChimneyMaker.cc

$(bin)$(binobj)UpperChimneyMaker.o : $(UpperChimneyMaker_cc_dependencies)
	$(cpp_echo) $(src)UpperChimneyMaker.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Chimney_pp_cppflags) $(lib_Chimney_pp_cppflags) $(UpperChimneyMaker_pp_cppflags) $(use_cppflags) $(Chimney_cppflags) $(lib_Chimney_cppflags) $(UpperChimneyMaker_cppflags) $(UpperChimneyMaker_cc_cppflags)  $(src)UpperChimneyMaker.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: Chimneyclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(Chimney.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

Chimneyclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library Chimney
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)Chimney$(library_suffix).a $(library_prefix)Chimney$(library_suffix).$(shlibsuffix) Chimney.stamp Chimney.shstamp
#-- end of cleanup_library ---------------
