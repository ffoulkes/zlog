# zlog/src
#
# Copyright 2024 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(zlog_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(zlog_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(zlog_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})

###################
# zlog_o (object) #
###################

add_library(zlog_o OBJECT
    buf.c
    buf.h
    category.c
    category.h
    category_table.c
    category_table.h
    conf.c
    conf.h
    event.c
    event.h
    fmacros.h
    format.c
    format.h
    level.c
    level.h
    level_list.c
    level_list.h
    lockfile.c
    lockfile.h
    mdc.c
    mdc.h
    record.c
    record.h
    record_table.c
    record_table.h
    rotater.c
    rotater.h
    rule.c
    rule.h
    spec.c
    spec.h
    thread.c
    thread.h
    version.h
    zc_arraylist.c
    zc_arraylist.h
    zc_defs.h
    zc_hashtable.c
    zc_hashtable.h
    zc_profile.c
    zc_profile.h
    zc_util.c
    zc_util.h
    zc_xplatform.h
    zlog.c
    zlog.h
)

if(WIN32)
    target_sources(zlog_o PRIVATE zlog_win.c zlog_win.h)
endif()

set_target_properties(zlog_o PROPERTIES POSITION_INDEPENDENT_CODE ON)

#######################
# set_zlog_properties #
#######################

function(set_zlog_properties TGT)
    target_link_libraries(${TGT} PUBLIC ${CMAKE_THREAD_PREFER_PTHREAD})
    if (WIN32)
        target_link_libraries(${TGT} PRIVATE ${UNIXEM_LIBRARY} Ws2_32)
    endif ()
endfunction()

#################
# zlog (shared) #
#################

add_library(zlog SHARED $<TARGET_OBJECTS:zlog_o>)
set_zlog_properties(zlog)

set_target_properties(zlog PROPERTIES
    VERSION ${ZLOG_VERSION}
    SOVERSION ${ZLOG_SO_VERSION}
)

###################
# zlog_s (static) #
###################

add_library(zlog_s STATIC $<TARGET_OBJECTS:zlog_o>)
set_zlog_properties(zlog_s)

set_target_properties(zlog_s PROPERTIES OUTPUT_NAME zlog)

#################
# zlog-chk-conf #
#################

add_executable(zlog-chk-conf zlog-chk-conf.c)

target_link_libraries(zlog-chk-conf PUBLIC zlog)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(zlog-chk-conf PROPERTIES
        INSTALL_RPATH "$ORIGIN/../${zlog_INSTALL_LIBDIR}"
    )
endif()

###########
# install #
###########

install(
    TARGETS zlog zlog_s zlog-chk-conf
    EXPORT ZlogTargets
    COMPONENT zlog
    ARCHIVE DESTINATION ${zlog_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${zlog_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${zlog_INSTALL_BINDIR}
    INCLUDES DESTINATION ${zlog_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY .
    COMPONENT zlog
    DESTINATION ${zlog_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING
      PATTERN "*.h"
)

###################
# pkg-config file #
###################

set(_pkgconfig_file ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/zlog.pc)

configure_file(cmake/zlog.pc.in ${_pkgconfig_file} @ONLY)

install(
  FILES ${_pkgconfig_file}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

###############
# config-file #
###############

install(
    EXPORT ZlogTargets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE zlog::
    DESTINATION ${zlog_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

configure_package_config_file(
    cmake/zlogConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${zlog_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    COMPONENT
        zlog
    DESTINATION
        ${zlog_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)
