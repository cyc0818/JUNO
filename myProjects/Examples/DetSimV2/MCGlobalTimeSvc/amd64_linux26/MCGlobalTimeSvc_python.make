#-- start of make_header -----------------

#====================================
#  Document MCGlobalTimeSvc_python
#
#   Generated Fri Nov 20 00:09:26 2020  by lint
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MCGlobalTimeSvc_python_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MCGlobalTimeSvc_python_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MCGlobalTimeSvc_python

MCGlobalTimeSvc_tag = $(tag)

#cmt_local_tagfile_MCGlobalTimeSvc_python = $(MCGlobalTimeSvc_tag)_MCGlobalTimeSvc_python.make
cmt_local_tagfile_MCGlobalTimeSvc_python = $(bin)$(MCGlobalTimeSvc_tag)_MCGlobalTimeSvc_python.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MCGlobalTimeSvc_tag = $(tag)

#cmt_local_tagfile_MCGlobalTimeSvc_python = $(MCGlobalTimeSvc_tag).make
cmt_local_tagfile_MCGlobalTimeSvc_python = $(bin)$(MCGlobalTimeSvc_tag).make

endif

include $(cmt_local_tagfile_MCGlobalTimeSvc_python)
#-include $(cmt_local_tagfile_MCGlobalTimeSvc_python)

ifdef cmt_MCGlobalTimeSvc_python_has_target_tag

cmt_final_setup_MCGlobalTimeSvc_python = $(bin)setup_MCGlobalTimeSvc_python.make
cmt_dependencies_in_MCGlobalTimeSvc_python = $(bin)dependencies_MCGlobalTimeSvc_python.in
#cmt_final_setup_MCGlobalTimeSvc_python = $(bin)MCGlobalTimeSvc_MCGlobalTimeSvc_pythonsetup.make
cmt_local_MCGlobalTimeSvc_python_makefile = $(bin)MCGlobalTimeSvc_python.make

else

cmt_final_setup_MCGlobalTimeSvc_python = $(bin)setup.make
cmt_dependencies_in_MCGlobalTimeSvc_python = $(bin)dependencies.in
#cmt_final_setup_MCGlobalTimeSvc_python = $(bin)MCGlobalTimeSvcsetup.make
cmt_local_MCGlobalTimeSvc_python_makefile = $(bin)MCGlobalTimeSvc_python.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MCGlobalTimeSvcsetup.make

#MCGlobalTimeSvc_python :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MCGlobalTimeSvc_python'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MCGlobalTimeSvc_python/
#MCGlobalTimeSvc_python::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of install_python_header ------


installarea = ${CMTINSTALLAREA}
install_python_dir = $(installarea)

ifneq ($(strip "$(source)"),"")
src = ../$(source)
dest = $(install_python_dir)/python
else
src = ../python
dest = $(install_python_dir)
endif

ifneq ($(strip "$(offset)"),"")
dest = $(install_python_dir)/python
endif

MCGlobalTimeSvc_python :: MCGlobalTimeSvc_pythoninstall

install :: MCGlobalTimeSvc_pythoninstall

MCGlobalTimeSvc_pythoninstall :: $(install_python_dir)
	@if [ ! "$(installarea)" = "" ] ; then\
	  echo "installation done"; \
	fi

$(install_python_dir) ::
	@if [ "$(installarea)" = "" ] ; then \
	  echo "Cannot install header files, no installation source specified"; \
	else \
	  if [ -d $(src) ] ; then \
	    echo "Installing files from $(src) to $(dest)" ; \
	    if [ "$(offset)" = "" ] ; then \
	      $(install_command) --exclude="*.py?" $(src) $(dest) ; \
	    else \
	      $(install_command) --exclude="*.py?" $(src) $(dest) --destname $(offset); \
	    fi ; \
	  else \
	    echo "no source  $(src)"; \
	  fi; \
	fi

MCGlobalTimeSvc_pythonclean :: MCGlobalTimeSvc_pythonuninstall

uninstall :: MCGlobalTimeSvc_pythonuninstall

MCGlobalTimeSvc_pythonuninstall ::
	@if test "$(installarea)" = ""; then \
	  echo "Cannot uninstall header files, no installation source specified"; \
	else \
	  echo "Uninstalling files from $(dest)"; \
	  $(uninstall_command) "$(dest)" ; \
	fi


#-- end of install_python_header ------
#-- start of cleanup_header --------------

clean :: MCGlobalTimeSvc_pythonclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MCGlobalTimeSvc_python.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

MCGlobalTimeSvc_pythonclean ::
#-- end of cleanup_header ---------------
