#include <babylon/particles/points_group.h>

namespace BABYLON {

PointsGroup::PointsGroup(size_t id,
                         const std::function<void(CloudPoint* particle, int i, int s)>& posFunction)
{
  groupID           = id;
  _positionFunction = posFunction;
}

PointsGroup::~PointsGroup() = default;

} // end of namespace BABYLON
