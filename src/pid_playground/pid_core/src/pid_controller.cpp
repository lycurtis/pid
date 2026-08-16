#include "pid_core/pid_controller.hpp"

namespace pid_core {

PidController::PidController(double kp, double ki, double kd)
: kp_(kp), ki_(ki), kd_(kd)
{
}

double PidController::update(double setpoint, double measurement, double dt)
{
  // Guard against dividing by 0 or invalid time.
  if(dt <= 0.00){
    return 0.0;
  }
  

  double error = setpoint - measurement; // e(t)
  
  double pTerm = kp_ * error;

  integral_ += error * dt;
  double iTerm = ki_ * integral_;

  double dTerm;
  if(sampleCount_ == 0 || dt <= 0){
    dTerm = 0;
  }
  else{
    double derivative = (error - prevError_) / dt;
    dTerm = kd_ * derivative;
  }

  double uOutput = pTerm + iTerm + dTerm; // u(t) or control output

  // Clamping output u(t) to match physical hardware limits + Antiwindup
  if(uOutput > maxOutput){
    uOutput = maxOutput;
    integral -= error * dt; // prevent integral accumulation during saturation
  }
  else if(uOutput < minOutput){
    uOutput = minOutput;
    itnegral -= error * dt; // prevent integral accumulation during saturation
  }

  // updating state variables for next update() loop
  prevError_ = error;
  sampleCount_++;

  return uOutput;
}

void PidController::reset()
{
  integral_ = 0;
  prevError_ = 0;
  sampleCount_ = 0;
}

void PidController::set_gains(double kp, double ki, double kd)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

}  // namespace pid_core
