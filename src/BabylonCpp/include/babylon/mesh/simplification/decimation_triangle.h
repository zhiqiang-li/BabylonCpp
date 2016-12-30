#ifndef BABYLON_MESH_SIMPLIFICATION_DECIMATION_TRIANGLE_H
#define BABYLON_MESH_SIMPLIFICATION_DECIMATION_TRIANGLE_H

#include <babylon/babylon_global.h>

#include <babylon/math/vector3.h>
#include <babylon/mesh/simplification/decimation_vertex.h>

namespace BABYLON {

/**
 * @brief
 */
class BABYLON_SHARED_EXPORT DecimationTriangle {

public:
  DecimationTriangle(const std::vector<DecimationVertex>& vertices);
  ~DecimationTriangle();

public:
  Vector3 normal;
  std::array<int, 4> error;
  bool deleted;
  bool isDirty;
  float borderFactor;
  bool deletePending;
  float originalOffset;
  std::vector<DecimationVertex> vertices;

}; // end of class DecimationTriangle

} // end of namespace BABYLON

#endif // end of BABYLON_MESH_SIMPLIFICATION_DECIMATION_TRIANGLE_H
