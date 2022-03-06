cmake_minimum_required(VERSION 3.18.5)
project(ioctl-parser LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(Parser ${PROJECT_NAME})

# Find Qt and setup auto processing for MOC, RCC, and UIC files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

# Add and configure the parser executable
add_executable(${Parser} main.cpp)
set_target_properties(${Parser} PROPERTIES AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${Parser} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/widgets)
# Add the directory which the UIC files are auto generated in
# source: https://stackoverflow.com/a/60203765/11039217
if(_isMultiConfig)
    target_include_directories(${Parser} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${Parser}_autogen/include_$<BUILD_INTERFACE:$<CONFIG>>)
else()
    target_include_directories(${Parser} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${Parser}_autogen/include)
endif()
target_sources(${Parser} PRIVATE
    resources/manifest.qrc
    widgets/HexSpinBox.hpp
    widgets/MainWindow.cpp
    widgets/MainWindow.hpp
    widgets/MainWindow.ui
)
target_link_libraries(${Parser} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

# Find and configure windeployqt for deployment when building on Windows
if(WIN32)
    # Add Qt5::windeployqt
    # source: https://stackoverflow.com/a/41199492/11039217
    if(TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
        get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
        execute_process(
            COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
            RESULT_VARIABLE return_code
            OUTPUT_VARIABLE qt5_install_prefix
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")
        if(EXISTS ${imported_location})
            add_executable(Qt5::windeployqt IMPORTED)
            set_target_properties(Qt5::windeployqt PROPERTIES IMPORTED_LOCATION ${imported_location})
        endif()
    endif()
    # Run Qt5::windeployqt at build time
    # source: https://stackoverflow.com/a/41199492/11039217
    if(TARGET Qt5::windeployqt)
        add_custom_command(TARGET ${Parser}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
            COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
            COMMAND Qt5::windeployqt --dir "$<TARGET_FILE_DIR:${Parser}>" "$<TARGET_FILE_DIR:${Parser}>/$<TARGET_FILE_NAME:${Parser}>"
                --no-patchqt --no-plugins --no-quick-import --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-webkit2 --no-angle --no-opengl-sw --no-svg
        )
    endif()
endif()