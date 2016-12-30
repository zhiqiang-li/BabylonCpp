#ifndef BABYLON_ANIMATIONS_EASING_EASING_FUNCTION_H
#define BABYLON_ANIMATIONS_EASING_EASING_FUNCTION_H

#include <babylon/babylon_global.h>

#include <babylon/animations/easing/ieasing_function.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT EasingFunction : public IEasingFunction {

public:
  // Statics
  static constexpr int EASINGMODE_EASEIN    = 0;
  static constexpr int EASINGMODE_EASEOUT   = 1;
  static constexpr int EASINGMODE_EASEINOUT = 2;

public:
  EasingFunction();
  virtual ~EasingFunction();

  /** Properties **/
  void setEasingMode(int easingMode);
  int getEasingMode() const;
  virtual float easeInCore(float gradient) const = 0;
  float ease(float gradient) override;

private:
  int _easingMode;

}; // end of class EasingFunction

} // end of namespace BABYLON

#endif // end of BABYLON_ANIMATIONS_EASING_EASING_FUNCTION_H
