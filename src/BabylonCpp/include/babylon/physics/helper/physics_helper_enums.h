#ifndef BABYLON_PHYSICS_HELPER_PHYSICS_HELPER_ENUMS_H
#define BABYLON_PHYSICS_HELPER_PHYSICS_HELPER_ENUMS_H

namespace BABYLON {

/**
 * @brief The strenght of the force in correspondence to the distance of the
 * affected object.
 * @see
 * https://doc.babylonjs.com/how_to/using_the_physics_engine#further-functionality-of-the-impostor-class
 */
enum class PhysicsRadialImpulseFalloff {
  /**
   * Defines that impulse is constant in strength across it's whole radius
   */
  Constant = 0,
  /**
   * Defines that impulse gets weaker if it's further from the origin
   */
  Linear = 1
}; // end of enum class PhysicsRadialImpulseFalloff

/**
 * @brief The strength of the force in correspondence to the distance of the
 * affected object.
 * @see
 * https://doc.babylonjs.com/how_to/using_the_physics_engine#further-functionality-of-the-impostor-class
 */
enum class PhysicsUpdraftMode {
  /**
   * Defines that the upstream forces will pull towards the top center of the //
   * cylinder
   */
  Center = 0,
  /**
   * Defines that once a impostor is inside the cylinder, it will shoot out
   * perpendicular from the ground of the cylinder
   */
  Perpendicular = 1
}; // end of enum class PhysicsUpdraftMode

} // end of namespace BABYLON

#endif // end of BABYLON_PHYSICS_HELPER_PHYSICS_HELPER_ENUMS_H
