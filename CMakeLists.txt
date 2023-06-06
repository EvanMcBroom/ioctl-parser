cmake_minimum_required(VERSION 3.24.2)
project(ioctl-parser LANGUAGES CXX VERSION 1.1)
set(CMAKE_CXX_STANDARD 17)
set(Parser ${PROJECT_NAME})

# Force static linking of the VC runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Find Qt and setup auto processing for MOC, RCC, and UIC files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
qt_standard_project_setup()

# Add and configure the parser executable
qt_add_executable(${Parser} WIN32 MACOSX_BUNDLE main.cpp resources/icon.rc)
set_target_properties(${Parser} PROPERTIES AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${Parser} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/widgets)
target_sources(${Parser} PRIVATE
    resources/manifest.qrc
    widgets/HexSpinBox.hpp
    widgets/MainWindow.cpp
    widgets/MainWindow.hpp
    widgets/MainWindow.ui
)
target_link_libraries(${Parser} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Find and configure windeployqt for deployment when building on Windows
if(WIN32)
    # Run Qt6::windeployqt at build time
    # source: https://stackoverflow.com/a/41199492/11039217
    if(TARGET Qt6::windeployqt)
        add_custom_command(TARGET ${Parser}
            POST_BUILD
            COMMAND set PATH=%PATH%$<SEMICOLON>${Qt6_install_prefix}/bin
            COMMAND Qt6::windeployqt --dir "$<TARGET_FILE_DIR:${Parser}>" "$<TARGET_FILE_DIR:${Parser}>/$<TARGET_FILE_NAME:${Parser}>"
                $<IF:$<CONFIG:Debug>,--debug,--release>
                --no-network --no-patchqt --no-quick-import --no-translations --no-system-d3d-compiler --no-opengl-sw --no-svg
            COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:${Parser}>/imageformats"
        )
    endif()
endif()

# CPack settings
# Force the INSTALL and PACKAGE targets to be contained in the build tree
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/installs CACHE PATH "Project install tree" FORCE)
endif()
install(FILES "$<TARGET_FILE_DIR:${Parser}>/" DESTINATION .)
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "") # Prevent cpack from packaging source code
include(CPack)