cmake_minimum_required(VERSION 3.16)
project(pid_core)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

add_library(pid_core src/pid_controller.cpp)
target_compile_features(pid_core PUBLIC cxx_std_17)
target_include_directories(pid_core PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>")

install(DIRECTORY include/ DESTINATION include)
install(TARGETS pid_core
  EXPORT export_pid_core
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)

ament_export_targets(export_pid_core HAS_LIBRARY_TARGET)
ament_export_include_directories(include)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_pid_controller test/test_pid_controller.cpp)
  target_link_libraries(test_pid_controller pid_core)
endif()

ament_package()
