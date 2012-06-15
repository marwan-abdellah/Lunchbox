
# Copyright (c) 2012 Stefan Eilemann <eile@eyescale.ch>
# Info: http://www.itk.org/Wiki/CMake:Component_Install_With_CPack

string(TOUPPER ${CMAKE_PROJECT_NAME} UPPER_PROJECT_NAME)

set(${UPPER_PROJECT_NAME}_PACKAGE_VERSION "" CACHE 
  STRING "Additional build version for packages")
mark_as_advanced(${UPPER_PROJECT_NAME}_PACKAGE_VERSION)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}${VERSION_ABI}")
else()
  set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
endif()

if(NOT APPLE)
  # deb lintian insists on URL
  set(CPACK_PACKAGE_VENDOR "http://${CPACK_PACKAGE_VENDOR}")
endif()

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.txt)

# Feel free to override components and description after including this file
if(RELEASE_VERSION)
  set(CPACK_COMPONENTS_ALL lib dev)
else()
  set(CPACK_COMPONENTS_ALL unspecified lib dev)
endif()

set(CPACK_COMPONENT_UNSPECIFIED_DISPLAY_NAME "Unspecified")
set(CPACK_COMPONENT_UNSPECIFIED_DESCRIPTION
  "Unspecified Component - set COMPONENT in CMake install() command")

set(CPACK_COMPONENT_LIB_DISPLAY_NAME "${CMAKE_PROJECT_NAME} Libraries")
set(CPACK_COMPONENT_LIB_DESCRIPTION "${CMAKE_PROJECT_NAME} Runtime Libraries")

set(CPACK_COMPONENT_DEV_DISPLAY_NAME "${CMAKE_PROJECT_NAME} Development Files")
set(CPACK_COMPONENT_DEV_DESCRIPTION
  "Header and Library Files for ${CMAKE_PROJECT_NAME} Development")
set(CPACK_COMPONENT_DEV_DEPENDS lib)

# LSB system information
set(LSB_DISTRIBUTOR_ID "unknown")
set(LSB_RELEASE "unknown")
set(LSB_CODENAME "unknown")
find_program(LSB_RELEASE_EXECUTABLE lsb_release)
if(LSB_RELEASE_EXECUTABLE)
  execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -sc
    OUTPUT_VARIABLE LSB_CODENAME OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -sr
    OUTPUT_VARIABLE LSB_RELEASE OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -si
    OUTPUT_VARIABLE LSB_DISTRIBUTOR_ID OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(STATUS "LSB-Release system information:: Distributor-ID: "
    "${LSB_DISTRIBUTOR_ID}  Release: ${LSB_RELEASE}  Codename: ${LSB_CODENAME}")
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  find_program(RPM_EXE rpmbuild)
  find_program(DEB_EXE debuild)
endif()

# Heuristics to figure out cpack generator
if(MSVC)
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_MODIFY_PATH ON)
elseif(APPLE)
  set(CPACK_GENERATOR "PackageMaker")
  set(CPACK_OSX_PACKAGE_VERSION "${${UPPER_PROJECT_NAME}_OSX_VERSION}")
elseif(LSB_DISTRIBUTOR_ID MATCHES "Ubuntu")
  set(CPACK_GENERATOR "DEB")
elseif(LSB_DISTRIBUTOR_ID MATCHES "RedHatEnterpriseServer")
  set(CPACK_GENERATOR "RPM")
elseif(DEB_EXE)
  set(CPACK_GENERATOR "DEB")
elseif(RPM_EXE)
  set(CPACK_GENERATOR "RPM")
else()
  set(CPACK_GENERATOR "TGZ")
endif()

if(CPACK_GENERATOR STREQUAL "RPM")
  set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
  set(CPACK_RPM_PACKAGE_LICENSE "LGPL")
  set(CPACK_RPM_PACKAGE_RELEASE ${UPPER_PROJECT_NAME}_PACKAGE_VERSION)
  set(CPACK_RPM_PACKAGE_VERSION ${VERSION})
else()
  if(${UPPER_PROJECT_NAME}_PACKAGE_VERSION)
    set(CPACK_PACKAGE_VERSION_PATCH
      ${CPACK_PACKAGE_VERSION_PATCH}-${${UPPER_PROJECT_NAME}_PACKAGE_VERSION})
  endif()

  if(CPACK_GENERATOR STREQUAL "DEB")
    # dpkg requires lowercase package names
    string(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_DEBIAN_PACKAGE_NAME)
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "/sbin/ldconfig")

    if(NOT CPACK_DEBIAN_PACKAGE_MAINTAINER)
      set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
    endif()
  endif()
endif()

set(CPACK_STRIP_FILES TRUE)
include(InstallRequiredSystemLibraries)
include(CPack)

include(UploadPPA)
if(UPLOADPPA_FOUND)
  upload_ppas()
endif()