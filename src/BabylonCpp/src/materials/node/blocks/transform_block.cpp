#include <babylon/materials/node/blocks/transform_block.h>

#include <babylon/core/json_util.h>
#include <babylon/materials/node/blocks/input/input_block.h>
#include <babylon/materials/node/node_material_build_state.h>
#include <babylon/materials/node/node_material_connection_point.h>
#include <babylon/misc/string_tools.h>

namespace BABYLON {

TransformBlock::TransformBlock(const std::string& iName)
    : NodeMaterialBlock{iName, NodeMaterialBlockTargets::Neutral}
    , complementW{1.f}
    , complementZ{0.f}
    , vector{this, &TransformBlock::get_vector}
    , output{this, &TransformBlock::get_output}
    , transform{this, &TransformBlock::get_transform}
{
  registerInput("vector", NodeMaterialBlockConnectionPointTypes::AutoDetect);
  registerInput("transform", NodeMaterialBlockConnectionPointTypes::Matrix);
  registerOutput("output", NodeMaterialBlockConnectionPointTypes::Vector4);

  _inputs[0]->onConnectionObservable.add(
    [this](NodeMaterialConnectionPoint* other, EventState & /*es*/) -> void {
      if (other && other->ownerBlock()->isInput()) {
        const auto otherAsInput = std::static_pointer_cast<InputBlock>(other->ownerBlock());

        if (otherAsInput->name == "normal") {
          complementW = 0.f;
        }
      }
    });
}

TransformBlock::~TransformBlock() = default;

std::string TransformBlock::getClassName() const
{
  return "TransformBlock";
}

NodeMaterialConnectionPointPtr& TransformBlock::get_vector()
{
  return _inputs[0];
}

NodeMaterialConnectionPointPtr& TransformBlock::get_output()
{
  return _outputs[0];
}

NodeMaterialConnectionPointPtr& TransformBlock::get_transform()
{
  return _inputs[1];
}

TransformBlock& TransformBlock::_buildBlock(NodeMaterialBuildState& state)
{
  NodeMaterialBlock::_buildBlock(state);

  const auto& iOutput = _outputs[0];

  switch (vector()->connectedPoint()->type()) {
    case NodeMaterialBlockConnectionPointTypes::Vector2:
      state.compilationString
        += _declareOutput(iOutput, state)
           + StringTools::printf(
             " = %s * vec4(%s, %s, %s);\r\n", transform()->associatedVariableName().c_str(),
             vector()->associatedVariableName().c_str(), _writeFloat(complementZ).c_str(),
             _writeFloat(complementW).c_str());
      break;
    case NodeMaterialBlockConnectionPointTypes::Vector3:
    case NodeMaterialBlockConnectionPointTypes::Color3:
      state.compilationString
        += _declareOutput(iOutput, state)
           + StringTools::printf(
             " = %s * vec4(%s, %s);\r\n", transform()->associatedVariableName().c_str(),
             vector()->associatedVariableName().c_str(), _writeFloat(complementW).c_str());
      break;
    default:
      state.compilationString
        += _declareOutput(iOutput, state)
           + StringTools::printf(" = %s * %s;\r\n", transform()->associatedVariableName().c_str(),
                                 vector()->associatedVariableName().c_str());
      break;
  }

  return *this;
}

json TransformBlock::serialize() const
{
  return nullptr;
}

void TransformBlock::_deserialize(const json& /*serializationObject*/, Scene* /*scene*/,
                                  const std::string& /*rootUrl*/)
{
}

std::string TransformBlock::_dumpPropertiesCode()
{
  auto codeString
    = StringTools::printf("%s.complementZ = %f;\r\n", _codeVariableName.c_str(), complementZ);

  codeString
    += StringTools::printf("%s.complementW = %f;\r\n", _codeVariableName.c_str(), complementW);

  return codeString;
}

} // end of namespace BABYLON
