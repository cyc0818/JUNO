#-- start of make_header -----------------

#====================================
#  Document DummyAlg_python
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

cmt_DummyAlg_python_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DummyAlg_python_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DummyAlg_python

DummyAlg_tag = $(tag)

#cmt_local_tagfile_DummyAlg_python = $(DummyAlg_tag)_DummyAlg_python.make
cmt_local_tagfile_DummyAlg_python = $(bin)$(DummyAlg_tag)_DummyAlg_python.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DummyAlg_tag = $(tag)

#cmt_local_tagfile_DummyAlg_python = $(DummyAlg_tag).make
cmt_local_tagfile_DummyAlg_python = $(bin)$(DummyAlg_tag).make

endif

include $(cmt_local_tagfile_DummyAlg_python)
#-include $(cmt_local_tagfile_DummyAlg_python)

ifdef cmt_DummyAlg_python_has_target_tag

cmt_final_setup_DummyAlg_python = $(bin)setup_DummyAlg_python.make
cmt_dependencies_in_DummyAlg_python = $(bin)dependencies_DummyAlg_python.in
#cmt_final_setup_DummyAlg_python = $(bin)DummyAlg_DummyAlg_pythonsetup.make
cmt_local_DummyAlg_python_makefile = $(bin)DummyAlg_python.make

else

cmt_final_setup_DummyAlg_python = $(bin)setup.make
cmt_dependencies_in_DummyAlg_python = $(bin)dependencies.in
#cmt_final_setup_DummyAlg_python = $(bin)DummyAlgsetup.make
cmt_local_DummyAlg_python_makefile = $(bin)DummyAlg_python.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DummyAlgsetup.make

#DummyAlg_python :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DummyAlg_python'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DummyAlg_python/
#DummyAlg_python::
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

DummyAlg_python :: DummyAlg_pythoninstall

install :: DummyAlg_pythoninstall

DummyAlg_pythoninstall :: $(install_python_dir)
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

DummyAlg_pythonclean :: DummyAlg_pythonuninstall

uninstall :: DummyAlg_pythonuninstall

DummyAlg_pythonuninstall ::
	@if test "$(installarea)" = ""; then \
	  echo "Cannot uninstall header files, no installation source specified"; \
	else \
	  echo "Uninstalling files from $(dest)"; \
	  $(uninstall_command) "$(dest)" ; \
	fi


#-- end of install_python_header ------
#-- start of cleanup_header --------------

clean :: DummyAlg_pythonclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DummyAlg_python.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DummyAlg_pythonclean ::
#-- end of cleanup_header ---------------
