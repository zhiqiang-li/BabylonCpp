#include <babylon/samples/optimizations/clip_planes_scene.h>

#include <babylon/babylon_stl_util.h>
#include <babylon/cameras/arc_rotate_camera.h>
#include <babylon/engine/scene.h>
#include <babylon/interfaces/icanvas.h>
#include <babylon/lights/hemispheric_light.h>
#include <babylon/lights/point_light.h>
#include <babylon/loading/scene_loader.h>
#include <babylon/materials/standard_material.h>
#include <babylon/mesh/abstract_mesh.h>

namespace BABYLON {
namespace Samples {

ClipPlanesScene::ClipPlanesScene(ICanvas* iCanvas)
    : IRenderableScene(iCanvas)
    , _camera{nullptr}
    , verticalSlider{-35.f, 30.f, 0.f}
    , horizontalSlider{-30.f, 20.f, -30.f}
{
}

ClipPlanesScene::~ClipPlanesScene()
{
}

const char* ClipPlanesScene::getName()
{
  return "Clip Planes Scene";
}

void ClipPlanesScene::initializeScene(ICanvas* canvas, Scene* scene)
{
  auto hemi       = HemisphericLight::New("hemi", Vector3::Up(), scene);
  hemi->intensity = 0.98f;

  // Adding a light
  _light = PointLight::New("Omni", Vector3(20.f, 20.f, 100.f), scene);
  _light->intensity = 0.5f;

  // Adding an Arc Rotate Camera
  _camera
    = ArcRotateCamera::New("Camera", 0.f, 0.8f, 100.f, Vector3::Zero(), scene);
  _camera->attachControl(canvas, false);

  // The first parameter can be used to specify which mesh to import. Here we
  // import all meshes
  SceneLoader::ImportMesh(
    {}, "scenes/", "skull.babylon", scene,
    [this](const std::vector<AbstractMeshPtr>& newMeshes,
           const std::vector<IParticleSystemPtr>& /*newParticleSystems*/,
           const std::vector<SkeletonPtr>& /*newSkeletons*/,
           const std::vector<AnimationGroupPtr>& /*newAnimationGroups*/) {
      // Set the target of the camera to the first imported mesh
      _camera->target = newMeshes[0]->position();
      std::dynamic_pointer_cast<StandardMaterial>(_scene->defaultMaterial())
        ->backFaceCulling
        = false;

      _scene->clipPlane4 = Plane(0.f, 1.f, 0.f, 0.f);
      _scene->clipPlane3 = Plane(1.f, 0.f, 0.f, -30.f);
    });

  // Move the light with the camera
  scene->registerBeforeRender([this](Scene* /*scene*/, EventState& /*es*/) {
    _light->position = _camera->position;
  });

  // Events
  canvas->addKeyEventListener(EventType::KEY_DOWN, [this](KeyboardEvent&& evt) {
    // "Numpad4"
    if (evt.keyCode == 324) {
      auto newValue
        = std::min(horizontalSlider.maximum, horizontalSlider.value + 1.f);
      if (!stl_util::almost_equal(newValue, horizontalSlider.value)) {
        _scene->clipPlane3 = Plane(1.f, 0.f, 0.f, newValue);
      }
      horizontalSlider.value = newValue;
    }
    // "Numpad6"
    else if (evt.keyCode == 326) {
      auto newValue
        = std::max(horizontalSlider.minimum, horizontalSlider.value - 1.f);
      if (!stl_util::almost_equal(newValue, horizontalSlider.value)) {
        _scene->clipPlane3 = Plane(1.f, 0.f, 0.f, newValue);
      }
      horizontalSlider.value = newValue;
    }
    // "Numpad2"
    else if (evt.keyCode == 322) {
      auto newValue
        = std::min(verticalSlider.maximum, verticalSlider.value + 1.f);
      if (!stl_util::almost_equal(newValue, verticalSlider.value)) {
        _scene->clipPlane4 = Plane(0.f, 1.f, 0.f, newValue);
      }
      verticalSlider.value = newValue;
    }
    // "Numpad8"
    else if (evt.keyCode == 328) {
      auto newValue
        = std::max(verticalSlider.minimum, verticalSlider.value - 1.f);
      if (!stl_util::almost_equal(newValue, verticalSlider.value)) {
        _scene->clipPlane4 = Plane(0.f, 1.f, 0.f, newValue);
      }
      verticalSlider.value = newValue;
    }
  });
}

} // end of namespace Samples
} // end of namespace BABYLON