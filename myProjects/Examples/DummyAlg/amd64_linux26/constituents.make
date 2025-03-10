
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

DummyAlg_tag = $(tag)

#cmt_local_tagfile = $(DummyAlg_tag).make
cmt_local_tagfile = $(bin)$(DummyAlg_tag).make

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

#cmt_local_setup = $(bin)setup$$$$.make
#cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)DummyAlgsetup.make
cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)$(package)setup.make

cmt_build_library_linksstamp = $(bin)cmt_build_library_links.stamp
#--------------------------------------------------------

#cmt_lock_setup = /tmp/lock$(cmt_lock_pid).make
#cmt_temp_tag = /tmp/tag$(cmt_lock_pid).make

#first :: $(cmt_local_tagfile)
#	@echo $(cmt_local_tagfile) ok
#ifndef QUICK
#first :: $(cmt_final_setup) ;
#else
#first :: ;
#endif

##	@bin=`$(cmtexe) show macro_value bin`

#$(cmt_local_tagfile) : $(cmt_lock_setup)
#	@echo "#CMT> Error: $@: No such file" >&2; exit 1
#$(cmt_local_tagfile) :
#	@echo "#CMT> Warning: $@: No such file" >&2; exit
#	@echo "#CMT> Info: $@: No need to rebuild file" >&2; exit

#$(cmt_final_setup) : $(cmt_local_tagfile) 
#	$(echo) "(constituents.make) Rebuilding $@"
#	@if test ! -d $(@D); then $(mkdir) -p $(@D); fi; \
#	  if test -f $(cmt_local_setup); then /bin/rm -f $(cmt_local_setup); fi; \
#	  trap '/bin/rm -f $(cmt_local_setup)' 0 1 2 15; \
#	  $(cmtexe) -tag=$(tags) show setup >>$(cmt_local_setup); \
#	  if test ! -f $@; then \
#	    mv $(cmt_local_setup) $@; \
#	  else \
#	    if /usr/bin/diff $(cmt_local_setup) $@ >/dev/null ; then \
#	      : ; \
#	    else \
#	      mv $(cmt_local_setup) $@; \
#	    fi; \
#	  fi

#	@/bin/echo $@ ok   

#config :: checkuses
#	@exit 0
#checkuses : ;

env.make ::
	printenv >env.make.tmp; $(cmtexe) check files env.make.tmp env.make

ifndef QUICK
all :: build_library_links ;
else
all :: $(cmt_build_library_linksstamp) ;
endif

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

dirs :: requirements
	@if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi
#	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
#	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

#requirements :
#	@if test ! -r requirements ; then echo "No requirements file" ; fi

build_library_links : dirs
	$(echo) "(constituents.make) Rebuilding library links"; \
	 $(build_library_links)
#	if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi; \
#	$(build_library_links)

$(cmt_build_library_linksstamp) : $(cmt_final_setup) $(cmt_local_tagfile) $(bin)library_links.in
	$(echo) "(constituents.make) Rebuilding library links"; \
	 $(build_library_links) -f=$(bin)library_links.in -without_cmt
	$(silent) \touch $@

ifndef PEDANTIC
.DEFAULT ::
#.DEFAULT :
	$(echo) "(constituents.make) $@: No rule for such target" >&2
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of constituents_header ------
#-- start of group ------

all_groups :: all

all :: $(all_dependencies)  $(all_pre_constituents) $(all_constituents)  $(all_post_constituents)
	$(echo) "all ok."

#	@/bin/echo " all ok."

clean :: allclean

allclean ::  $(all_constituentsclean)
	$(echo) $(all_constituentsclean)
	$(echo) "allclean ok."

#	@echo $(all_constituentsclean)
#	@/bin/echo " allclean ok."

#-- end of group ------
#-- start of group ------

all_groups :: cmt_actions

cmt_actions :: $(cmt_actions_dependencies)  $(cmt_actions_pre_constituents) $(cmt_actions_constituents)  $(cmt_actions_post_constituents)
	$(echo) "cmt_actions ok."

#	@/bin/echo " cmt_actions ok."

clean :: allclean

cmt_actionsclean ::  $(cmt_actions_constituentsclean)
	$(echo) $(cmt_actions_constituentsclean)
	$(echo) "cmt_actionsclean ok."

#	@echo $(cmt_actions_constituentsclean)
#	@/bin/echo " cmt_actionsclean ok."

#-- end of group ------
#-- start of constituent_app_lib ------

cmt_DummyAlg_has_no_target_tag = 1
cmt_DummyAlg_has_prototypes = 1

#--------------------------------------

ifdef cmt_DummyAlg_has_target_tag

cmt_local_tagfile_DummyAlg = $(bin)$(DummyAlg_tag)_DummyAlg.make
cmt_final_setup_DummyAlg = $(bin)setup_DummyAlg.make
cmt_local_DummyAlg_makefile = $(bin)DummyAlg.make

DummyAlg_extratags = -tag_add=target_DummyAlg

else

cmt_local_tagfile_DummyAlg = $(bin)$(DummyAlg_tag).make
cmt_final_setup_DummyAlg = $(bin)setup.make
cmt_local_DummyAlg_makefile = $(bin)DummyAlg.make

endif

not_DummyAlgcompile_dependencies = { n=0; for p in $?; do m=0; for d in $(DummyAlgcompile_dependencies); do if [ $$p = $$d ]; then m=1; break; fi; done; if [ $$m -eq 0 ]; then n=1; break; fi; done; [ $$n -eq 1 ]; }

ifdef STRUCTURED_OUTPUT
DummyAlgdirs :
	@if test ! -d $(bin)DummyAlg; then $(mkdir) -p $(bin)DummyAlg; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)DummyAlg
else
DummyAlgdirs : ;
endif

ifdef cmt_DummyAlg_has_target_tag

ifndef QUICK
$(cmt_local_DummyAlg_makefile) : $(DummyAlgcompile_dependencies) build_library_links
	$(echo) "(constituents.make) Building DummyAlg.make"; \
	  $(cmtexe) -tag=$(tags) $(DummyAlg_extratags) build constituent_config -out=$(cmt_local_DummyAlg_makefile) DummyAlg
else
$(cmt_local_DummyAlg_makefile) : $(DummyAlgcompile_dependencies) $(cmt_build_library_linksstamp) $(use_requirements)
	@if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_DummyAlg) ] || \
	  [ ! -f $(cmt_final_setup_DummyAlg) ] || \
	  $(not_DummyAlgcompile_dependencies) ; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building DummyAlg.make"; \
	  $(cmtexe) -tag=$(tags) $(DummyAlg_extratags) build constituent_config -out=$(cmt_local_DummyAlg_makefile) DummyAlg; \
	  fi
endif

else

ifndef QUICK
$(cmt_local_DummyAlg_makefile) : $(DummyAlgcompile_dependencies) build_library_links
	$(echo) "(constituents.make) Building DummyAlg.make"; \
	  $(cmtexe) -f=$(bin)DummyAlg.in -tag=$(tags) $(DummyAlg_extratags) build constituent_makefile -without_cmt -out=$(cmt_local_DummyAlg_makefile) DummyAlg
else
$(cmt_local_DummyAlg_makefile) : $(DummyAlgcompile_dependencies) $(cmt_build_library_linksstamp) $(bin)DummyAlg.in
	@if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_DummyAlg) ] || \
	  [ ! -f $(cmt_final_setup_DummyAlg) ] || \
	  $(not_DummyAlgcompile_dependencies) ; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building DummyAlg.make"; \
	  $(cmtexe) -f=$(bin)DummyAlg.in -tag=$(tags) $(DummyAlg_extratags) build constituent_makefile -without_cmt -out=$(cmt_local_DummyAlg_makefile) DummyAlg; \
	  fi
endif

endif

#	  $(cmtexe) -tag=$(tags) $(DummyAlg_extratags) build constituent_makefile -out=$(cmt_local_DummyAlg_makefile) DummyAlg

DummyAlg :: DummyAlgcompile DummyAlginstall ;

ifdef cmt_DummyAlg_has_prototypes

DummyAlgprototype : $(DummyAlgprototype_dependencies) $(cmt_local_DummyAlg_makefile) dirs DummyAlgdirs
	$(echo) "(constituents.make) Starting $@"
	@if test -f $(cmt_local_DummyAlg_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_makefile) $@; \
	  fi
#	@$(MAKE) -f $(cmt_local_DummyAlg_makefile) $@
	$(echo) "(constituents.make) $@ done"

DummyAlgcompile : DummyAlgprototype

endif

DummyAlgcompile : $(DummyAlgcompile_dependencies) $(cmt_local_DummyAlg_makefile) dirs DummyAlgdirs
	$(echo) "(constituents.make) Starting $@"
	@if test -f $(cmt_local_DummyAlg_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_makefile) $@; \
	  fi
#	@$(MAKE) -f $(cmt_local_DummyAlg_makefile) $@
	$(echo) "(constituents.make) $@ done"

clean :: DummyAlgclean ;

DummyAlgclean :: $(DummyAlgclean_dependencies) ##$(cmt_local_DummyAlg_makefile)
	$(echo) "(constituents.make) Starting $@"
	@-if test -f $(cmt_local_DummyAlg_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_makefile) $@; \
	fi
	$(echo) "(constituents.make) $@ done"
#	@-$(MAKE) -f $(cmt_local_DummyAlg_makefile) DummyAlgclean

##	  /bin/rm -f $(cmt_local_DummyAlg_makefile) $(bin)DummyAlg_dependencies.make

install :: DummyAlginstall ;

DummyAlginstall :: DummyAlgcompile $(DummyAlg_dependencies) $(cmt_local_DummyAlg_makefile)
	$(echo) "(constituents.make) Starting $@"
	@if test -f $(cmt_local_DummyAlg_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_makefile) $@; \
	  fi
#	@$(MAKE) -f $(cmt_local_DummyAlg_makefile) $@
	$(echo) "(constituents.make) $@ done"

uninstall : DummyAlguninstall

$(foreach d,$(DummyAlg_dependencies),$(eval $(d)uninstall_dependencies += DummyAlguninstall))

DummyAlguninstall : $(DummyAlguninstall_dependencies) ##$(cmt_local_DummyAlg_makefile)
	$(echo) "(constituents.make) Starting $@"
	@-if test -f $(cmt_local_DummyAlg_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_makefile) uninstall; \
	  fi
#	@$(MAKE) -f $(cmt_local_DummyAlg_makefile) uninstall
	$(echo) "(constituents.make) $@ done"

remove_library_links :: DummyAlguninstall ;

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ DummyAlg"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ DummyAlg done"
endif

#-- end of constituent_app_lib ------
#-- start of constituent ------

cmt_DummyAlg_python_has_no_target_tag = 1

#--------------------------------------

ifdef cmt_DummyAlg_python_has_target_tag

cmt_local_tagfile_DummyAlg_python = $(bin)$(DummyAlg_tag)_DummyAlg_python.make
cmt_final_setup_DummyAlg_python = $(bin)setup_DummyAlg_python.make
cmt_local_DummyAlg_python_makefile = $(bin)DummyAlg_python.make

DummyAlg_python_extratags = -tag_add=target_DummyAlg_python

else

cmt_local_tagfile_DummyAlg_python = $(bin)$(DummyAlg_tag).make
cmt_final_setup_DummyAlg_python = $(bin)setup.make
cmt_local_DummyAlg_python_makefile = $(bin)DummyAlg_python.make

endif

not_DummyAlg_python_dependencies = { n=0; for p in $?; do m=0; for d in $(DummyAlg_python_dependencies); do if [ $$p = $$d ]; then m=1; break; fi; done; if [ $$m -eq 0 ]; then n=1; break; fi; done; [ $$n -eq 1 ]; }

ifdef STRUCTURED_OUTPUT
DummyAlg_pythondirs :
	@if test ! -d $(bin)DummyAlg_python; then $(mkdir) -p $(bin)DummyAlg_python; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)DummyAlg_python
else
DummyAlg_pythondirs : ;
endif

ifdef cmt_DummyAlg_python_has_target_tag

ifndef QUICK
$(cmt_local_DummyAlg_python_makefile) : $(DummyAlg_python_dependencies) build_library_links
	$(echo) "(constituents.make) Building DummyAlg_python.make"; \
	  $(cmtexe) -tag=$(tags) $(DummyAlg_python_extratags) build constituent_config -out=$(cmt_local_DummyAlg_python_makefile) DummyAlg_python
else
$(cmt_local_DummyAlg_python_makefile) : $(DummyAlg_python_dependencies) $(cmt_build_library_linksstamp) $(use_requirements)
	@if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_DummyAlg_python) ] || \
	  [ ! -f $(cmt_final_setup_DummyAlg_python) ] || \
	  $(not_DummyAlg_python_dependencies) ; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building DummyAlg_python.make"; \
	  $(cmtexe) -tag=$(tags) $(DummyAlg_python_extratags) build constituent_config -out=$(cmt_local_DummyAlg_python_makefile) DummyAlg_python; \
	  fi
endif

else

ifndef QUICK
$(cmt_local_DummyAlg_python_makefile) : $(DummyAlg_python_dependencies) build_library_links
	$(echo) "(constituents.make) Building DummyAlg_python.make"; \
	  $(cmtexe) -f=$(bin)DummyAlg_python.in -tag=$(tags) $(DummyAlg_python_extratags) build constituent_makefile -without_cmt -out=$(cmt_local_DummyAlg_python_makefile) DummyAlg_python
else
$(cmt_local_DummyAlg_python_makefile) : $(DummyAlg_python_dependencies) $(cmt_build_library_linksstamp) $(bin)DummyAlg_python.in
	@if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_DummyAlg_python) ] || \
	  [ ! -f $(cmt_final_setup_DummyAlg_python) ] || \
	  $(not_DummyAlg_python_dependencies) ; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building DummyAlg_python.make"; \
	  $(cmtexe) -f=$(bin)DummyAlg_python.in -tag=$(tags) $(DummyAlg_python_extratags) build constituent_makefile -without_cmt -out=$(cmt_local_DummyAlg_python_makefile) DummyAlg_python; \
	  fi
endif

endif

#	  $(cmtexe) -tag=$(tags) $(DummyAlg_python_extratags) build constituent_makefile -out=$(cmt_local_DummyAlg_python_makefile) DummyAlg_python

DummyAlg_python :: $(DummyAlg_python_dependencies) $(cmt_local_DummyAlg_python_makefile) dirs DummyAlg_pythondirs
	$(echo) "(constituents.make) Starting DummyAlg_python"
	@if test -f $(cmt_local_DummyAlg_python_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_python_makefile) DummyAlg_python; \
	  fi
#	@$(MAKE) -f $(cmt_local_DummyAlg_python_makefile) DummyAlg_python
	$(echo) "(constituents.make) DummyAlg_python done"

clean :: DummyAlg_pythonclean ;

DummyAlg_pythonclean :: $(DummyAlg_pythonclean_dependencies) ##$(cmt_local_DummyAlg_python_makefile)
	$(echo) "(constituents.make) Starting DummyAlg_pythonclean"
	@-if test -f $(cmt_local_DummyAlg_python_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_python_makefile) DummyAlg_pythonclean; \
	fi
	$(echo) "(constituents.make) DummyAlg_pythonclean done"
#	@-$(MAKE) -f $(cmt_local_DummyAlg_python_makefile) DummyAlg_pythonclean

##	  /bin/rm -f $(cmt_local_DummyAlg_python_makefile) $(bin)DummyAlg_python_dependencies.make

install :: DummyAlg_pythoninstall ;

DummyAlg_pythoninstall :: $(DummyAlg_python_dependencies) $(cmt_local_DummyAlg_python_makefile)
	$(echo) "(constituents.make) Starting $@"
	@if test -f $(cmt_local_DummyAlg_python_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_python_makefile) install; \
	  fi
#	@-$(MAKE) -f $(cmt_local_DummyAlg_python_makefile) install
	$(echo) "(constituents.make) $@ done"

uninstall : DummyAlg_pythonuninstall

$(foreach d,$(DummyAlg_python_dependencies),$(eval $(d)uninstall_dependencies += DummyAlg_pythonuninstall))

DummyAlg_pythonuninstall : $(DummyAlg_pythonuninstall_dependencies) ##$(cmt_local_DummyAlg_python_makefile)
	$(echo) "(constituents.make) Starting $@"
	@-if test -f $(cmt_local_DummyAlg_python_makefile); then \
	  $(MAKE) -f $(cmt_local_DummyAlg_python_makefile) uninstall; \
	  fi
#	@$(MAKE) -f $(cmt_local_DummyAlg_python_makefile) uninstall
	$(echo) "(constituents.make) $@ done"

remove_library_links :: DummyAlg_pythonuninstall ;

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ DummyAlg_python"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ DummyAlg_python done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_make_has_no_target_tag = 1

#--------------------------------------

ifdef cmt_make_has_target_tag

cmt_local_tagfile_make = $(bin)$(DummyAlg_tag)_make.make
cmt_final_setup_make = $(bin)setup_make.make
cmt_local_make_makefile = $(bin)make.make

make_extratags = -tag_add=target_make

else

cmt_local_tagfile_make = $(bin)$(DummyAlg_tag).make
cmt_final_setup_make = $(bin)setup.make
cmt_local_make_makefile = $(bin)make.make

endif

not_make_dependencies = { n=0; for p in $?; do m=0; for d in $(make_dependencies); do if [ $$p = $$d ]; then m=1; break; fi; done; if [ $$m -eq 0 ]; then n=1; break; fi; done; [ $$n -eq 1 ]; }

ifdef STRUCTURED_OUTPUT
makedirs :
	@if test ! -d $(bin)make; then $(mkdir) -p $(bin)make; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)make
else
makedirs : ;
endif

ifdef cmt_make_has_target_tag

ifndef QUICK
$(cmt_local_make_makefile) : $(make_dependencies) build_library_links
	$(echo) "(constituents.make) Building make.make"; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_config -out=$(cmt_local_make_makefile) make
else
$(cmt_local_make_makefile) : $(make_dependencies) $(cmt_build_library_linksstamp) $(use_requirements)
	@if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_make) ] || \
	  [ ! -f $(cmt_final_setup_make) ] || \
	  $(not_make_dependencies) ; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building make.make"; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_config -out=$(cmt_local_make_makefile) make; \
	  fi
endif

else

ifndef QUICK
$(cmt_local_make_makefile) : $(make_dependencies) build_library_links
	$(echo) "(constituents.make) Building make.make"; \
	  $(cmtexe) -f=$(bin)make.in -tag=$(tags) $(make_extratags) build constituent_makefile -without_cmt -out=$(cmt_local_make_makefile) make
else
$(cmt_local_make_makefile) : $(make_dependencies) $(cmt_build_library_linksstamp) $(bin)make.in
	@if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_make) ] || \
	  [ ! -f $(cmt_final_setup_make) ] || \
	  $(not_make_dependencies) ; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building make.make"; \
	  $(cmtexe) -f=$(bin)make.in -tag=$(tags) $(make_extratags) build constituent_makefile -without_cmt -out=$(cmt_local_make_makefile) make; \
	  fi
endif

endif

#	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_makefile -out=$(cmt_local_make_makefile) make

make :: $(make_dependencies) $(cmt_local_make_makefile) dirs makedirs
	$(echo) "(constituents.make) Starting make"
	@if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) make; \
	  fi
#	@$(MAKE) -f $(cmt_local_make_makefile) make
	$(echo) "(constituents.make) make done"

clean :: makeclean ;

makeclean :: $(makeclean_dependencies) ##$(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting makeclean"
	@-if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) makeclean; \
	fi
	$(echo) "(constituents.make) makeclean done"
#	@-$(MAKE) -f $(cmt_local_make_makefile) makeclean

##	  /bin/rm -f $(cmt_local_make_makefile) $(bin)make_dependencies.make

install :: makeinstall ;

makeinstall :: $(make_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting $@"
	@if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) install; \
	  fi
#	@-$(MAKE) -f $(cmt_local_make_makefile) install
	$(echo) "(constituents.make) $@ done"

uninstall : makeuninstall

$(foreach d,$(make_dependencies),$(eval $(d)uninstall_dependencies += makeuninstall))

makeuninstall : $(makeuninstall_dependencies) ##$(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting $@"
	@-if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) uninstall; \
	  fi
#	@$(MAKE) -f $(cmt_local_make_makefile) uninstall
	$(echo) "(constituents.make) $@ done"

remove_library_links :: makeuninstall ;

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ make"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ make done"
endif

#-- end of constituent ------
#-- start of constituents_trailer ------

uninstall : remove_library_links ;
clean ::
	$(cleanup_echo) $(cmt_build_library_linksstamp)
	-$(cleanup_silent) \rm -f $(cmt_build_library_linksstamp)
#clean :: remove_library_links

remove_library_links ::
ifndef QUICK
	$(echo) "(constituents.make) Removing library links"; \
	  $(remove_library_links)
else
	$(echo) "(constituents.make) Removing library links"; \
	  $(remove_library_links) -f=$(bin)library_links.in -without_cmt
endif

#-- end of constituents_trailer ------
