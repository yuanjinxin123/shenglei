cmake_minimum_required(VERSION 3.15)
project(shengxiong VERSION 1.0 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent Gui Network SerialPort Sql)
set(QRC_FILE pic.qrc q.qrc)
qt5_add_resources(QRC ${QRC_FILE})
source_group(rc   FILES ${QRC_FILE})
file(GLOB_RECURSE ui        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ *.ui)
source_group(ui FILES ${ui})
set(TS_FILES shenglei_zh_CN.ts)
add_subdirectory(thrid/QsLog)
set(src config.cpp
    config.h
    dlg_act.cpp
    dlg_act.h
    iport iport.cpp
    jlcompressex.cpp
    jlcompressex.h
    loadingdialog.cpp
    loadingdialog.h
    log.cpp log.h
    mFristPage.cpp mFristPage.h
    macousto_pod.cpp macousto_pod.h
    macousto_pso.cpp macousto_pso.h
    macoustobase.cpp macoustobase.h
    macoustoparam.cpp macoustoparam.h
    maichongwdt.cpp maichongwdt.h
    main.cpp
    mainwindow.cpp mainwindow.h
    mconnectdlg.cpp mconnectdlg.h
    mcorrectiondlg.cpp mcorrectiondlg.h
    mdatecheckdlg.cpp mdatecheckdlg.h
    melectric.cpp melectric.h
    meletdlgadmin.cpp meletdlgadmin.h
    mexportdatedlg.cpp mexportdatedlg.h
    mfucwdt.cpp mfucwdt.h
    mhighestwdt.cpp mhighestwdt.h
    mlogindlg.cpp mlogindlg.h
    motherwdt.cpp motherwdt.h
    mportmanager.cpp mportmanager.h
    mpowerchage.cpp mpowerchage.h
    mpowerinfowdt.cpp mpowerinfowdt.h
    mpsobasewdt.cpp mpsobasewdt.h
    mpsowdt.cpp mpsowdt.h
    mseedparamwdt.cpp mseedparamwdt.h
    msempwdt.cpp msempwdt.h
    mserial.cpp mserial.h
    msettingdlg.cpp msettingdlg.h
    mtoolbtn.cpp mtoolbtn.h
    mwarminfo.cpp mwarminfo.h
    mwarnmap.h
    mwarnwdt.cpp mwarnwdt.h
    mwidget.cpp mwidget.h
    mytitlebar.cpp mytitlebar.h
    order.h
    portcfg.cpp portcfg.h
    spinbox.cpp spinbox.h
    sql.cpp sql.h
    titlebar.cpp titlebar.h
    xiaohao.cpp xiaohao.h)
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
set(CPACK_NSIS_COMPONENT_INSTALL ON)
include(cpack)
set(shengx shengxiong)
set(shengl shenglei)
macro(make_exe exe_target rc_file)

add_executable(${exe_target} WIN32 MACOSX_BUNDLE
    ${src}
    ${rc_file}
    ${ui}
    ${QRC}
   # ${TS_FILES}
)

target_include_directories(${exe_target} PRIVATE
    thrid/zlib/include
)

target_compile_definitions(${exe_target} PRIVATE
    QS_LOG_LINE_NUMBERS
)

target_link_libraries(${exe_target} PRIVATE
    Qt::Concurrent
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::SerialPort
    Qt::Sql
    Qt${QT_VERSION_MAJOR}::Widgets
    lib:log
)
set_target_properties(${exe_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/bin/${exe_target})

if(WIN32)
    target_link_directories(${exe_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thrid/zlib/lib/)
    target_link_libraries(${exe_target} PRIVATE
        quazip
    )
endif()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(${exe_target} PROPERTIES INSTALL_RPATH "$ORIGIN/lib")
#install(TARGETS shengxiong
#    LIBRARY DESTINATION lib)
#install(TARGETS ${exe_target}
#     DESTINATION .
#    )

file(GLOB_RECURSE MATH_LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/thrid/zlib/lib/*.dll)
install(FILES ${MATH_LIB_HEADERS} DESTINATION .)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${exe_target})
endif()


#set(VERSION_MAJOR 1)
#set(VERSION_MINOR 3)
#set(VERSION_PATCH 1)
#set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
#set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")

#set(PROJECT_VERSION_FULL ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
#获取编译时间
string(TIMESTAMP COMPILE_TIME %Y%m%d_%H%M%S)
set(build_time ${COMPILE_TIME})
#设置cpack

set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
# 获得qmake所在路径(Qt5.15.2/5.15.2/msvc2019_64/bin/qmake.exe)
get_target_property(qmake_exec_filepath Qt5::qmake IMPORTED_LOCATION)
# qt bin所在目录 (Qt5.15.2/5.15.2/msvc2019_64/bin)
get_filename_component(qt_exec_bin_dir "${qmake_exec_filepath}" DIRECTORY)
# windeployqt所在路径(Qt5.15.2/5.15.2/msvc2019_64/bin/windeployqt.exe)
find_program(windeployqt_exec_filepath windeployqt HINTS "${qt_exec_bin_dir}")

FILE(GLOB zlib_DLL_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/thrid/zlib/lib/*
)

add_custom_command(TARGET ${exe_target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_BINARY_DIR}/qtDeploy/${exe_target}"
        COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${qt_exec_bin_dir}" "${windeployqt_exec_filepath}"
        --dir "${CMAKE_BINARY_DIR}/qtDeploy/${exe_target}""$<TARGET_FILE:${exe_target}>"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${exe_target}> ${CMAKE_BINARY_DIR}/qtDeploy/${exe_target}
        COMMAND ${CMAKE_COMMAND} -E copy ${zlib_DLL_FILES} ${CMAKE_BINARY_DIR}/qtDeploy/${exe_target}
        COMMENT "Running windeployqt..."
        )
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_BINARY_DIR}/qtDeploy/${exe_target})
cpack_add_component(${exe_target}_setup
        DISPLAY_NAME  "${exe_target}"
        DESCRIPTION   "The program for ${exe_target}"
        GROUP ${exe_target}_grp)
cpack_add_component_group(${exe_target}_grp)
install(
        DIRECTORY "${CMAKE_BINARY_DIR}/qtDeploy/${exe_target}"
        DESTINATION ${exe_target}_dir
    COMPONENT
    ${exe_target}_setup
)

string(TIMESTAMP vTimeStamp "%Y%m%d%H%M%S")
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE vGitCommit
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE vGitBranch
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CPACK_PACKAGE_FILE_NAME "${exe_target}-${vTimeStamp}-${vGitBranch}-${vGitCommit}")
endmacro(make_exe)

make_exe(${shengx} logo.rc)
#make_exe(${shengl} logo_t.rc)
#    set(CMAKE_SKIP_BUILD_RPATH FALSE)
#    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#file(GLOB_RECURSE MATH_LIB_HEADERS ${CMAKE_BINARY_DIR}/*.dll)

#install(PROGRAMS ${MATH_LIB_HEADERS} DESTINATION lib)
#install(FILES ${MATH_LIB_HEADERS} DESTINATION .)








