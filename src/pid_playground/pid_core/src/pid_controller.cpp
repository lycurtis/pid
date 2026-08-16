#include "pid_core/pid_controller.hpp"

namespace pid_core {

PidController::PidController(double kp, double ki, double kd)
: kp_(kp), ki_(ki), kd_(kd)
{
}

double PidController::update(double setpoint, double measurement, double dt)
{
  double error = setpoint - measurement; // e(t)
  
  double p_term = kp_ * error;

  integral_ += error * dt;
  double i_term = ki_ * integral_;

  double d_term;
  if(sample_count_ == 0 || dt <= 0){
    d_term = 0;
  }
  else{
    double derivative = (error - prev_error_) / dt;
    d_term = kd_ * derivative;
  }

  double u = p_term + i_term + d_term; // u(t) or control output

  // updating state variables
  prev_error_ = error;
  sample_count_++;

  return u;
}

void PidController::reset()
{
  integral_ = 0;
  prev_error_ = 0;
  sample_count_ = 0;
}

void PidController::set_gains(double kp, double ki, double kd)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

}  // namespace pid_core
