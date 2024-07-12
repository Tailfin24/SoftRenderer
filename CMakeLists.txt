cmake_minimum_required(VERSION 3.5)

project(SoftRenderer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE -Ofast)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2  -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2  -Wall")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp


)


set(LIB_DIRECTORY D:/Documents/package)
include_directories(${LIB_DIRECTORY}/assimp/include)
link_directories(${LIB_DIRECTORY}/assimp/lib)

if (LINUX)
    find_package(Eigen3 REQUIRED)
    include_directories(${EIGEN3_INCLUDE_DIRS})
else()
    include_directories("C:/Program Files (x86)/Eigen3/include/eigen3")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SoftRenderer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/core/maths.cpp
        src/core/maths.h
        src/core/model.cpp
        src/core/model.h
        src/core/rasterizer.cpp
        src/core/rasterizer.h
        src/core/scene.cpp
        src/core/scene.h
        src/core/tgaimage.cpp
        src/core/tgaimage.h
        src/core/camera.h
        src/core/camera.cpp
        src/shader/shader.h
        src/shader/phong_shader.cpp
        src/gui/framebufferwidget.cpp src/gui/framebufferwidget.h src/gui/mainwindow.cpp src/gui/mainwindow.h
        src/gui/lightcontrolwidget.h src/gui/lightcontrolwidget.cpp


    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SoftRenderer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SoftRenderer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SoftRenderer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SoftRenderer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SoftRenderer PRIVATE ${LIB_DIRECTORY}/assimp/lib/libassimp-5.dll)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SoftRenderer)
endif()
set_target_properties(SoftRenderer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SoftRenderer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SoftRenderer)
endif()
