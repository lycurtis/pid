#include "pid_core/pid_controller.hpp"

namespace pid_core {

PidController::PidController(double kp, double ki, double kd)
: kp_(kp), ki_(ki), kd_(kd)
{
}

double PidController::update([[maybe_unused]] double setpoint,
                             [[maybe_unused]] double measurement,
                             [[maybe_unused]] double dt)
{
  // Your job. Returning 0 keeps the library compiling.
  return 0.0;
}

void PidController::reset()
{
}

void PidController::set_gains(double kp, double ki, double kd)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

}  // namespace pid_core
