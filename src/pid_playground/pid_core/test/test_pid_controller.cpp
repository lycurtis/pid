#include <gtest/gtest.h>

#include "pid_core/pid_controller.hpp"

// Deliberately failing. Replace FAIL() with a real assertion once update()
// does something — start with P-only (ki = 0, kd = 0) and a non-zero error.
/*
TEST(TestSuiteName, TestName) {
  ... test body ...
}
*/
TEST(PidController, TestPGain)
{
  double kp = 2.0;
  double ki = 0.0;
  double kd = 0.0;
  pid_core::PidController pid(kp, ki, kd);
  EXPECT_EQ(pid.update(3.0, 1.0, 1.0), 4.0);
}

TEST(PidController, TestIGain)
{
  double kp = 0.0;
  double ki = 2.0;
  double kd = 0.0;
  pid_core::PidController pid(kp, ki, kd);
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 0.04); // integral is 2 ==> ki*integral = 2*2 = 4
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 0.08); // integral is 4 ==> ki*integral = 2*4 = 8
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 0.12); // integral is 6 ==> ki*integral = 2*6 = 12
}

TEST(PidController, TestDGain)
{
  double kp = 0.0;
  double ki = 0.0;
  double kd = 2.0;
  pid_core::PidController pid(kp, ki, kd);
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 0.0); // first sample (d_term = 0)
  EXPECT_EQ(pid.update(5.0, 1.0, 0.01), 400.0); // 2nd sample (d_term = 4)
}

TEST(PidController, TestDTEQZero) // Call 1: dt == 0 or is tiny (Divide by zero case)
{
  double kp = 0.0;
  double ki = 0.0;
  double kd = 2.0;
  pid_core::PidController pid(kp, ki, kd);
  

  EXPECT_EQ(pid.update(3.0, 1.0, 0.0), 0.0);
  EXPECT_EQ(pid.update(5.0, 1.0, 0.0), 0.0);
}

TEST(PidController, TestNoPrevSample) // Call 2: no previous sample or is the first call
{
  double kp = 2.0;
  double ki = 0.0;
  double kd = 2.0;
  pid_core::PidController pid(kp, ki, kd);
  
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 4.0);
}

TEST(PidController, TestReset)
{
  double kp = 2.0;
  double ki = 2.0;
  double kd = 2.0;
  pid_core::PidController pid(kp, ki, kd);
  
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 4.04);
  pid.reset();
  EXPECT_EQ(pid.update(3.0, 1.0, 0.01), 4.04);
}