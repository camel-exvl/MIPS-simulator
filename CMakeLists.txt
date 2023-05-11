cmake_minimum_required(VERSION 3.16)

project(MIPS-simulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup()

qt_add_executable(appMIPS-simulator
    main.cpp main.h assembler.cpp assembler.h disassembler.cpp disassembler.h systemsimulation.cpp systemsimulation.h
)

qt_add_qml_module(appMIPS-simulator
    URI MIPS-simulator
    VERSION 1.0
    QML_FILES Main.qml MessagePopup.qml
)

set_target_properties(appMIPS-simulator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMIPS-simulator
    PRIVATE Qt6::Quick Qt6::QuickControls2
)

install(TARGETS appMIPS-simulator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
