#-- start of make_header -----------------

#====================================
#  Library DetSimAlg
#
#   Generated Fri Nov 20 00:10:22 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DetSimAlg_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DetSimAlg_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DetSimAlg

DetSimAlg_tag = $(tag)

#cmt_local_tagfile_DetSimAlg = $(DetSimAlg_tag)_DetSimAlg.make
cmt_local_tagfile_DetSimAlg = $(bin)$(DetSimAlg_tag)_DetSimAlg.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DetSimAlg_tag = $(tag)

#cmt_local_tagfile_DetSimAlg = $(DetSimAlg_tag).make
cmt_local_tagfile_DetSimAlg = $(bin)$(DetSimAlg_tag).make

endif

include $(cmt_local_tagfile_DetSimAlg)
#-include $(cmt_local_tagfile_DetSimAlg)

ifdef cmt_DetSimAlg_has_target_tag

cmt_final_setup_DetSimAlg = $(bin)setup_DetSimAlg.make
cmt_dependencies_in_DetSimAlg = $(bin)dependencies_DetSimAlg.in
#cmt_final_setup_DetSimAlg = $(bin)DetSimAlg_DetSimAlgsetup.make
cmt_local_DetSimAlg_makefile = $(bin)DetSimAlg.make

else

cmt_final_setup_DetSimAlg = $(bin)setup.make
cmt_dependencies_in_DetSimAlg = $(bin)dependencies.in
#cmt_final_setup_DetSimAlg = $(bin)DetSimAlgsetup.make
cmt_local_DetSimAlg_makefile = $(bin)DetSimAlg.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DetSimAlgsetup.make

#DetSimAlg :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DetSimAlg'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DetSimAlg/
#DetSimAlg::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

DetSimAlglibname   = $(bin)$(library_prefix)DetSimAlg$(library_suffix)
DetSimAlglib       = $(DetSimAlglibname).a
DetSimAlgstamp     = $(bin)DetSimAlg.stamp
DetSimAlgshstamp   = $(bin)DetSimAlg.shstamp

DetSimAlg :: dirs  DetSimAlgLIB
	$(echo) "DetSimAlg ok"

cmt_DetSimAlg_has_prototypes = 1

#--------------------------------------

ifdef cmt_DetSimAlg_has_prototypes

DetSimAlgprototype :  ;

endif

DetSimAlgcompile : $(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

DetSimAlgLIB :: $(DetSimAlglib) $(DetSimAlgshstamp)
	$(echo) "DetSimAlg : library ok"

$(DetSimAlglib) :: $(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(DetSimAlglib) $(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o
	$(lib_silent) $(ranlib) $(DetSimAlglib)
	$(lib_silent) cat /dev/null >$(DetSimAlgstamp)

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

$(DetSimAlglibname).$(shlibsuffix) :: $(DetSimAlglib) requirements $(use_requirements) $(DetSimAlgstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" DetSimAlg $(DetSimAlg_shlibflags)
	$(lib_silent) cat /dev/null >$(DetSimAlgshstamp)

$(DetSimAlgshstamp) :: $(DetSimAlglibname).$(shlibsuffix)
	$(lib_silent) if test -f $(DetSimAlglibname).$(shlibsuffix) ; then cat /dev/null >$(DetSimAlgshstamp) ; fi

DetSimAlgclean ::
	$(cleanup_echo) objects DetSimAlg
	$(cleanup_silent) /bin/rm -f $(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o) $(patsubst %.o,%.dep,$(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o) $(patsubst %.o,%.d.stamp,$(bin)DetSimAlg.o $(bin)MgrOfAnaElem.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf DetSimAlg_deps DetSimAlg_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
DetSimAlginstallname = $(library_prefix)DetSimAlg$(library_suffix).$(shlibsuffix)

DetSimAlg :: DetSimAlginstall ;

install :: DetSimAlginstall ;

DetSimAlginstall :: $(install_dir)/$(DetSimAlginstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(DetSimAlginstallname) :: $(bin)$(DetSimAlginstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DetSimAlginstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##DetSimAlgclean :: DetSimAlguninstall

uninstall :: DetSimAlguninstall ;

DetSimAlguninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DetSimAlginstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),DetSimAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),DetSimAlgprototype)

$(bin)DetSimAlg_dependencies.make : $(use_requirements) $(cmt_final_setup_DetSimAlg)
	$(echo) "(DetSimAlg.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)DetSimAlg.cc $(src)MgrOfAnaElem.cc -end_all $(includes) $(app_DetSimAlg_cppflags) $(lib_DetSimAlg_cppflags) -name=DetSimAlg $? -f=$(cmt_dependencies_in_DetSimAlg) -without_cmt

-include $(bin)DetSimAlg_dependencies.make

endif
endif
endif

DetSimAlgclean ::
	$(cleanup_silent) \rm -rf $(bin)DetSimAlg_deps $(bin)DetSimAlg_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),DetSimAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DetSimAlg.d

$(bin)$(binobj)DetSimAlg.d :

$(bin)$(binobj)DetSimAlg.o : $(cmt_final_setup_DetSimAlg)

$(bin)$(binobj)DetSimAlg.o : $(src)DetSimAlg.cc
	$(cpp_echo) $(src)DetSimAlg.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(DetSimAlg_pp_cppflags) $(lib_DetSimAlg_pp_cppflags) $(DetSimAlg_pp_cppflags) $(use_cppflags) $(DetSimAlg_cppflags) $(lib_DetSimAlg_cppflags) $(DetSimAlg_cppflags) $(DetSimAlg_cc_cppflags)  $(src)DetSimAlg.cc
endif
endif

else
$(bin)DetSimAlg_dependencies.make : $(DetSimAlg_cc_dependencies)

$(bin)DetSimAlg_dependencies.make : $(src)DetSimAlg.cc

$(bin)$(binobj)DetSimAlg.o : $(DetSimAlg_cc_dependencies)
	$(cpp_echo) $(src)DetSimAlg.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DetSimAlg_pp_cppflags) $(lib_DetSimAlg_pp_cppflags) $(DetSimAlg_pp_cppflags) $(use_cppflags) $(DetSimAlg_cppflags) $(lib_DetSimAlg_cppflags) $(DetSimAlg_cppflags) $(DetSimAlg_cc_cppflags)  $(src)DetSimAlg.cc

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),DetSimAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MgrOfAnaElem.d

$(bin)$(binobj)MgrOfAnaElem.d :

$(bin)$(binobj)MgrOfAnaElem.o : $(cmt_final_setup_DetSimAlg)

$(bin)$(binobj)MgrOfAnaElem.o : $(src)MgrOfAnaElem.cc
	$(cpp_echo) $(src)MgrOfAnaElem.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(DetSimAlg_pp_cppflags) $(lib_DetSimAlg_pp_cppflags) $(MgrOfAnaElem_pp_cppflags) $(use_cppflags) $(DetSimAlg_cppflags) $(lib_DetSimAlg_cppflags) $(MgrOfAnaElem_cppflags) $(MgrOfAnaElem_cc_cppflags)  $(src)MgrOfAnaElem.cc
endif
endif

else
$(bin)DetSimAlg_dependencies.make : $(MgrOfAnaElem_cc_dependencies)

$(bin)DetSimAlg_dependencies.make : $(src)MgrOfAnaElem.cc

$(bin)$(binobj)MgrOfAnaElem.o : $(MgrOfAnaElem_cc_dependencies)
	$(cpp_echo) $(src)MgrOfAnaElem.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DetSimAlg_pp_cppflags) $(lib_DetSimAlg_pp_cppflags) $(MgrOfAnaElem_pp_cppflags) $(use_cppflags) $(DetSimAlg_cppflags) $(lib_DetSimAlg_cppflags) $(MgrOfAnaElem_cppflags) $(MgrOfAnaElem_cc_cppflags)  $(src)MgrOfAnaElem.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: DetSimAlgclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DetSimAlg.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DetSimAlgclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library DetSimAlg
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)DetSimAlg$(library_suffix).a $(library_prefix)DetSimAlg$(library_suffix).$(shlibsuffix) DetSimAlg.stamp DetSimAlg.shstamp
#-- end of cleanup_library ---------------
