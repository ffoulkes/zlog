# zlog/src
#
# Copyright 2024 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

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
        INSTALL_RPATH "$ORIGIN/../lib"
    )
endif()

###########
# install #
###########

install(
    TARGETS zlog zlog_s zlog-chk-conf
    COMPONENT zlog
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    FILES zlog.h
    COMPONENT zlog
    DESTINATION include
)
