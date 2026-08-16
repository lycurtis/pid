pure C++, no ROS. The algorithm.

building
```bash
source /opt/ros/humble/setup.bash
colcon build --symlink-install --packages-select pid_core
source install/setup.bash
colcon test --packages-select pid_core && colcon test-result --verbose
```
After a test failure, the useful output is usually
```bash
colcon test-result --verbose
```
or the gtest binary directly
```
./build/pid_core/test_pid_controller
```
