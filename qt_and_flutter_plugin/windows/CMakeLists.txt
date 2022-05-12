cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "qt_and_flutter_plugin")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "qt_and_flutter_plugin_plugin")

add_library(${PLUGIN_NAME} SHARED
  "qt_and_flutter_plugin.cpp"
  "qmfcapp.cpp"
  "qmfcapp.h"
)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#查找需要的Qt库文件，最好每一个库都要写，Qt也会根据依赖关系自动添加
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin Qt5::Widgets Qt5::Core Qt5::Gui)

# List of absolute paths to libraries that should be bundled with the plugin
set(qt_and_flutter_plugin_bundled_libraries
  ""
  PARENT_SCOPE
)
