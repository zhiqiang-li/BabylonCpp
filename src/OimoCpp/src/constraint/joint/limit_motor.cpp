#include <oimo/constraint/joint/limit_motor.h>

namespace OIMO {

LimitMotor::LimitMotor(const Vec3& _axis, bool fixed)
    : axis{_axis}
    , angle{0.f}
    , lowerLimit{fixed ? 0.f : 1.f}
    , upperLimit{0.f}
    , motorSpeed{0.f}
    , maxMotorForce{0.f}
    , frequency{0.f}
    , dampingRatio{0.f}
{
}

LimitMotor::LimitMotor(const LimitMotor& lm)
    : axis{lm.axis}
    , angle{lm.angle}
    , lowerLimit{lm.lowerLimit}
    , upperLimit{lm.upperLimit}
    , motorSpeed{lm.motorSpeed}
    , maxMotorForce{lm.maxMotorForce}
    , frequency{lm.frequency}
    , dampingRatio{lm.dampingRatio}
{
}

LimitMotor::LimitMotor(LimitMotor&& lm)
{
  *this = std::move(lm);
}

LimitMotor::~LimitMotor()
{
}

LimitMotor& LimitMotor::operator=(const LimitMotor& lm)
{
  if (&lm != this) {
    axis          = lm.axis;
    angle         = lm.angle;
    lowerLimit    = lm.lowerLimit;
    upperLimit    = lm.upperLimit;
    motorSpeed    = lm.motorSpeed;
    maxMotorForce = lm.maxMotorForce;
    frequency     = lm.frequency;
    dampingRatio  = lm.dampingRatio;
  }

  return *this;
}

LimitMotor& LimitMotor::operator=(LimitMotor&& lm)
{
  if (&lm != this) {
    std::swap(axis, lm.axis);
    std::swap(angle, lm.angle);
    std::swap(lowerLimit, lm.lowerLimit);
    std::swap(upperLimit, lm.upperLimit);
    std::swap(motorSpeed, lm.motorSpeed);
    std::swap(maxMotorForce, lm.maxMotorForce);
    std::swap(frequency, lm.frequency);
    std::swap(dampingRatio, lm.dampingRatio);
  }

  return *this;
}

LimitMotor LimitMotor::clone() const
{
  return LimitMotor(*this);
}

LimitMotor* LimitMotor::cloneToNewObject() const
{
  return new LimitMotor(*this);
}

void LimitMotor::setLimit(float _lowerLimit, float _upperLimit)
{
  lowerLimit = _lowerLimit;
  upperLimit = _upperLimit;
}

void LimitMotor::setMotor(float _motorSpeed, float _maxMotorForce)
{
  motorSpeed    = _motorSpeed;
  maxMotorForce = _maxMotorForce;
}

void LimitMotor::setSpring(float _frequency, float _dampingRatio)
{
  frequency    = _frequency;
  dampingRatio = _dampingRatio;
}

} // end of namespace OIMO
