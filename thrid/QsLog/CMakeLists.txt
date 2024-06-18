cmake_minimum_required(VERSION 3.15)
project(Qslog VERSION 1.0 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )


add_library(Qslog STATIC
    QsLog.cpp 
	QsLog.h
    QsLogDest.cpp
	QsLogDest.h
    QsLogDestConsole.cpp 
	QsLogDestConsole.h
    QsLogDestFile.cpp
	QsLogDestFile.h
    QsLogDestFunctor.cpp 
	QsLogDestFunctor.h
    QsLogDisableForThisFile.h
    QsLogLevel.h

)
target_include_directories(Qslog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(Qslog PRIVATE
    QS_LOG_LINE_NUMBERS
)

target_link_libraries(Qslog PRIVATE
    Qt::Core
)
add_library(lib:log ALIAS Qslog)

