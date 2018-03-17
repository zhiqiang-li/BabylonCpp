#include <babylon/mesh/primitivegeometries/ribbon_geometry.h>

#include <babylon/mesh/vertex_data.h>
#include <babylon/mesh/vertex_data_options.h>

namespace BABYLON {

RibbonGeometry::RibbonGeometry(const string_t& _id, Scene* scene,
                               const vector_t<vector_t<Vector3>>& _pathArray,
                               bool _closeArray, bool _closePath, int _offset,
                               bool canBeRegenerated, Mesh* mesh,
                               unsigned int _side)
    : _PrimitiveGeometry{_id, scene, canBeRegenerated, mesh}
    , pathArray{_pathArray}
    , closeArray{_closeArray}
    , closePath{_closePath}
    , offset{_offset}
    , side{_side}
{
}

RibbonGeometry::~RibbonGeometry()
{
}

unique_ptr_t<VertexData> RibbonGeometry::_regenerateVertexData()
{
  RibbonOptions options(pathArray, offset);
  options.closeArray      = closeArray;
  options.closePath       = closePath;
  options.sideOrientation = side;

  return VertexData::CreateRibbon(options);
}

Geometry* RibbonGeometry::copy(const string_t& _id)
{
  return RibbonGeometry::New(_id, getScene(), pathArray, closeArray, closePath,
                             offset, canBeRegenerated(), nullptr, side);
}

} // end of namespace BABYLON
