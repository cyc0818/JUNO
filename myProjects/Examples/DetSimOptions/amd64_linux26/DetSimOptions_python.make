#-- start of make_header -----------------

#====================================
#  Document DetSimOptions_python
#
#   Generated Fri Dec 18 15:34:55 2020  by yuchincheng
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DetSimOptions_python_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DetSimOptions_python_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DetSimOptions_python

DetSimOptions_tag = $(tag)

#cmt_local_tagfile_DetSimOptions_python = $(DetSimOptions_tag)_DetSimOptions_python.make
cmt_local_tagfile_DetSimOptions_python = $(bin)$(DetSimOptions_tag)_DetSimOptions_python.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DetSimOptions_tag = $(tag)

#cmt_local_tagfile_DetSimOptions_python = $(DetSimOptions_tag).make
cmt_local_tagfile_DetSimOptions_python = $(bin)$(DetSimOptions_tag).make

endif

include $(cmt_local_tagfile_DetSimOptions_python)
#-include $(cmt_local_tagfile_DetSimOptions_python)

ifdef cmt_DetSimOptions_python_has_target_tag

cmt_final_setup_DetSimOptions_python = $(bin)setup_DetSimOptions_python.make
cmt_dependencies_in_DetSimOptions_python = $(bin)dependencies_DetSimOptions_python.in
#cmt_final_setup_DetSimOptions_python = $(bin)DetSimOptions_DetSimOptions_pythonsetup.make
cmt_local_DetSimOptions_python_makefile = $(bin)DetSimOptions_python.make

else

cmt_final_setup_DetSimOptions_python = $(bin)setup.make
cmt_dependencies_in_DetSimOptions_python = $(bin)dependencies.in
#cmt_final_setup_DetSimOptions_python = $(bin)DetSimOptionssetup.make
cmt_local_DetSimOptions_python_makefile = $(bin)DetSimOptions_python.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DetSimOptionssetup.make

#DetSimOptions_python :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DetSimOptions_python'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DetSimOptions_python/
#DetSimOptions_python::
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

DetSimOptions_python :: DetSimOptions_pythoninstall

install :: DetSimOptions_pythoninstall

DetSimOptions_pythoninstall :: $(install_python_dir)
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

DetSimOptions_pythonclean :: DetSimOptions_pythonuninstall

uninstall :: DetSimOptions_pythonuninstall

DetSimOptions_pythonuninstall ::
	@if test "$(installarea)" = ""; then \
	  echo "Cannot uninstall header files, no installation source specified"; \
	else \
	  echo "Uninstalling files from $(dest)"; \
	  $(uninstall_command) "$(dest)" ; \
	fi


#-- end of install_python_header ------
#-- start of cleanup_header --------------

clean :: DetSimOptions_pythonclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DetSimOptions_python.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DetSimOptions_pythonclean ::
#-- end of cleanup_header ---------------
