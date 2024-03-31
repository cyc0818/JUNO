#-- start of make_header -----------------

#====================================
#  Library DummyAlg
#
#   Generated Tue Jan  2 19:02:44 2018  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DummyAlg_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DummyAlg_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DummyAlg

DummyAlg_tag = $(tag)

#cmt_local_tagfile_DummyAlg = $(DummyAlg_tag)_DummyAlg.make
cmt_local_tagfile_DummyAlg = $(bin)$(DummyAlg_tag)_DummyAlg.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DummyAlg_tag = $(tag)

#cmt_local_tagfile_DummyAlg = $(DummyAlg_tag).make
cmt_local_tagfile_DummyAlg = $(bin)$(DummyAlg_tag).make

endif

include $(cmt_local_tagfile_DummyAlg)
#-include $(cmt_local_tagfile_DummyAlg)

ifdef cmt_DummyAlg_has_target_tag

cmt_final_setup_DummyAlg = $(bin)setup_DummyAlg.make
cmt_dependencies_in_DummyAlg = $(bin)dependencies_DummyAlg.in
#cmt_final_setup_DummyAlg = $(bin)DummyAlg_DummyAlgsetup.make
cmt_local_DummyAlg_makefile = $(bin)DummyAlg.make

else

cmt_final_setup_DummyAlg = $(bin)setup.make
cmt_dependencies_in_DummyAlg = $(bin)dependencies.in
#cmt_final_setup_DummyAlg = $(bin)DummyAlgsetup.make
cmt_local_DummyAlg_makefile = $(bin)DummyAlg.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DummyAlgsetup.make

#DummyAlg :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DummyAlg'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DummyAlg/
#DummyAlg::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

DummyAlglibname   = $(bin)$(library_prefix)DummyAlg$(library_suffix)
DummyAlglib       = $(DummyAlglibname).a
DummyAlgstamp     = $(bin)DummyAlg.stamp
DummyAlgshstamp   = $(bin)DummyAlg.shstamp

DummyAlg :: dirs  DummyAlgLIB
	$(echo) "DummyAlg ok"

cmt_DummyAlg_has_prototypes = 1

#--------------------------------------

ifdef cmt_DummyAlg_has_prototypes

DummyAlgprototype :  ;

endif

DummyAlgcompile : $(bin)DummyAlg.o $(bin)DummyTool.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

DummyAlgLIB :: $(DummyAlglib) $(DummyAlgshstamp)
	$(echo) "DummyAlg : library ok"

$(DummyAlglib) :: $(bin)DummyAlg.o $(bin)DummyTool.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(DummyAlglib) $(bin)DummyAlg.o $(bin)DummyTool.o
	$(lib_silent) $(ranlib) $(DummyAlglib)
	$(lib_silent) cat /dev/null >$(DummyAlgstamp)

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

$(DummyAlglibname).$(shlibsuffix) :: $(DummyAlglib) requirements $(use_requirements) $(DummyAlgstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" DummyAlg $(DummyAlg_shlibflags)
	$(lib_silent) cat /dev/null >$(DummyAlgshstamp)

$(DummyAlgshstamp) :: $(DummyAlglibname).$(shlibsuffix)
	$(lib_silent) if test -f $(DummyAlglibname).$(shlibsuffix) ; then cat /dev/null >$(DummyAlgshstamp) ; fi

DummyAlgclean ::
	$(cleanup_echo) objects DummyAlg
	$(cleanup_silent) /bin/rm -f $(bin)DummyAlg.o $(bin)DummyTool.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)DummyAlg.o $(bin)DummyTool.o) $(patsubst %.o,%.dep,$(bin)DummyAlg.o $(bin)DummyTool.o) $(patsubst %.o,%.d.stamp,$(bin)DummyAlg.o $(bin)DummyTool.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf DummyAlg_deps DummyAlg_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
DummyAlginstallname = $(library_prefix)DummyAlg$(library_suffix).$(shlibsuffix)

DummyAlg :: DummyAlginstall ;

install :: DummyAlginstall ;

DummyAlginstall :: $(install_dir)/$(DummyAlginstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(DummyAlginstallname) :: $(bin)$(DummyAlginstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DummyAlginstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##DummyAlgclean :: DummyAlguninstall

uninstall :: DummyAlguninstall ;

DummyAlguninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DummyAlginstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),DummyAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),DummyAlgprototype)

$(bin)DummyAlg_dependencies.make : $(use_requirements) $(cmt_final_setup_DummyAlg)
	$(echo) "(DummyAlg.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)DummyAlg.cc $(src)DummyTool.cc -end_all $(includes) $(app_DummyAlg_cppflags) $(lib_DummyAlg_cppflags) -name=DummyAlg $? -f=$(cmt_dependencies_in_DummyAlg) -without_cmt

-include $(bin)DummyAlg_dependencies.make

endif
endif
endif

DummyAlgclean ::
	$(cleanup_silent) \rm -rf $(bin)DummyAlg_deps $(bin)DummyAlg_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),DummyAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DummyAlg.d

$(bin)$(binobj)DummyAlg.d :

$(bin)$(binobj)DummyAlg.o : $(cmt_final_setup_DummyAlg)

$(bin)$(binobj)DummyAlg.o : $(src)DummyAlg.cc
	$(cpp_echo) $(src)DummyAlg.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(DummyAlg_pp_cppflags) $(lib_DummyAlg_pp_cppflags) $(DummyAlg_pp_cppflags) $(use_cppflags) $(DummyAlg_cppflags) $(lib_DummyAlg_cppflags) $(DummyAlg_cppflags) $(DummyAlg_cc_cppflags)  $(src)DummyAlg.cc
endif
endif

else
$(bin)DummyAlg_dependencies.make : $(DummyAlg_cc_dependencies)

$(bin)DummyAlg_dependencies.make : $(src)DummyAlg.cc

$(bin)$(binobj)DummyAlg.o : $(DummyAlg_cc_dependencies)
	$(cpp_echo) $(src)DummyAlg.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DummyAlg_pp_cppflags) $(lib_DummyAlg_pp_cppflags) $(DummyAlg_pp_cppflags) $(use_cppflags) $(DummyAlg_cppflags) $(lib_DummyAlg_cppflags) $(DummyAlg_cppflags) $(DummyAlg_cc_cppflags)  $(src)DummyAlg.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),DummyAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DummyTool.d

$(bin)$(binobj)DummyTool.d :

$(bin)$(binobj)DummyTool.o : $(cmt_final_setup_DummyAlg)

$(bin)$(binobj)DummyTool.o : $(src)DummyTool.cc
	$(cpp_echo) $(src)DummyTool.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(DummyAlg_pp_cppflags) $(lib_DummyAlg_pp_cppflags) $(DummyTool_pp_cppflags) $(use_cppflags) $(DummyAlg_cppflags) $(lib_DummyAlg_cppflags) $(DummyTool_cppflags) $(DummyTool_cc_cppflags)  $(src)DummyTool.cc
endif
endif

else
$(bin)DummyAlg_dependencies.make : $(DummyTool_cc_dependencies)

$(bin)DummyAlg_dependencies.make : $(src)DummyTool.cc

$(bin)$(binobj)DummyTool.o : $(DummyTool_cc_dependencies)
	$(cpp_echo) $(src)DummyTool.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DummyAlg_pp_cppflags) $(lib_DummyAlg_pp_cppflags) $(DummyTool_pp_cppflags) $(use_cppflags) $(DummyAlg_cppflags) $(lib_DummyAlg_cppflags) $(DummyTool_cppflags) $(DummyTool_cc_cppflags)  $(src)DummyTool.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: DummyAlgclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DummyAlg.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DummyAlgclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library DummyAlg
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)DummyAlg$(library_suffix).a $(library_prefix)DummyAlg$(library_suffix).$(shlibsuffix) DummyAlg.stamp DummyAlg.shstamp
#-- end of cleanup_library ---------------
