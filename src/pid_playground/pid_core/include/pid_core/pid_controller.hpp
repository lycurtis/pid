#pragma once

namespace pid_core {

// Pure C++ PID

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

  // Controller state variable
  double integral_ = 0;
  double prev_error_ = 0;
  int sample_count_ = 0;

  // Persistent controller state lives here. You decide what that is.
  // Hint: anything you need from a *previous* call to update() is state.
};

}  // namespace pid_core
