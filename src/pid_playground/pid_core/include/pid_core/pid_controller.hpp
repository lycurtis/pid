#pragma once

namespace pid_core {

// Pure C++ PID. No ROS types in this header — that's the whole point of
// splitting pid_core from pid_ros.
//
// You own the math. This is just the shape of the class so the library
// compiles and tests can link against it.
class PidController {
public:
  PidController(double kp, double ki, double kd);

  // setpoint, measurement: same units as each other.
  // dt: seconds, computed by the *caller* from actual clock deltas.
  // Returns the control effort for this step.
  double update(double setpoint, double measurement, double dt);

  // Clear whatever internal state you accumulate (integral, last sample, …).
  void reset();

  // Live retuning. pid_ros will call this from a parameter callback later.
  void set_gains(double kp, double ki, double kd);

private:
  double kp_;
  double ki_;
  double kd_;

  // Persistent controller state lives here. You decide what that is.
  // Hint: anything you need from a *previous* call to update() is state.
};

}  // namespace pid_core
