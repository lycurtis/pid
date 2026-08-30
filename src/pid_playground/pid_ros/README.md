ROS2 nodes wrapping pid_core

## Node Roles
- setpoint_node: Says "go here" (`/setpoint`)
- pid_node: Compares setpoint vs state, computes effort
- plant_node: Responds to effort, reports state

## Commands

### Building

```bash
colcon build --symlink-install --packages-select pid_ros --allow-overriding pid_core
source install/setup.bash
```

### Running
ros2 run <package_name> <executable_name>

Terminal 1 (Run the physical system)
```bash
source install/setup.bash
ros2 run pid_ros plant_node
```
Terminal 2 (Publishes the desired setpoint value)
```bash
source install/setup.bash
ros2 run pid_ros setpoint_node
```
Terminal 3 (Run the PID algorithm)
```bash
source install/setup.bash
ros2 run pid_ros pid_node
```
Terminal 4 (Watch the measured state of the system)
```bash
source install/setup.bash
ros2 topic echo /state
```
