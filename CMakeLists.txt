cmake_minimum_required(VERSION 3.14)

project(CameraCalibration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用自动 MOC、UIC 和 RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    mylabel.cpp
    settings.cpp
)

set(HEADERS
    mainwindow.h
    mylabel.h
    settings.h
)

set(UI_FILES
    mainwindow.ui
    settings.ui
)

set(RESOURCE_FILES
    img.qrc
)

# 不再需要手动调用 qt6_wrap_ui 和 qt6_add_resources

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
    settingsStore.cpp
    settingsStore.h


)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
