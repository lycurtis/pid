ROS2 nodes wrapping pid_core

## Node Roles
- setpoint_node: Says "go here" (`/setpoint`)
- pid_node: Compares setpoint vs state, computes effort
- plant_node: Responds to effort, reports state

## Commands

### Running
ros2 run <package_name> <executable_name>

```bash
source install/setup.bash
ros2 run pid_ros plant_node
```
