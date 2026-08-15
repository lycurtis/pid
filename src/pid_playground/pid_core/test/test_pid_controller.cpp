#include <gtest/gtest.h>

#include "pid_core/pid_controller.hpp"

// Deliberately failing. Replace FAIL() with a real assertion once update()
// does something — start with P-only (ki = 0, kd = 0) and a non-zero error.
TEST(PidController, ScaffoldingRed)
{
  FAIL() << "Build/test loop works. Now write a real assertion and make it pass.";
}
