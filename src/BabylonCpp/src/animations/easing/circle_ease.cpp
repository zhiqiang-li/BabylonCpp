#include <babylon/animations/easing/circle_ease.h>

namespace BABYLON {

CircleEase::CircleEase() : EasingFunction{}
{
}

CircleEase::~CircleEase()
{
}

float CircleEase::easeInCore(float gradient) const
{
  float _gradient = std::fmax(0.f, std::fmin(1.f, gradient));
  return (1.f - std::sqrt(1.f - (_gradient * _gradient)));
}

} // end of namespace BABYLON
