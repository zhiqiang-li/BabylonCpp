#ifndef BABYLON_ENGINE_SCENE_H
#define BABYLON_ENGINE_SCENE_H

#include <babylon/animations/ianimatable.h>
#include <babylon/babylon_global.h>
#include <babylon/core/structs.h>
#include <babylon/culling/octrees/octree.h>
#include <babylon/engine/pointer_info.h>
#include <babylon/engine/pointer_info_pre.h>
#include <babylon/interfaces/idisposable.h>
#include <babylon/math/color3.h>
#include <babylon/math/matrix.h>
#include <babylon/math/plane.h>
#include <babylon/tools/observable.h>
#include <babylon/tools/observer.h>
#include <babylon/tools/perf_counter.h>

namespace BABYLON {

/**
 * @brief Represents a scene to be rendered by the engine.
 */
class BABYLON_SHARED_EXPORT Scene : public IAnimatable, public IDisposable {

public:
  // Statics
  static constexpr unsigned int FOGMODE_NONE   = 0;
  static constexpr unsigned int FOGMODE_EXP    = 1;
  static constexpr unsigned int FOGMODE_EXP2   = 2;
  static constexpr unsigned int FOGMODE_LINEAR = 3;

  static microseconds_t MinDeltaTime;
  static microseconds_t MaxDeltaTime;

  template <typename... Ts>
  static std::unique_ptr<Scene> New(Ts&&... args)
  {
    std::unique_ptr<Scene> scene(new Scene(std::forward<Ts>(args)...));
    return scene;
  }
  virtual ~Scene();

  IReflect::Type type() const override;

  // Events
  void setOnDispose(const std::function<void()>& callback);
  void setBeforeRender(const std::function<void()>& callback);
  void setAfterRender(const std::function<void()>& callback);
  void setBeforeCameraRender(const std::function<void()>& callback);
  void setAfterCameraRender(const std::function<void()>& callback);

  // Pointers
  Vector2 unTranslatedPointer() const;

  /** Properties **/
  Plane* clipPlane();
  void setClipPlane(const Plane& plane);
  void resetClipPlane();
  void setMirroredCameraPosition(const Vector3& newPosition);
  StandardMaterial* defaultMaterial();
  DebugLayer* debugLayer();
  void setWorkerCollisions(bool enabled);
  bool workerCollisions() const;
  Octree<AbstractMesh*>* SelectionOctree();
  /**
   * The mesh that is currently under the pointer.
   * @return {BABYLON.AbstractMesh} mesh under the pointer/mouse cursor or null
   * if none.
   */
  AbstractMesh* meshUnderPointer();
  /**
   * Current on-screen X position of the pointer
   * @return {number} X position of the pointer
   */
  int pointerX();
  /**
   * Current on-screen Y position of the pointer
   * @return {number} Y position of the pointer
   */
  int pointerY();
  Material* getCachedMaterial();
  BoundingBoxRenderer* getBoundingBoxRenderer();
  OutlineRenderer* getOutlineRenderer();
  Engine* getEngine();
  size_t getTotalVertices() const;
  PerfCounter& totalVerticesPerfCounter();
  size_t getActiveIndices() const;
  PerfCounter& totalActiveIndicesPerfCounter();
  size_t getActiveParticles() const;
  PerfCounter& activeParticlesPerfCounter();
  size_t getActiveBones() const;
  PerfCounter& activeBonesPerfCounter();
  /** Stats **/
  microsecond_t getLastFrameDuration() const;
  PerfCounter& lastFramePerfCounter();
  microsecond_t getEvaluateActiveMeshesDuration() const;
  PerfCounter& evaluateActiveMeshesDurationPerfCounter();
  std::vector<Mesh*>& getActiveMeshes();
  microsecond_t getRenderTargetsDuration() const;
  microsecond_t getRenderDuration() const;
  PerfCounter& renderDurationPerfCounter();
  microsecond_t getParticlesDuration() const;
  PerfCounter& particlesDurationPerfCounter();
  microsecond_t getSpritesDuration() const;
  PerfCounter& spriteDuractionPerfCounter();
  float getAnimationRatio() const;
  int getRenderId() const;
  void incrementRenderId();
  /** Pointers handling **/
  /**
   * Attach events to the canvas (To handle actionManagers triggers and raise
   * onPointerMove, onPointerDown and onPointerUp
   * @param attachUp defines if you want to attach events to pointerup
   * @param attachDown defines if you want to attach events to pointerdown
   * @param attachMove defines if you want to attach events to pointermove
   */
  void attachControl(bool attachUp = true, bool attachDown = true,
                     bool attachMove = true);
  void detachControl();
  /** Ready **/
  bool isReady();
  void resetCachedMaterial();
  void registerBeforeRender(const std::function<void()>& func);
  void unregisterBeforeRender(const std::function<void()>& func);
  void registerAfterRender(const std::function<void()>& func);
  void unregisterAfterRender(const std::function<void()>& func);
  void _addPendingData(Mesh* mesh);
  void _addPendingData(GL::IGLTexture* texure);
  void _removePendingData(GL::IGLTexture* texture);
  void getWaitingItemsCount();
  /**
   * Registers a function to be executed when the scene is ready.
   * @param {Function} func - the function to be executed.
   */
  void executeWhenReady(std::function<void()>& func);
  void _checkIsReady();
  /** Animations **/
  std::vector<Animation*> getAnimations() override;
  /**
   * Will start the animation sequence of a given target
   * @param target - the target
   * @param {number} from - from which frame should animation start
   * @param {number} to - till which frame should animation run.
   * @param {boolean} [loop] - should the animation loop
   * @param {number} [speedRatio] - the speed in which to run the animation
   * @param {Function} [onAnimationEnd] function to be executed when the
   * animation ended.
   * @param {BABYLON.Animatable} [animatable] an animatable object. If not
   * provided a new one will be created from the given params.
   * @return {BABYLON.Animatable} the animatable object created for this
   * animation
   * @see BABYLON.Animatable
   * @see http://doc.babylonjs.com/page.php?p=22081
   */
  Animatable* beginAnimation(IAnimatable* target, float from, float to,
                             bool loop = false, float speedRatio = 1.f,
                             const std::function<void()>& onAnimationEnd
                             = nullptr,
                             Animatable* animatable = nullptr);
  Animatable*
  beginDirectAnimation(IAnimatable* target,
                       const std::vector<Animation*>& animations, int from,
                       int to, bool loop = false, float speedRatio = 1.f,
                       const std::function<void()>& onAnimationEnd = nullptr);
  Animatable* getAnimatableByTarget(IAnimatable* target);
  std::vector<Animatable*>& animatables();
  /**
   * Will stop the animation of the given target
   * @param target - the target
   * @param animationName - the name of the animation to stop (all animations
   * will be stopped is empty)
   */
  void stopAnimation(IAnimatable* target,
                     const std::string& animationName = "");

  /** Matrix **/
  Matrix getViewMatrix();
  Matrix& getProjectionMatrix();
  const Matrix& getProjectionMatrix() const;
  Matrix getTransformMatrix();
  void setTransformMatrix(Matrix& view, Matrix& projection);
  /** Methods **/
  void addMesh(std::unique_ptr<AbstractMesh>&& newMesh);
  int removeMesh(AbstractMesh* toRemove);
  int removeSkeleton(Skeleton* toRemove);
  int removeLight(Light* toRemove);
  int removeCamera(Camera* toRemove);
  void addLight(std::unique_ptr<Light>&& newLight);
  void addCamera(std::unique_ptr<Camera>&& newCamera);
  /**
   * Switch active camera
   * @param {Camera} newCamera - new active camera
   * @param {boolean} attachControl - call attachControl for the new active
   * camera (default: true)
   */
  void switchActiveCamera(Camera* newCamera, bool attachControl = true);
  /**
   * sets the active camera of the scene using its ID
   * @param {string} id - the camera's ID
   * @return {BABYLON.Camera|null} the new active camera or null if none found.
   * @see activeCamera
   */
  Camera* setActiveCameraByID(const std::string& id);
  /**
   * sets the active camera of the scene using its name
   * @param {string} name - the camera's name
   * @return {BABYLON.Camera|null} the new active camera or null if none found.
   * @see activeCamera
   */
  Camera* setActiveCameraByName(const std::string& name);
  /**
   * get a material using its id
   * @param {string} the material's ID
   * @return {BABYLON.Material|null} the material or null if none found.
   */
  Material* getMaterialByID(const std::string& id);
  /**
   * get a material using its name
   * @param {string} the material's name
   * @return {BABYLON.Material|null} the material or null if none found.
   */
  Material* getMaterialByName(const std::string& name);
  LensFlareSystem* getLensFlareSystemByName(const std::string& name);
  LensFlareSystem* getLensFlareSystemByID(const std::string& id);
  std::vector<Camera*> getCameras() const;
  Camera* getCameraByID(const std::string& id);
  Camera* getCameraByUniqueID(unsigned int uniqueId);
  /**
   * get a camera using its name
   * @param {string} the camera's name
   * @return {BABYLON.Camera|null} the camera or null if none found.
   */
  Camera* getCameraByName(const std::string& name);
  /**
   * get a bone using its id
   * @param {string} the bone's id
   * @return {BABYLON.Bone|null} the bone or null if not found
   */
  Bone* getBoneByID(const std::string& id);
  /**
   * get a bone using its id
   * @param {string} the bone's name
   * @return {BABYLON.Bone|null} the bone or null if not found
   */
  Bone* getBoneByName(const std::string& name);
  /**
   * get a light node using its name
   * @param {string} the light's name
   * @return {BABYLON.Light|null} the light or null if none found.
   */
  Light* getLightByName(const std::string& name);
  /**
   * get a light node using its ID
   * @param {string} the light's id
   * @return {BABYLON.Light|null} the light or null if none found.
   */
  Light* getLightByID(const std::string& id);
  /**
   * get a light node using its scene-generated unique ID
   * @param {number} the light's unique id
   * @return {BABYLON.Light|null} the light or null if none found.
   */
  Light* getLightByUniqueID(unsigned int uniqueId);
  /**
   * get a particle system by id
   * @param id {number} the particle system id
   * @return {BABYLON.ParticleSystem|null} the corresponding system or null if
   * none found.
   */
  ParticleSystem* getParticleSystemByID(const std::string& id);
  /**
   * get a geometry using its ID
   * @param {string} the geometry's id
   * @return {BABYLON.Geometry|null} the geometry or null if none found.
   */
  Geometry* getGeometryByID(const std::string& id);
  /**
   * add a new geometry to this scene.
   * @param {BABYLON.Geometry} geometry - the geometry to be added to the scene.
   * @param {boolean} [force] - force addition, even if a geometry with this ID
   * already exists
   * @return {boolean} was the geometry added or not
   */
  bool pushGeometry(std::unique_ptr<Geometry>&& geometry, bool force = false);
  /**
   * Removes an existing geometry
   * @param {BABYLON.Geometry} geometry - the geometry to be removed from the
   * scene.
   * @return {boolean} was the geometry removed or not
   */
  bool removeGeometry(Geometry* geometry);
  std::vector<std::unique_ptr<Geometry>>& getGeometries();
  /**
   * Get the first added mesh found of a given ID
   * @param {string} id - the id to search for
   * @return {BABYLON.AbstractMesh|null} the mesh found or null if not found at
   * all.
   */
  AbstractMesh* getMeshByID(const std::string& id);
  /**
   * Get the all added meshes found of a given ID
   * @param {string} id - the id to search for
   * @return {BABYLON.AbstractMesh[]|[]]} the meshes found or an empty list if
   * not found at all.
   */
  std::vector<AbstractMesh*> getMeshesByID(const std::string& id);
  /**
   * Get a mesh with its auto-generated unique id
   * @param {number} uniqueId - the unique id to search for
   * @return {BABYLON.AbstractMesh|null} the mesh found or null if not found at
   * all.
   */
  AbstractMesh* getMeshByUniqueID(unsigned int uniqueId);
  /**
   * Get a the last added mesh found of a given ID
   * @param {string} id - the id to search for
   * @return {BABYLON.AbstractMesh|null} the mesh found or null if not found at
   * all.
   */
  AbstractMesh* getLastMeshByID(const std::string& id);
  std::vector<AbstractMesh*> getMeshes() const;
  /**
   * Get a the last added node (Mesh, Camera, Light) found of a given ID
   * @param {string} id - the id to search for
   * @return {BABYLON.Node|null} the node found or null if not found at all.
   */
  Node* getLastEntryByID(const std::string& id);
  Node* getNodeByID(const std::string& id);
  Node* getNodeByName(const std::string& name);
  AbstractMesh* getMeshByName(const std::string& name);
  Sound* getSoundByName(const std::string& name);
  Skeleton* getLastSkeletonByID(const std::string& id);
  Skeleton* getSkeletonById(const std::string& id);
  Skeleton* getSkeletonByName(const std::string& name);
  bool isActiveMesh(Mesh* mesh);
  /**
   * Return a unique id as a string which can serve as an identifier for the
   * scene
   */
  std::string uid();
  bool _isInIntermediateRendering() const;
  void updateTransformMatrix(bool force = false);
  void render();
  /** Audio **/
  bool audioEnabled() const;
  void setAudioEnabled(bool value);
  bool headphone() const;
  void setHeadphone(bool value);
  void _switchAudioModeForHeadphones();
  void _switchAudioModeForNormalSpeakers();
  /** Rendering **/
  DepthRenderer* enableDepthRenderer();
  void disableDepthRenderer();
  void freezeMaterials();
  void unfreezeMaterials();
  void dispose(bool doNotRecurse = false) override;
  /** Release sounds & sounds tracks **/
  void disposeSounds();
  /** Octrees **/
  MinMax getWorldExtends();
  Octree<AbstractMesh*>* createOrUpdateSelectionOctree(size_t maxCapacity = 64,
                                                       size_t maxDepth    = 2);
  /** Picking **/
  Ray* createPickingRay(int x, int y, Matrix* world, Camera* camera);
  Ray* createPickingRayInCameraSpace(int x, int y, Camera* camera);
  /**
   * @brief Launch a ray to try to pick a mesh in the scene
   * @param x X position on screen
   * @param y Y position on screen
   * @param predicate Predicate function used to determine eligible meshes. Can
   * be set to null. In this case, a mesh must be enabled, visible and with
   * isPickable set to true.
   * @param fastCheck Launch a fast check only using the bounding boxes.
   * @param camera camera to use for computing the picking ray. Can be set to
   * nullptr. In this case, the scene.activeCamera will be used.
   * @return picking info object
   */
  PickingInfo* pick(int x, int y,
                    const std::function<bool(AbstractMesh* mesh)>& predicate,
                    bool fastCheck = false, Camera* camera = nullptr);
  /**
   * @brief Launch a ray to try to pick a mesh in the scene
   * @param x X position on screen
   * @param y Y position on screen
   * @param predicate Predicate function used to determine eligible sprites. Can
   * be set to null. In this case, a sprite must have isPickable set to true
   * @param fastCheck Launch a fast check only using the bounding boxes.
   * @param camera camera to use for computing the picking ray. Can be set to
   * null. In this case, the scene.activeCamera will be used
   * @return picking info object
   */
  PickingInfo* pickSprite(int x, int y,
                          const std::function<bool(Sprite* sprite)>& predicate,
                          bool fastCheck = false, Camera* camera = nullptr);
  PickingInfo* pickWithRay(const Ray& ray,
                           const std::function<bool(Mesh* mesh)>& predicate,
                           bool fastCheck = false);
  /**
   * @brief Launch a ray to try to pick a mesh in the scene
   * @param x X position on screen
   * @param y Y position on screen
   * @param predicate Predicate function used to determine eligible meshes. Can
   * be set to null. In this case, a mesh must be enabled, visible and with
   * isPickable set to true
   * @param camera camera to use for computing the picking ray. Can be set to
   * null. In this case, the scene.activeCamera will be used
   * @return list with picking info objects
   */
  std::vector<PickingInfo*>
  multiPick(int x, int y,
            const std::function<bool(AbstractMesh* mesh)>& predicate,
            Camera* camera);
  /**
   * @brief Launch a ray to try to pick a mesh in the scene
   * @param ray Ray to use
   * @param predicate Predicate function used to determine eligible meshes. Can
   * be set to null. In this case, a mesh must be enabled, visible and with
   * isPickable set to true
   * @return list with picking info objects
   */
  std::vector<PickingInfo*>
  multiPickWithRay(const Ray& ray,
                   const std::function<bool(Mesh* mesh)>& predicate);
  AbstractMesh* getPointerOverMesh();
  void setPointerOverMesh(AbstractMesh* mesh);
  void setPointerOverSprite(Sprite* sprite);
  Sprite* getPointerOverSprite() const;
  /** Physics **/
  PhysicsEngine* getPhysicsEngine();
  /**
   * Enables physics to the current scene
   * @param {BABYLON.Vector3} [gravity] - the scene's gravity for the physics
   * engine
   * @param {BABYLON.IPhysicsEnginePlugin} [plugin] - The physics engine to be
   * used. defaults to OimoJS.
   * @return {boolean} was the physics engine initialized
   */
  bool enablePhysics(const Vector3& gravity, IPhysicsEnginePlugin* plugin);
  void disablePhysicsEngine();
  bool isPhysicsEnabled();
  /** Misc. **/
  void createDefaultCameraOrLight();
  /** Tags **/
  std::vector<Mesh*> getMeshesByTags();
  std::vector<Camera*> getCamerasByTags();
  std::vector<Light*> getLightsByTags();
  std::vector<Material*> getMaterialByTags();

  /**
   * Overrides the default sort function applied in the renderging group to
   * prepare the meshes.
   * This allowed control for front to back rendering or reversly depending of
   * the special needs.
   *
   * @param renderingGroupId The rendering group id corresponding to its index
   * @param opaqueSortCompareFn The opaque queue comparison function use to
   * sort.
   * @param alphaTestSortCompareFn The alpha test queue comparison function use
   * to sort.
   * @param transparentSortCompareFn The transparent queue comparison function
   * use to sort.
   */
  void setRenderingOrder(
    int renderingGroupId,
    const std::function<int(SubMesh* a, SubMesh* b)>& opaqueSortCompareFn
    = nullptr,
    const std::function<int(SubMesh* a, SubMesh* b)>& alphaTestSortCompareFn
    = nullptr,
    const std::function<int(SubMesh* a, SubMesh* b)>& transparentSortCompareFn
    = nullptr);

  /**
   * Specifies whether or not the stencil and depth buffer are cleared between
   * two rendering groups.
   *
   * @param renderingGroupId The rendering group id corresponding to its index
   * @param autoClearDepthStencil Automatically clears depth and stencil between
   * groups if true.
   */
  void setRenderingAutoClearDepthStencil(int renderingGroupId,
                                         bool autoClearDepthStencil);

protected:
  /**
   * Constructor
   * @param {BABYLON.Engine} engine - the engine to be used to render this
   * scene.
   */
  explicit Scene(Engine* engine);

private:
  void _updatePointerPosition(const PointerEvent evt);
  void _animate(const millisecond_t& delay = std::chrono::milliseconds(0));
  void _evaluateSubMesh(SubMesh* subMesh, AbstractMesh* mesh);
  void _evaluateActiveMeshes();
  void _activeMesh(AbstractMesh* mesh);
  void _renderForCamera(Camera* camera);
  void _processSubCameras(Camera* camera);
  void _checkIntersections();
  void _updateAudioParameters();
  /** Pointers handling **/
  void _onPointerMoveEvent(PointerEvent&& evt);
  void _onPointerDownEvent(PointerEvent&& evt);
  void _onPointerUpEvent(PointerEvent&& evt);
  void _onKeyDownEvent(Event&& evt);
  void _onKeyUpEvent(Event&& evt);
  /** Audio **/
  void _disableAudio();
  void _enableAudio();
  // void _switchAudioModeForHeadphones();
  // void _switchAudioModeForNormalSpeakers();
  /** Picking **/
  PickingInfo*
  _internalPick(const std::function<Ray(const Matrix& world)>& rayFunction,
                const std::function<bool(AbstractMesh* mesh)>& predicate,
                bool fastCheck);
  std::vector<PickingInfo*>
  _internalMultiPick(const std::function<Ray(const Matrix& world)>& rayFunction,
                     const std::function<bool(AbstractMesh* mesh)>& predicate);
  PickingInfo*
  _internalPickSprites(const Ray& ray,
                       const std::function<bool(Sprite* sprite)>& predicate,
                       bool fastCheck, Camera* camera);
  /** Tags **/
  std::vector<std::string> _getByTags();

public:
  // Members
  bool autoClear;
  Color3 clearColor;
  Color3 ambientColor;
  // Events
  /**
   * An event triggered when the scene is disposed.
   */
  Observable<Scene> onDisposeObservable;
  /**
   * An event triggered before rendering the scene
   */
  Observable<Scene> onBeforeRenderObservable;
  /**
   * An event triggered after rendering the scene
   */
  Observable<Scene> onAfterRenderObservable;
  /**
   * An event triggered after rendering the scene
   */
  Observable<Scene> onReadyObservable;
  /**
   * An event triggered before rendering a camera
   */
  Observable<Camera> onBeforeCameraRenderObservable;
  /**
   * An event triggered after rendering a camera
   */
  Observable<Camera> onAfterCameraRenderObservable;
  /**
   * An event triggered when a camera is created
   */
  Observable<Camera> onNewCameraAddedObservable;

  /**
   * An event triggered when a camera is removed
   */
  Observable<Camera> onCameraRemovedObservable;

  /**
   * An event triggered when a light is created
   */
  Observable<Light> onNewLightAddedObservable;

  /**
   * An event triggered when a light is removed
   */
  Observable<Light> onLightRemovedObservable;

  /**
   * An event triggered when a geometry is created
   */
  Observable<Geometry> onNewGeometryAddedObservable;

  /**
   * An event triggered when a geometry is removed
   */
  Observable<Geometry> onGeometryRemovedObservable;

  /**
   * An event triggered when a mesh is created
   */
  Observable<AbstractMesh> onNewMeshAddedObservable;

  /**
   * An event triggered when a mesh is removed
   */
  Observable<AbstractMesh> onMeshRemovedObservable;

  /**
   * This Observable will be triggered for each stage of each renderingGroup of
   * each rendered camera. The RenderinGroupInfo class contains all the
   * information about the context in which the observable is called If you wish
   * to register an Observer only for a given set of renderingGroup, use the
   * mask with a combination of the renderingGroup index elevated to the power
   * of two (1 for renderingGroup 0, 2 for renderingrOup1, 4 for 2 and 8 for 3)
   */
  Observable<RenderingGroupInfo> onRenderingGroupObservable;

  // Animations
  std::vector<Animation*> animations;

  // Pointers
  std::function<bool(AbstractMesh* Mesh)> pointerDownPredicate;
  std::function<bool(AbstractMesh* Mesh)> pointerUpPredicate;
  std::function<bool(AbstractMesh* Mesh)> pointerMovePredicate;

  bool forceWireframe;
  bool forcePointsCloud;
  bool forceShowBoundingBoxes;
  std::unique_ptr<Plane> _clipPlane;
  bool animationsEnabled;
  bool constantlyUpdateMeshUnderPointer;
  bool useRightHandedSystem;

  std::string hoverCursor;

  // Metadata
  Json::object metadata;

  // Pointers

  /**
   * This observable event is triggered when any mouse event registered during
   * Scene.attach() is called BEFORE the 3D engine to process anything
   * (mesh/sprite picking for instance).
   * You have the possibility to skip the 3D Engine process and the call to
   * onPointerObservable by setting PointerInfoBase.skipOnPointerObservable to
   * true
   */
  Observable<PointerInfoPre> onPrePointerObservable;

  /**
   * Observable event triggered each time an input event is received from the
   * rendering canvas
   */
  Observable<PointerInfo> onPointerObservable;

  Camera* cameraToUseForPointers;
  // Mirror
  std::unique_ptr<Vector3> _mirroredCameraPosition;
  // Fog
  /**
   * is fog enabled on this scene.
   */
  bool fogEnabled;
  unsigned int fogMode;
  Color3 fogColor;
  float fogDensity;
  float fogStart;
  float fogEnd;
  // Lights
  /**
   * is shadow enabled on this scene.
   */
  bool shadowsEnabled;
  /**
   * is light enabled on this scene.
   */
  bool lightsEnabled;
  /**
   * All of the lights added to this scene.
   * @see BABYLON.Light
   */
  std::vector<std::unique_ptr<Light>> lights;
  // Cameras
  /**
   * All of the cameras added to this scene.
   * @see BABYLON.Camera
   */
  std::vector<std::unique_ptr<Camera>> cameras;
  std::vector<Camera*> activeCameras;
  Camera* activeCamera;
  // Meshes
  /**
   * All of the (abstract) meshes added to this scene.
   * @see BABYLON.AbstractMesh
   */
  std::vector<std::unique_ptr<AbstractMesh>> meshes;
  // Geometries
  std::vector<std::unique_ptr<Material>> materials;
  std::vector<std::unique_ptr<MultiMaterial>> multiMaterials;
  // Textures
  bool texturesEnabled;
  std::vector<std::unique_ptr<BaseTexture>> textures;
  // Particles
  bool particlesEnabled;
  std::vector<std::unique_ptr<ParticleSystem>> particleSystems;
  // Sprites
  bool spritesEnabled;
  std::vector<std::unique_ptr<SpriteManager>> spriteManagers;
  std::function<bool(Sprite* sprite)> spritePredicate;
  // Layers
  std::vector<Layer*> layers;
  std::vector<std::unique_ptr<HighlightLayer>> highlightLayers;
  // Skeletons
  bool skeletonsEnabled;
  std::vector<std::unique_ptr<Skeleton>> skeletons;
  // Lens flares
  bool lensFlaresEnabled;
  std::vector<std::unique_ptr<LensFlareSystem>> lensFlareSystems;
  // Collisions
  bool collisionsEnabled;
  std::unique_ptr<ICollisionCoordinator> collisionCoordinator;
  Vector3 gravity;
  // Postprocesses
  bool postProcessesEnabled;
  std::unique_ptr<PostProcessManager> postProcessManager;
  std::unique_ptr<PostProcessRenderPipelineManager>
    postProcessRenderPipelineManager;
  // Customs render targets
  bool renderTargetsEnabled;
  bool dumpNextRenderTargets;
  std::vector<RenderTargetTexture*> customRenderTargets;
  // Delay loading
  bool useDelayedTextureLoading;
  // Imported meshes
  std::vector<std::string> importedMeshesFiles;
  // Probes
  bool probesEnabled;
  std::vector<std::unique_ptr<ReflectionProbe>> reflectionProbes;
  // Database
  // public database; //ANY
  // Actions
  /**
   * This scene's action manager
   */
  ActionManager* actionManager;
  std::vector<std::unique_ptr<ActionManager>> _actionManagers;
  // Procedural textures
  bool proceduralTexturesEnabled;
  std::vector<std::unique_ptr<ProceduralTexture>> _proceduralTextures;
  // Sound Tracks
  std::unique_ptr<SoundTrack> mainSoundTrack;
  std::vector<SoundTrack*> soundTracks;
  // Simplification Queue
  std::unique_ptr<SimplificationQueue> simplificationQueue;
  // Performance counters
  PerfCounter _activeIndices;
  PerfCounter _activeParticles;
  PerfCounter _particlesDuration;
  PerfCounter _spritesDuration;
  PerfCounter _activeBones;
  Material* _cachedMaterial;
  std::vector<IDisposable*> _toBeDisposed;
  std::vector<ParticleSystem*> _activeParticleSystems;
  std::vector<Animatable*> _activeAnimatables;

private:
  // Events
  Observer<Scene>::Ptr _onDisposeObserver;
  Observer<Scene>::Ptr _onBeforeRenderObserver;
  Observer<Scene>::Ptr _onAfterRenderObserver;
  Observer<Camera>::Ptr _onBeforeCameraRenderObserver;
  Observer<Camera>::Ptr _onAfterCameraRenderObserver;
  // Pointers
  std::function<void(PointerEvent&& evt)> _onPointerMove;
  std::function<void(PointerEvent&& evt)> _onPointerDown;
  std::function<void(PointerEvent&& evt)> _onPointerUp;
  int _pointerX;
  int _pointerY;
  int _unTranslatedPointerX;
  int _unTranslatedPointerY;
  Vector2 _startingPointerPosition;
  high_res_time_point_t _startingPointerTime;
  // AbstractMesh* _meshUnderPointer;
  std::function<void()> beforeRender;
  std::function<void()> afterRender;
  // Keyboard
  std::function<void(Event&& evt)> _onKeyDown;
  std::function<void(Event&& evt)> _onKeyUp;
  // Geometries
  std::vector<std::unique_ptr<Geometry>> _geometries;
  // Materials
  StandardMaterial* _defaultMaterial;
  // Collisions
  bool _workerCollisions;
  // Actions
  std::vector<AbstractMesh*> _meshesForIntersections;
  // Sound Tracks
  bool _hasAudioEngine;
  bool _audioEnabled;
  bool _headphone;
  Engine* _engine;
  // Performance counters
  PerfCounter _totalMeshesCounter;
  PerfCounter _totalLightsCounter;
  PerfCounter _totalMaterialsCounter;
  PerfCounter _totalTexturesCounter;
  PerfCounter _totalVertices;
  PerfCounter _lastFrameDuration;
  PerfCounter _evaluateActiveMeshesDuration;
  PerfCounter _renderTargetsDuration;
  PerfCounter _renderDuration;
  float _animationRatio;
  bool _animationStartDateSet;
  high_res_time_point_t _animationStartDate;
  int _renderId;
  int _executeWhenReadyTimeoutId;
  bool _intermediateRendering;
  // _pendingData = [];//ANY
  std::vector<Mesh*> _activeMeshes;
  std::vector<Material*> _processedMaterials;
  std::vector<RenderTargetTexture*> _renderTargets;
  std::vector<Skeleton*> _activeSkeletons;
  std::vector<Mesh*> _softwareSkinnedMeshes;
  std::unique_ptr<RenderingManager> _renderingManager;
  std::unique_ptr<PhysicsEngine> _physicsEngine;
  Matrix _transformMatrix;
  Matrix _pickWithRayInverseMatrix;
  std::vector<EdgesRenderer*> _edgesRenderers;
  std::unique_ptr<BoundingBoxRenderer> _boundingBoxRenderer;
  std::unique_ptr<OutlineRenderer> _outlineRenderer;
  Matrix _viewMatrix;
  Matrix _projectionMatrix;
  bool _frustumPlanesSet;
  std::array<Plane, 6> _frustumPlanes;
  Octree<AbstractMesh*>* _selectionOctree;
  AbstractMesh* _pointerOverMesh;
  Sprite* _pointerOverSprite;
  std::unique_ptr<DebugLayer> _debugLayer;
  std::unique_ptr<DepthRenderer> _depthRenderer;
  unsigned int _uniqueIdCounter;
  AbstractMesh* _pickedDownMesh;
  Sprite* _pickedDownSprite;
  std::string _uid;

}; // end of class Scene

} // end of namespace BABYLON

#endif // end of BABYLON_ENGINE_SCENE_H
