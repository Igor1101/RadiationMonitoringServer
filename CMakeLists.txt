cmake_minimum_required(VERSION 3.6)

project(RadiationMonitoringServer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core LinguistTools REQUIRED)

find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Sql)
set(TS_FILES igorm_messanger_uk_UA.ts)
include_directories(${CURSES_INCLUDE_DIR})

add_executable( ${PROJECT_NAME}
  main.cpp
  ${TS_FILES} server.cpp server.h defs.h db_mng.h db_mng.cpp
)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} Qt${QT_VERSION_MAJOR}::Core Qt5::Network Qt5::Sql)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )
