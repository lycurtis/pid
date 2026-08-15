cmake_minimum_required(VERSION 3.16)
project(pid_ros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(pid_core REQUIRED)

add_executable(pid_node src/pid_node.cpp)
target_include_directories(pid_node PRIVATE include)
target_compile_features(pid_node PUBLIC cxx_std_17)
ament_target_dependencies(pid_node rclcpp std_msgs pid_core)

add_executable(plant_node src/plant_node.cpp)
target_include_directories(plant_node PRIVATE include)
target_compile_features(plant_node PUBLIC cxx_std_17)
ament_target_dependencies(plant_node rclcpp std_msgs)

add_executable(setpoint_node src/setpoint_node.cpp)
target_include_directories(setpoint_node PRIVATE include)
target_compile_features(setpoint_node PUBLIC cxx_std_17)
ament_target_dependencies(setpoint_node rclcpp std_msgs)

install(TARGETS
  pid_node
  plant_node
  setpoint_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
