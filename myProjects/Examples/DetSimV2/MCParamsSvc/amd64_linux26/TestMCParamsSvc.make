#-- start of make_header -----------------

#====================================
#  Library TestMCParamsSvc
#
#   Generated Fri Nov 20 00:13:21 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_TestMCParamsSvc_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_TestMCParamsSvc_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_TestMCParamsSvc

MCParamsSvc_tag = $(tag)

#cmt_local_tagfile_TestMCParamsSvc = $(MCParamsSvc_tag)_TestMCParamsSvc.make
cmt_local_tagfile_TestMCParamsSvc = $(bin)$(MCParamsSvc_tag)_TestMCParamsSvc.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MCParamsSvc_tag = $(tag)

#cmt_local_tagfile_TestMCParamsSvc = $(MCParamsSvc_tag).make
cmt_local_tagfile_TestMCParamsSvc = $(bin)$(MCParamsSvc_tag).make

endif

include $(cmt_local_tagfile_TestMCParamsSvc)
#-include $(cmt_local_tagfile_TestMCParamsSvc)

ifdef cmt_TestMCParamsSvc_has_target_tag

cmt_final_setup_TestMCParamsSvc = $(bin)setup_TestMCParamsSvc.make
cmt_dependencies_in_TestMCParamsSvc = $(bin)dependencies_TestMCParamsSvc.in
#cmt_final_setup_TestMCParamsSvc = $(bin)MCParamsSvc_TestMCParamsSvcsetup.make
cmt_local_TestMCParamsSvc_makefile = $(bin)TestMCParamsSvc.make

else

cmt_final_setup_TestMCParamsSvc = $(bin)setup.make
cmt_dependencies_in_TestMCParamsSvc = $(bin)dependencies.in
#cmt_final_setup_TestMCParamsSvc = $(bin)MCParamsSvcsetup.make
cmt_local_TestMCParamsSvc_makefile = $(bin)TestMCParamsSvc.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MCParamsSvcsetup.make

#TestMCParamsSvc :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'TestMCParamsSvc'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = TestMCParamsSvc/
#TestMCParamsSvc::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

TestMCParamsSvclibname   = $(bin)$(library_prefix)TestMCParamsSvc$(library_suffix)
TestMCParamsSvclib       = $(TestMCParamsSvclibname).a
TestMCParamsSvcstamp     = $(bin)TestMCParamsSvc.stamp
TestMCParamsSvcshstamp   = $(bin)TestMCParamsSvc.shstamp

TestMCParamsSvc :: dirs  TestMCParamsSvcLIB
	$(echo) "TestMCParamsSvc ok"

cmt_TestMCParamsSvc_has_prototypes = 1

#--------------------------------------

ifdef cmt_TestMCParamsSvc_has_prototypes

TestMCParamsSvcprototype :  ;

endif

TestMCParamsSvccompile : $(bin)TestAlg.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

TestMCParamsSvcLIB :: $(TestMCParamsSvclib) $(TestMCParamsSvcshstamp)
	$(echo) "TestMCParamsSvc : library ok"

$(TestMCParamsSvclib) :: $(bin)TestAlg.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(TestMCParamsSvclib) $(bin)TestAlg.o
	$(lib_silent) $(ranlib) $(TestMCParamsSvclib)
	$(lib_silent) cat /dev/null >$(TestMCParamsSvcstamp)

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

$(TestMCParamsSvclibname).$(shlibsuffix) :: $(TestMCParamsSvclib) requirements $(use_requirements) $(TestMCParamsSvcstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" TestMCParamsSvc $(TestMCParamsSvc_shlibflags)
	$(lib_silent) cat /dev/null >$(TestMCParamsSvcshstamp)

$(TestMCParamsSvcshstamp) :: $(TestMCParamsSvclibname).$(shlibsuffix)
	$(lib_silent) if test -f $(TestMCParamsSvclibname).$(shlibsuffix) ; then cat /dev/null >$(TestMCParamsSvcshstamp) ; fi

TestMCParamsSvcclean ::
	$(cleanup_echo) objects TestMCParamsSvc
	$(cleanup_silent) /bin/rm -f $(bin)TestAlg.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)TestAlg.o) $(patsubst %.o,%.dep,$(bin)TestAlg.o) $(patsubst %.o,%.d.stamp,$(bin)TestAlg.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf TestMCParamsSvc_deps TestMCParamsSvc_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
TestMCParamsSvcinstallname = $(library_prefix)TestMCParamsSvc$(library_suffix).$(shlibsuffix)

TestMCParamsSvc :: TestMCParamsSvcinstall ;

install :: TestMCParamsSvcinstall ;

TestMCParamsSvcinstall :: $(install_dir)/$(TestMCParamsSvcinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(TestMCParamsSvcinstallname) :: $(bin)$(TestMCParamsSvcinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(TestMCParamsSvcinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##TestMCParamsSvcclean :: TestMCParamsSvcuninstall

uninstall :: TestMCParamsSvcuninstall ;

TestMCParamsSvcuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(TestMCParamsSvcinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),TestMCParamsSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),TestMCParamsSvcprototype)

$(bin)TestMCParamsSvc_dependencies.make : $(use_requirements) $(cmt_final_setup_TestMCParamsSvc)
	$(echo) "(TestMCParamsSvc.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)test/TestAlg.cc -end_all $(includes) $(app_TestMCParamsSvc_cppflags) $(lib_TestMCParamsSvc_cppflags) -name=TestMCParamsSvc $? -f=$(cmt_dependencies_in_TestMCParamsSvc) -without_cmt

-include $(bin)TestMCParamsSvc_dependencies.make

endif
endif
endif

TestMCParamsSvcclean ::
	$(cleanup_silent) \rm -rf $(bin)TestMCParamsSvc_deps $(bin)TestMCParamsSvc_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),TestMCParamsSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)TestAlg.d

$(bin)$(binobj)TestAlg.d :

$(bin)$(binobj)TestAlg.o : $(cmt_final_setup_TestMCParamsSvc)

$(bin)$(binobj)TestAlg.o : $(src)test/TestAlg.cc
	$(cpp_echo) $(src)test/TestAlg.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(TestMCParamsSvc_pp_cppflags) $(lib_TestMCParamsSvc_pp_cppflags) $(TestAlg_pp_cppflags) $(use_cppflags) $(TestMCParamsSvc_cppflags) $(lib_TestMCParamsSvc_cppflags) $(TestAlg_cppflags) $(TestAlg_cc_cppflags) -I../src/test $(src)test/TestAlg.cc
endif
endif

else
$(bin)TestMCParamsSvc_dependencies.make : $(TestAlg_cc_dependencies)

$(bin)TestMCParamsSvc_dependencies.make : $(src)test/TestAlg.cc

$(bin)$(binobj)TestAlg.o : $(TestAlg_cc_dependencies)
	$(cpp_echo) $(src)test/TestAlg.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(TestMCParamsSvc_pp_cppflags) $(lib_TestMCParamsSvc_pp_cppflags) $(TestAlg_pp_cppflags) $(use_cppflags) $(TestMCParamsSvc_cppflags) $(lib_TestMCParamsSvc_cppflags) $(TestAlg_cppflags) $(TestAlg_cc_cppflags) -I../src/test $(src)test/TestAlg.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: TestMCParamsSvcclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(TestMCParamsSvc.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

TestMCParamsSvcclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library TestMCParamsSvc
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)TestMCParamsSvc$(library_suffix).a $(library_prefix)TestMCParamsSvc$(library_suffix).$(shlibsuffix) TestMCParamsSvc.stamp TestMCParamsSvc.shstamp
#-- end of cleanup_library ---------------
