cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(QJsonAndTreeview)
## укажем проект 

set(CMAKE_CXX_STANDARD 17)
## используемые языки

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(
        CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} /JMC"
    )
else()
    set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-implicit-fallthrough"
    )
endif()

# укажем путь, где установлен Qt
SET(QT_ROOT_DIR "C:/Qt/6.6.0/msvc2019_64")

## укажем, что мы хотим найти пакет Qt6 или Qt5
find_package(QT NAMES Qt6 Qt5
  COMPONENTS Core Gui Widgets REQUIRED)

 ## укажем, что из найденного пакета нам нужны такие компоненты 
find_package(
  Qt${QT_VERSION_MAJOR}
  COMPONENTS Core Gui Widgets
  REQUIRED)

set(QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/osx/bin/qmake)


# Includes used Qt headers.
#include(${QT_USE_FILE})

file(GLOB sources
    ./*.cpp
    ./*.h
    ./*.ui
    ./*.qrc
    ./*.txt
)

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

##  укажем, что мы хотим собрать exe-файл
add_executable( ${PROJECT_NAME} ${sources} )

## и слинковать его с такими-то библиотеками
target_link_libraries( ${PROJECT_NAME}
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    PRIVATE Qt${QT_VERSION_MAJOR}::Gui
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    )
	
## также добавим макрос
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_LIBRARY)
