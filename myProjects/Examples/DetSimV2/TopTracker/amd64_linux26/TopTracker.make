#-- start of make_header -----------------

#====================================
#  Library TopTracker
#
#   Generated Fri Nov 20 00:12:03 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_TopTracker_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_TopTracker_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_TopTracker

TopTracker_tag = $(tag)

#cmt_local_tagfile_TopTracker = $(TopTracker_tag)_TopTracker.make
cmt_local_tagfile_TopTracker = $(bin)$(TopTracker_tag)_TopTracker.make

else

tags      = $(tag),$(CMTEXTRATAGS)

TopTracker_tag = $(tag)

#cmt_local_tagfile_TopTracker = $(TopTracker_tag).make
cmt_local_tagfile_TopTracker = $(bin)$(TopTracker_tag).make

endif

include $(cmt_local_tagfile_TopTracker)
#-include $(cmt_local_tagfile_TopTracker)

ifdef cmt_TopTracker_has_target_tag

cmt_final_setup_TopTracker = $(bin)setup_TopTracker.make
cmt_dependencies_in_TopTracker = $(bin)dependencies_TopTracker.in
#cmt_final_setup_TopTracker = $(bin)TopTracker_TopTrackersetup.make
cmt_local_TopTracker_makefile = $(bin)TopTracker.make

else

cmt_final_setup_TopTracker = $(bin)setup.make
cmt_dependencies_in_TopTracker = $(bin)dependencies.in
#cmt_final_setup_TopTracker = $(bin)TopTrackersetup.make
cmt_local_TopTracker_makefile = $(bin)TopTracker.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)TopTrackersetup.make

#TopTracker :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'TopTracker'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = TopTracker/
#TopTracker::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

TopTrackerlibname   = $(bin)$(library_prefix)TopTracker$(library_suffix)
TopTrackerlib       = $(TopTrackerlibname).a
TopTrackerstamp     = $(bin)TopTracker.stamp
TopTrackershstamp   = $(bin)TopTracker.shstamp

TopTracker :: dirs  TopTrackerLIB
	$(echo) "TopTracker ok"

cmt_TopTracker_has_prototypes = 1

#--------------------------------------

ifdef cmt_TopTracker_has_prototypes

TopTrackerprototype :  ;

endif

TopTrackercompile : $(bin)TopTrackerConstruction.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

TopTrackerLIB :: $(TopTrackerlib) $(TopTrackershstamp)
	$(echo) "TopTracker : library ok"

$(TopTrackerlib) :: $(bin)TopTrackerConstruction.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(TopTrackerlib) $(bin)TopTrackerConstruction.o
	$(lib_silent) $(ranlib) $(TopTrackerlib)
	$(lib_silent) cat /dev/null >$(TopTrackerstamp)

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

$(TopTrackerlibname).$(shlibsuffix) :: $(TopTrackerlib) requirements $(use_requirements) $(TopTrackerstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" TopTracker $(TopTracker_shlibflags)
	$(lib_silent) cat /dev/null >$(TopTrackershstamp)

$(TopTrackershstamp) :: $(TopTrackerlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(TopTrackerlibname).$(shlibsuffix) ; then cat /dev/null >$(TopTrackershstamp) ; fi

TopTrackerclean ::
	$(cleanup_echo) objects TopTracker
	$(cleanup_silent) /bin/rm -f $(bin)TopTrackerConstruction.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)TopTrackerConstruction.o) $(patsubst %.o,%.dep,$(bin)TopTrackerConstruction.o) $(patsubst %.o,%.d.stamp,$(bin)TopTrackerConstruction.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf TopTracker_deps TopTracker_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
TopTrackerinstallname = $(library_prefix)TopTracker$(library_suffix).$(shlibsuffix)

TopTracker :: TopTrackerinstall ;

install :: TopTrackerinstall ;

TopTrackerinstall :: $(install_dir)/$(TopTrackerinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(TopTrackerinstallname) :: $(bin)$(TopTrackerinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(TopTrackerinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##TopTrackerclean :: TopTrackeruninstall

uninstall :: TopTrackeruninstall ;

TopTrackeruninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(TopTrackerinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),TopTrackerclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),TopTrackerprototype)

$(bin)TopTracker_dependencies.make : $(use_requirements) $(cmt_final_setup_TopTracker)
	$(echo) "(TopTracker.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)TopTrackerConstruction.cc -end_all $(includes) $(app_TopTracker_cppflags) $(lib_TopTracker_cppflags) -name=TopTracker $? -f=$(cmt_dependencies_in_TopTracker) -without_cmt

-include $(bin)TopTracker_dependencies.make

endif
endif
endif

TopTrackerclean ::
	$(cleanup_silent) \rm -rf $(bin)TopTracker_deps $(bin)TopTracker_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),TopTrackerclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)TopTrackerConstruction.d

$(bin)$(binobj)TopTrackerConstruction.d :

$(bin)$(binobj)TopTrackerConstruction.o : $(cmt_final_setup_TopTracker)

$(bin)$(binobj)TopTrackerConstruction.o : $(src)TopTrackerConstruction.cc
	$(cpp_echo) $(src)TopTrackerConstruction.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(TopTracker_pp_cppflags) $(lib_TopTracker_pp_cppflags) $(TopTrackerConstruction_pp_cppflags) $(use_cppflags) $(TopTracker_cppflags) $(lib_TopTracker_cppflags) $(TopTrackerConstruction_cppflags) $(TopTrackerConstruction_cc_cppflags)  $(src)TopTrackerConstruction.cc
endif
endif

else
$(bin)TopTracker_dependencies.make : $(TopTrackerConstruction_cc_dependencies)

$(bin)TopTracker_dependencies.make : $(src)TopTrackerConstruction.cc

$(bin)$(binobj)TopTrackerConstruction.o : $(TopTrackerConstruction_cc_dependencies)
	$(cpp_echo) $(src)TopTrackerConstruction.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(TopTracker_pp_cppflags) $(lib_TopTracker_pp_cppflags) $(TopTrackerConstruction_pp_cppflags) $(use_cppflags) $(TopTracker_cppflags) $(lib_TopTracker_cppflags) $(TopTrackerConstruction_cppflags) $(TopTrackerConstruction_cc_cppflags)  $(src)TopTrackerConstruction.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: TopTrackerclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(TopTracker.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

TopTrackerclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library TopTracker
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)TopTracker$(library_suffix).a $(library_prefix)TopTracker$(library_suffix).$(shlibsuffix) TopTracker.stamp TopTracker.shstamp
#-- end of cleanup_library ---------------
