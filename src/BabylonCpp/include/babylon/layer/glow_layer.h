#ifndef BABYLON_LAYER_GLOW_LAYER_H
#define BABYLON_LAYER_GLOW_LAYER_H

#include <babylon/babylon_global.h>
#include <babylon/layer/effect_layer.h>
#include <babylon/layer/iglow_layer_options.h>

namespace BABYLON {

/**
 * @brief The glow layer Helps adding a glow effect around the emissive parts of
 * a mesh.
 *
 * Once instantiated in a scene, simply use the pushMesh or removeMesh method to
 * add or remove glowy meshes to your scene.
 *
 * Documentation: https://doc.babylonjs.com/how_to/glow_layer
 */
class BABYLON_SHARED_EXPORT GlowLayer : public EffectLayer {

public:
  /**
   * Effect Name of the layer.
   */
  static constexpr const char* EffectName = "GlowLayer";

  /**
   * The default blur kernel size used for the glow.
   */
  static constexpr unsigned int DefaultBlurKernelSize = 32;

  /**
   * The default texture size ratio used for the glow.
   */
  static constexpr float DefaultTextureRatio = 0.5f;

public:
  template <typename... Ts>
  static GlowLayer* New(Ts&&... args)
  {
    auto glowLayer = new GlowLayer(::std::forward<Ts>(args)...);
    glowLayer->addToScene(static_cast<unique_ptr_t<EffectLayer>>(glowLayer));

    return glowLayer;
  }
  ~GlowLayer() override;

  /**
   * @brief Sets the kernel size of the blur.
   */
  void setBlurKernelSize(float value);

  /**
   * @brief Gets the kernel size of the blur.
   */
  float blurKernelSize() const;

  /**
   * @brief Sets the glow intensity.
   */
  void setIntensity(float value);

  /**
   * @brief Gets the glow intensity.
   */
  float intensity() const;

  /**
   * @brief Get the effect name of the layer.
   * @return The effect name
   */
  string_t getEffectName() const override;

  /**
   * @brief Checks for the readiness of the element composing the layer.
   * @param subMesh the mesh to check for
   * @param useInstances specify wether or not to use instances to render the
   * mesh
   * @param emissiveTexture the associated emissive texture used to generate the
   * glow
   * @return true if ready otherwise, false
   */
  bool isReady(SubMesh* subMesh, bool useInstances) override;

  /**
   * @brief Returns wether or nood the layer needs stencil enabled during the
   * mesh rendering.
   */
  bool needStencil() const override;

  /**
   * @brief Add a mesh in the exclusion list to prevent it to impact or being
   * impacted by the glow layer.
   * @param mesh The mesh to exclude from the glow layer
   */
  void addExcludedMesh(Mesh* mesh);

  /**
   * @brief Remove a mesh from the exclusion list to let it impact or being
   * impacted by the glow layer.
   * @param mesh The mesh to remove
   */
  void removeExcludedMesh(Mesh* mesh);

  /**
   * @brief Add a mesh in the inclusion list to impact or being impacted by the
   * glow layer.
   * @param mesh The mesh to include in the glow layer
   */
  void addIncludedOnlyMesh(Mesh* mesh);

  /**
   * @brief Remove a mesh from the Inclusion list to prevent it to impact or
   * being impacted by the glow layer.
   * @param mesh The mesh to remove
   */
  void removeIncludedOnlyMesh(Mesh* mesh);

  /**
   * @brief Determine if a given mesh will be used in the glow layer
   * @param mesh The mesh to test
   * @returns true if the mesh will be highlighted by the current glow layer
   */
  bool hasMesh(AbstractMesh* mesh) const override;

  /**
   * @brief Free any resources and references associated to a mesh.
   * Internal use
   * @param mesh The mesh to free.
   */
  void _disposeMesh(Mesh* mesh) override;

protected:
  /**
   * @brief Instantiates a new glow Layer and references it to the scene.
   * @param name The name of the layer
   * @param scene The scene to use the layer in
   * @param options Sets of none mandatory options to use with the layer (see
   * IGlowLayerOptions for more information)
   */
  GlowLayer(const string_t& name, Scene* scene);
  GlowLayer(const string_t& name, Scene* scene,
            const IGlowLayerOptions& options);

  /**
   * @brief Create the merge effect. This is the shader use to blit the
   * information back to the main canvas at the end of the scene rendering.
   */
  Effect* _createMergeEffect() override;

  /**
   * @brief Creates the render target textures and post processes used in the
   * glow layer.
   */
  void _createTextureAndPostProcesses() override;

  /**
   * @brief Implementation specific of rendering the generating effect on the
   * main canvas.
   * @param effect The effect used to render through
   */
  void _internalRender(Effect* effect) override;

  /**
   * @brief Sets the required values for both the emissive texture and and the
   * main color.
   */
  void _setEmissiveTextureAndColor(Mesh* mesh, SubMesh* subMesh,
                                   Material* material) override;

  /**
   * @brief Returns true if the mesh should render, otherwise false.
   * @param mesh The mesh to render
   * @returns true if it should render otherwise false
   */
  bool _shouldRenderMesh(Mesh* mesh) const override;

private:
  IGlowLayerOptions _options;
  float _intensity;
  unique_ptr_t<BlurPostProcess> _horizontalBlurPostprocess1;
  unique_ptr_t<BlurPostProcess> _verticalBlurPostprocess1;
  unique_ptr_t<BlurPostProcess> _horizontalBlurPostprocess2;
  unique_ptr_t<BlurPostProcess> _verticalBlurPostprocess2;
  unique_ptr_t<RenderTargetTexture> _blurTexture1;
  unique_ptr_t<RenderTargetTexture> _blurTexture2;
  vector_t<PostProcess*> _postProcesses1;
  vector_t<PostProcess*> _postProcesses2;

  Uint64Array _includedOnlyMeshes;
  Uint64Array _excludedMeshes;

}; // end of struct GlowLayer

} // end of namespace BABYLON

#endif // end of BABYLON_LAYER_GLOW_LAYER_H