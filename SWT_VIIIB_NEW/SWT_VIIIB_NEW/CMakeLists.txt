cmake_minimum_required(VERSION 3.16)

project(SWT_VIIIB_NEW VERSION 0.1 LANGUAGES CXX)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/comm_dev)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/comm_dev/udp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/comm_dev/tcp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/comm_dev/protocol)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SWT_VIIIB_NEW
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        comm_dev/ICommunicator.h
        comm_dev/udp/UdpCommunicator.h
        comm_dev/udp/UdpCommunicator.cpp
        comm_dev/protocol/ProtocolTypes.h
        comm_dev/protocol/ProtocolCodec.h
        comm_dev/protocol/ProtocolCodec.cpp
        comm_dev/protocol/ProtocolHandler.h
        comm_dev/protocol/ProtocolHandler.cpp
        comm_dev/protocol/voltageregdialog.h comm_dev/protocol/voltageregdialog.cpp comm_dev/protocol/voltageregdialog.ui
        comm_dev/tcp/TcpCommunicator.h
        comm_dev/tcp/TcpCommunicator.cpp

        mainwindow.ui
        mainwindow.cpp
        mainwindow.h
        comm_dev/ICommunicator.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SWT_VIIIB_NEW APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SWT_VIIIB_NEW SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SWT_VIIIB_NEW
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SWT_VIIIB_NEW PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                            Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SWT_VIIIB_NEW)
endif()
set_target_properties(SWT_VIIIB_NEW PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SWT_VIIIB_NEW
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SWT_VIIIB_NEW)
endif()
