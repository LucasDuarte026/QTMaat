cmake_minimum_required(VERSION 3.16)

project(QTMaat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system filesystem thread)  # Adjust components as needed
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Boost library not found!")
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QTMaat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        sensorselectionwindow.h sensorselectionwindow.cpp
        addsensordialog.h addsensordialog.cpp
        removesensordialog.h removesensordialog.cpp
        viewsensorsdialog.h viewsensorsdialog.cpp
        servominas.h servominas.cpp

        datatypes.h

        # Adicionar arquivos do minas_src
        minas_src/ethercat_manager.h minas_src/ethercat_manager.cpp
        minas_src/minas_app.h minas_src/minas_app.cpp
        minas_src/minas_client.h minas_src/minas_client.cpp
        minas_src/time_stamp.h minas_src/time_stamp.cpp

        logservowindow.h logservowindow.cpp



    )
# Define target 2perties for Android with Qt 6 as:
#    set_property(TARGET QTMaat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QTMaat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QTMaat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QTMaat
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE ${Boost_LIBRARIES}       # Link Boost libraries
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/libsoem.a  # Link custom static library
)
# Incluir diretórios adicionais (include e minas_src)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/minas_src
)
# Adicionar a biblioteca estática do diretório lib
set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(QTMaat PRIVATE ${LIBRARY_PATH}/libsoem.a)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QTMaat)
endif()
set_target_properties(QTMaat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QTMaat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTMaat)
endif()
