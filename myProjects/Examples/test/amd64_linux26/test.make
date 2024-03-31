#-- start of make_header -----------------

#====================================
#  Library test
#
#   Generated Wed Nov 25 15:37:00 2020  by yuchincheng
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_test_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_test_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_test

test_tag = $(tag)

#cmt_local_tagfile_test = $(test_tag)_test.make
cmt_local_tagfile_test = $(bin)$(test_tag)_test.make

else

tags      = $(tag),$(CMTEXTRATAGS)

test_tag = $(tag)

#cmt_local_tagfile_test = $(test_tag).make
cmt_local_tagfile_test = $(bin)$(test_tag).make

endif

include $(cmt_local_tagfile_test)
#-include $(cmt_local_tagfile_test)

ifdef cmt_test_has_target_tag

cmt_final_setup_test = $(bin)setup_test.make
cmt_dependencies_in_test = $(bin)dependencies_test.in
#cmt_final_setup_test = $(bin)test_testsetup.make
cmt_local_test_makefile = $(bin)test.make

else

cmt_final_setup_test = $(bin)setup.make
cmt_dependencies_in_test = $(bin)dependencies.in
#cmt_final_setup_test = $(bin)testsetup.make
cmt_local_test_makefile = $(bin)test.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)testsetup.make

#test :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'test'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = test/
#test::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

testlibname   = $(bin)$(library_prefix)test$(library_suffix)
testlib       = $(testlibname).a
teststamp     = $(bin)test.stamp
testshstamp   = $(bin)test.shstamp

test :: dirs  testLIB
	$(echo) "test ok"

cmt_test_has_prototypes = 1

#--------------------------------------

ifdef cmt_test_has_prototypes

testprototype :  ;

endif

testcompile : $(bin)testAlg.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

testLIB :: $(testlib) $(testshstamp)
	$(echo) "test : library ok"

$(testlib) :: $(bin)testAlg.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(testlib) $(bin)testAlg.o
	$(lib_silent) $(ranlib) $(testlib)
	$(lib_silent) cat /dev/null >$(teststamp)

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

$(testlibname).$(shlibsuffix) :: $(testlib) requirements $(use_requirements) $(teststamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" test $(test_shlibflags)
	$(lib_silent) cat /dev/null >$(testshstamp)

$(testshstamp) :: $(testlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(testlibname).$(shlibsuffix) ; then cat /dev/null >$(testshstamp) ; fi

testclean ::
	$(cleanup_echo) objects test
	$(cleanup_silent) /bin/rm -f $(bin)testAlg.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)testAlg.o) $(patsubst %.o,%.dep,$(bin)testAlg.o) $(patsubst %.o,%.d.stamp,$(bin)testAlg.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf test_deps test_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
testinstallname = $(library_prefix)test$(library_suffix).$(shlibsuffix)

test :: testinstall ;

install :: testinstall ;

testinstall :: $(install_dir)/$(testinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(testinstallname) :: $(bin)$(testinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(testinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##testclean :: testuninstall

uninstall :: testuninstall ;

testuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(testinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),testclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),testprototype)

$(bin)test_dependencies.make : $(use_requirements) $(cmt_final_setup_test)
	$(echo) "(test.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)testAlg.cc -end_all $(includes) $(app_test_cppflags) $(lib_test_cppflags) -name=test $? -f=$(cmt_dependencies_in_test) -without_cmt

-include $(bin)test_dependencies.make

endif
endif
endif

testclean ::
	$(cleanup_silent) \rm -rf $(bin)test_deps $(bin)test_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),testclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)testAlg.d

$(bin)$(binobj)testAlg.d :

$(bin)$(binobj)testAlg.o : $(cmt_final_setup_test)

$(bin)$(binobj)testAlg.o : $(src)testAlg.cc
	$(cpp_echo) $(src)testAlg.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(test_pp_cppflags) $(lib_test_pp_cppflags) $(testAlg_pp_cppflags) $(use_cppflags) $(test_cppflags) $(lib_test_cppflags) $(testAlg_cppflags) $(testAlg_cc_cppflags)  $(src)testAlg.cc
endif
endif

else
$(bin)test_dependencies.make : $(testAlg_cc_dependencies)

$(bin)test_dependencies.make : $(src)testAlg.cc

$(bin)$(binobj)testAlg.o : $(testAlg_cc_dependencies)
	$(cpp_echo) $(src)testAlg.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(test_pp_cppflags) $(lib_test_pp_cppflags) $(testAlg_pp_cppflags) $(use_cppflags) $(test_cppflags) $(lib_test_cppflags) $(testAlg_cppflags) $(testAlg_cc_cppflags)  $(src)testAlg.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: testclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(test.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

testclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library test
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)test$(library_suffix).a $(library_prefix)test$(library_suffix).$(shlibsuffix) test.stamp test.shstamp
#-- end of cleanup_library ---------------
