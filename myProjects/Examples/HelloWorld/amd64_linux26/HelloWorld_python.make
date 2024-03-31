#-- start of make_header -----------------

#====================================
#  Document HelloWorld_python
#
#   Generated Thu Nov 26 15:03:45 2020  by yuchincheng
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_HelloWorld_python_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_HelloWorld_python_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_HelloWorld_python

HelloWorld_tag = $(tag)

#cmt_local_tagfile_HelloWorld_python = $(HelloWorld_tag)_HelloWorld_python.make
cmt_local_tagfile_HelloWorld_python = $(bin)$(HelloWorld_tag)_HelloWorld_python.make

else

tags      = $(tag),$(CMTEXTRATAGS)

HelloWorld_tag = $(tag)

#cmt_local_tagfile_HelloWorld_python = $(HelloWorld_tag).make
cmt_local_tagfile_HelloWorld_python = $(bin)$(HelloWorld_tag).make

endif

include $(cmt_local_tagfile_HelloWorld_python)
#-include $(cmt_local_tagfile_HelloWorld_python)

ifdef cmt_HelloWorld_python_has_target_tag

cmt_final_setup_HelloWorld_python = $(bin)setup_HelloWorld_python.make
cmt_dependencies_in_HelloWorld_python = $(bin)dependencies_HelloWorld_python.in
#cmt_final_setup_HelloWorld_python = $(bin)HelloWorld_HelloWorld_pythonsetup.make
cmt_local_HelloWorld_python_makefile = $(bin)HelloWorld_python.make

else

cmt_final_setup_HelloWorld_python = $(bin)setup.make
cmt_dependencies_in_HelloWorld_python = $(bin)dependencies.in
#cmt_final_setup_HelloWorld_python = $(bin)HelloWorldsetup.make
cmt_local_HelloWorld_python_makefile = $(bin)HelloWorld_python.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)HelloWorldsetup.make

#HelloWorld_python :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'HelloWorld_python'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = HelloWorld_python/
#HelloWorld_python::
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

HelloWorld_python :: HelloWorld_pythoninstall

install :: HelloWorld_pythoninstall

HelloWorld_pythoninstall :: $(install_python_dir)
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

HelloWorld_pythonclean :: HelloWorld_pythonuninstall

uninstall :: HelloWorld_pythonuninstall

HelloWorld_pythonuninstall ::
	@if test "$(installarea)" = ""; then \
	  echo "Cannot uninstall header files, no installation source specified"; \
	else \
	  echo "Uninstalling files from $(dest)"; \
	  $(uninstall_command) "$(dest)" ; \
	fi


#-- end of install_python_header ------
#-- start of cleanup_header --------------

clean :: HelloWorld_pythonclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(HelloWorld_python.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

HelloWorld_pythonclean ::
#-- end of cleanup_header ---------------
