
message(STATUS "Offline Version: ")
message(STATUS "Major: ${offline_VERSION_MAJOR}")
message(STATUS "Minor: ${offline_VERSION_MINOR}")
message(STATUS "Patch: ${offline_VERSION_PATCH}")

##############################################################################
# If using GIT, then use git to get the information
##############################################################################

if (EXISTS ${PROJECT_SOURCE_DIR}/.git)
  find_package(Git)

  if (Git_FOUND)
    # Get the current commit
    execute_process(
      COMMAND git log -1 --format=%h
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # check if a commit is a tag
    # if it is a tag, then use the tag name
    execute_process(
      COMMAND git tag --points-at ${GIT_COMMIT_HASH}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_TAG_NAME
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if (GIT_COMMIT_TAG_NAME)
      # if it is a tag
      message(STATUS "GIT_COMMIT_TAG_NAME: ${GIT_COMMIT_TAG_NAME}")
      set(offline_VERSION "${GIT_COMMIT_TAG_NAME}")

    else()
      # if it is not a tag, then we use the development version
      # for the id part, we use 'g' as prefix to indicate it is a git commit
      string(TIMESTAMP offline_VERSION_PATCH %Y%m%d)
      set(offline_VERSION "J${offline_VERSION_MAJOR}.${offline_VERSION_MINOR}.${offline_VERSION_PATCH}-g${GIT_COMMIT_HASH}")


    endif()

  endif()

endif()

##############################################################################
# If using SVN, then use svn info to get the information
##############################################################################

if (EXISTS ${PROJECT_SOURCE_DIR}/.svn)
  find_package(Subversion)

  Subversion_WC_INFO(${PROJECT_SOURCE_DIR} offline)
  message("Current revision is ${offline_WC_REVISION}")
  message("Current url is ${offline_WC_URL}")
  message("Current root url is ${offline_WC_ROOT}")

  string(REPLACE "${offline_WC_ROOT}" "" offline_WC_RELURL "${offline_WC_URL}")
  message("Current rel url is ${offline_WC_RELURL}")

  # url is /trunk, /branches/(.*), /tags/(.*)
  # only if it is tag, use the tag directly. otherwise need to use the build date
  if ("${offline_WC_RELURL}" MATCHES "^/trunk$")
    string(TIMESTAMP offline_VERSION_PATCH %Y%m%d)

    message(STATUS "Update offline_VERSION_PATCH to build date: ${offline_VERSION_PATCH}")

    set(offline_VERSION "J${offline_VERSION_MAJOR}.${offline_VERSION_MINOR}.${offline_VERSION_PATCH}-r${offline_WC_REVISION}")

  elseif ("${offline_WC_RELURL}" MATCHES "^/branches/(.*)$")

    message(STATUS "Update offline_VERSION_PATCH to build date: ${offline_VERSION_PATCH}")

    set(offline_VERSION "J${offline_VERSION_MAJOR}.${offline_VERSION_MINOR}.${offline_VERSION_PATCH}-r${offline_WC_REVISION}")


  elseif ("${offline_WC_RELURL}" MATCHES "^/tags/(.*)$")
    set(offline_VERSION "${CMAKE_MATCH_1}")
  
  else()
    # do nothing
  endif()
endif()


##############################################################################
# Final offline_VERSION
##############################################################################
message(STATUS "Update offline_VERSION: ${offline_VERSION}")
