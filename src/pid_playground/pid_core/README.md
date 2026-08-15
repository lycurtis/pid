pure C++, no ROS. The algorithm.

building
```bash
colcon build --symlink-install -packages-select pid_core
source install/setup.bash
colcon test --packages-select pid_core && colcon test-result --verbose
