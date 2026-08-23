#pragma once

namespace pid_core {

// Pure C++ PID

class PidController {
public:
  // Physical system contraints

  PidController(double kp, double ki, double kd);

  // setpoint, measurement: same units as each other.
  // dt: seconds, computed by the *caller* from actual clock deltas.
  // Returns the control effort for this step.
  double update(double setpoint, double measurement, double dt);

  // Clear whatever internal state you accumulate (integral, last sample, …).
  void reset();

  // Live retuning. pid_ros will call this from a parameter callback later.
  void set_gains(double kp, double ki, double kd);

  // optional setter for setting hardware limits of the system
  void set_output_limits(double minOutput, double maxOutput);

private:
  // Tuning gains
  double kp_;
  double ki_;
  double kd_;

  // phsyical system constraints
  double minOutput_ = 0; // default
  double maxOutput_ = 9999; // default

  // Controller state variable
  double integral_ = 0;
  double prevError_ = 0;
  int sampleCount_ = 0;
};

}  // namespace pid_core
