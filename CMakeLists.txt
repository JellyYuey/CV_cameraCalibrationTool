cmake_minimum_required(VERSION 3.14)

project(CameraCalibration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用自动 MOC、UIC 和 RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 查找 OpenCV
find_package(OpenCV REQUIRED)

# 打印 OpenCV 信息以进行调试
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

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

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# 链接 Qt6 和 OpenCV 库
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${OpenCV_LIBS})

# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OpenCV_INCLUDE_DIRS}
)