#include "pid_core/pid_controller.hpp"
#include <stdlib.h>
#include <iostream>
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
    dTerm = kd_ * derivative; // FUTURE TODO: apply a first-order low-pass filter (filtered D)
    /*
    For now keep raw D only as a temporary learning form
    When you add plant noise later, write a test or a PlotJuggler run that compares unfiltered 
    vs filtered D on the same noisy /state. That will teach you more than any formula dump.
     */
  }

  double uOutput = pTerm + iTerm + dTerm; // u(t) or control output

  // Clamping output u(t) to match physical hardware limits + Antiwindup
  if(uOutput > maxOutput_ && error > 0){ // Case A: Max Output but still below the target
    uOutput = maxOutput_;
    integral_ -= error * dt; // undo the integral (aka DO NOT add to the integral )
  }
  else if (uOutput > maxOutput_ && error < 0){ // Case B: Max Output but above the target (overshoot) (solution: clamp only)
    uOutput = maxOutput_;
    // We do want to keep the integratation for this step since error < 0 this will make the integration term smaller
  }
  else if(uOutput < minOutput_ && error > 0){ // Case C: Min Output but below the target (solution: clamp only)
    uOutput = minOutput_; 
    // We do want to keep the integration for this step since error > 0 this will make the integration term bigger
  }
  else if(uOutput < minOutput_ && error < 0){ // Case D: Min output but above the target
    uOutput = minOutput_;
    integral_ -= error * dt; // undo the integral (aka DO NOT add to the integral )
  }

  // updating state variables for next update() loop
  prevError_ = error;
  sampleCount_++;

  return uOutput;
}

void PidController::reset(){
  integral_ = 0;
  prevError_ = 0;
  sampleCount_ = 0;
}

void PidController::set_gains(double kp, double ki, double kd){
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

void PidController::set_output_limits(double minOutput, double maxOutput){
  minOutput_ = minOutput;
  maxOutput_ = maxOutput;
}

}  // namespace pid_core
