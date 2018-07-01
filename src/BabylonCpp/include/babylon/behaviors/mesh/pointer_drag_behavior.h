#ifndef BABYLON_BEHAVIORS_MESH_POINTER_DRAG_BEHAVIOR_H
#define BABYLON_BEHAVIORS_MESH_POINTER_DRAG_BEHAVIOR_H

#include <babylon/babylon_global.h>

#include <babylon/behaviors/behavior.h>
#include <babylon/behaviors/mesh/pointer_drag_behavior_options.h>
#include <babylon/core/structs.h>
#include <babylon/tools/observable.h>
#include <babylon/tools/observer.h>

namespace BABYLON {

/**
 * @brief A behavior that when attached to a mesh will allow the mesh to be
 * dragged around the screen based on pointer events.
 */
class BABYLON_SHARED_EXPORT PointerDragBehavior : public Behavior<Node> {

public:
  /**
   * @brief Creates a pointer drag behavior that can be attached to a mesh.
   * @param options The drag axis or normal of the plane that will be dragged
   * across. pointerObservableScene can be used to listen to drag events from
   * another scene(eg. if the attached mesh is in an overlay scene).
   */
  PointerDragBehavior(const PointerDragBehaviorOptions& options
                      = PointerDragBehaviorOptions());
  virtual ~PointerDragBehavior();

  /**
   * @brief The name of the behavior.
   */
  const string_t name() const;

  /**
   * @brief Initializes the behavior.
   */
  void init() override;

  /**
   * @brief Attaches the drag behavior the passed in mesh.
   * @param ownerNode The mesh that will be dragged around once attached
   */
  void attach(Node* ownerNode) override;

  void releaseDrag();

  /**
   * @brief Detaches the behavior from the mesh.
   */
  void detach() override;

private:
  Nullable<Vector3> _pickWithRayOnDragPlane(const Nullable<Ray>& ray);

  /**
   * @brief Position the drag plane based on the attached mesh position, for
   * single axis rotate the plane along the axis to face the camera.
   */
  void _updateDragPlanePosition(const Ray& ray,
                                const Vector3& dragPlanePosition);

public:
  /**
   * The id of the pointer that is currently interacting with the behavior (-1
   * when no pointer is active)
   */
  int currentDraggingPointerID;
  /**
   * The last position where the pointer hit the drag plane in world space
   */
  Vector3 lastDragPosition;
  /**
   * If the behavior is currently in a dragging state
   */
  bool dragging;
  /**
   * The distance towards the target drag position to move each frame. This
   * can be useful to avoid jitter. Set this to 1 for no delay. (Default: 0.2)
   */
  float dragDeltaRatio;
  /**
   * If the drag plane orientation should be updated during the dragging
   * (Default: true)
   */
  bool updateDragPlane;
  /**
   *  Fires each time the attached mesh is dragged with the pointer
   *  * delta between last drag position and current drag position in world
   *    space
   *  * dragDistance along the drag axis
   *  * dragPlaneNormal normal of the current drag plane used during the drag
   *  * dragPlanePoint in world space where the drag intersects the drag plane
   */
  Observable<DragMoveEvent> onDragObservable;
  /**
   * Fires each time a drag begins (eg. mouse down on mesh)
   */
  Observable<DragStartOrEndEvent> onDragStartObservable;
  /**
   * Fires each time a drag ends (eg. mouse release after drag)
   */
  Observable<DragStartOrEndEvent> onDragEndObservable;
  /**
   * If the attached mesh should be moved when dragged
   */
  bool moveAttached;
  /**
   *  If the drag behavior will react to drag events (Default: true)
   */
  bool enabled;
  /**
   * If set, the drag plane/axis will be rotated based on the attached mesh's
   * world rotation (Default: true)
   */
  bool useObjectOrienationForDragging;

  static unique_ptr_t<Scene> _planeScene;

private:
  Node* _attachedNode;
  Mesh* _dragPlane;
  Scene* _scene;
  Observer<PointerInfo>::Ptr _pointerObserver;
  int _draggingID;
  // Debug mode will display drag planes to help visualize behavior
  bool _debugMode;
  bool _moving;

  /**
   * The drag axis or normal of the plane that will be dragged across.
   * pointerObservableScene can be used to listen to drag events from another
   * scene(eg. if the attached mesh is in an overlay scene)
   */
  PointerDragBehaviorOptions options;

  Vector3 _tmpVector;
  Vector3 _worldDragAxis;

  // Variables to avoid instantiation in the method _updateDragPlanePosition
  Vector3 _pointA;
  Vector3 _pointB;
  Vector3 _pointC;
  Vector3 _lineA;
  Vector3 _lineB;
  Vector3 _localAxis;
  Vector3 _lookAt;

}; // end of class PointerDragBehavior

} // end of namespace BABYLON

#endif // end of BABYLON_BEHAVIORS_MESH_POINTER_DRAG_BEHAVIOR_H