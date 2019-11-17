#ifndef BABYLON_MATERIALS_TEXTURES_INTERNAL_TEXTURE_H
#define BABYLON_MATERIALS_TEXTURES_INTERNAL_TEXTURE_H

#include <variant>

#include <babylon/babylon_api.h>
#include <babylon/babylon_common.h>
#include <babylon/core/structs.h>
#include <babylon/materials/textures/iinternal_texture_tracker.h>
#include <babylon/misc/observable.h>

namespace BABYLON {

struct ArrayBufferView;
class BaseTexture;
class Engine;
class ICanvasRenderingContext2D;
class InternalTexture;
class SphericalPolynomial;
using BaseTexturePtr         = std::shared_ptr<BaseTexture>;
using InternalTexturePtr     = std::shared_ptr<InternalTexture>;
using SphericalPolynomialPtr = std::shared_ptr<SphericalPolynomial>;

namespace GL {
class IGLFramebuffer;
class IGLRenderbuffer;
class IGLTexture;
} // namespace GL

/**
 * @brief Class used to store data associated with WebGL texture data for the engine This class
 * should not be used directly.
 */
class BABYLON_SHARED_EXPORT InternalTexture : public std::enable_shared_from_this<InternalTexture>,
                                              public IInternalTextureTracker {

public:
  /**
   * The source of the texture data is unknown
   */
  static constexpr unsigned int DATASOURCE_UNKNOWN = 0;
  /**
   * Texture data comes from an URL
   */
  static constexpr unsigned int DATASOURCE_URL = 1;
  /**
   * Texture data is only used for temporary storage
   */
  static constexpr unsigned int DATASOURCE_TEMP = 2;
  /**
   * Texture data comes from raw data (ArrayBuffer)
   */
  static constexpr unsigned int DATASOURCE_RAW = 3;
  /**
   * Texture content is dynamic (video or dynamic texture)
   */
  static constexpr unsigned int DATASOURCE_DYNAMIC = 4;
  /**
   * Texture content is generated by rendering to it
   */
  static constexpr unsigned int DATASOURCE_RENDERTARGET = 5;
  /**
   * Texture content is part of a multi render target process
   */
  static constexpr unsigned int DATASOURCE_MULTIRENDERTARGET = 6;
  /**
   * Texture data comes from a cube data file
   */
  static constexpr unsigned int DATASOURCE_CUBE = 7;
  /**
   * Texture data comes from a raw cube data
   */
  static constexpr unsigned int DATASOURCE_CUBERAW = 8;
  /**
   * Texture data come from a prefiltered cube data file
   */
  static constexpr unsigned int DATASOURCE_CUBEPREFILTERED = 9;
  /**
   * Texture content is raw 3D data
   */
  static constexpr unsigned int DATASOURCE_RAW3D = 10;
  /**
   * Texture content is a depth texture
   */
  static constexpr unsigned int DATASOURCE_DEPTHTEXTURE = 11;

  /**
   * Texture data comes from a raw cube data encoded with RGBD
   */
  static constexpr unsigned int DATASOURCE_CUBERAW_RGBD = 12;

public:
  template <typename... Ts>
  static InternalTexturePtr New(Ts&&... args)
  {
    auto texture = std::shared_ptr<InternalTexture>(new InternalTexture(std::forward<Ts>(args)...));

    return texture;
  }
  ~InternalTexture(); // = default

  /**
   * @brief Gets the Engine the texture belongs to.
   * @returns The babylon engine
   */
  Engine* getEngine();

  /**
   * @brief Gets the data source type of the texture (can be one of the
   * InternalTexture.DATASOURCE_XXXX).
   */
  unsigned int dataSource() const;

  /**
   * @brief Increments the number of references (ie. the number of Texture that point to it).
   */
  void incrementReferences();

  /**
   * @brief Change the size of the texture (not the size of the content).
   * @param width defines the new width
   * @param height defines the new height
   * @param depth defines the new depth (1 by default)
   */
  void updateSize(int width, int height, int depth = 1);

  /**
   * @brief Rebuilds the internal texture.
   */
  void _rebuild();

  /**
   * @brief Hidden
   */
  void _swapAndDie(const InternalTexturePtr& target);

  /**
   * @brief Dispose the current allocated resources.
   */
  void dispose();

protected:
  /**
   * @brief Creates a new InternalTexture.
   * @param engine defines the engine to use
   * @param dataSource defines the type of data that will be used
   * @param delayAllocation if the texture allocation should be delayed (default: false)
   */
  InternalTexture(Engine* engine, unsigned int dataSource = InternalTexture::DATASOURCE_UNKNOWN,
                  bool delayAllocation = false);

public:
  /**
   * Defines if the texture is ready
   */
  bool isReady;
  /**
   * Defines if the texture is a cube texture
   */
  bool isCube;
  /**
   * Defines if the texture contains 3D data
   */
  bool is3D;
  /**
   * Defines if the texture contains multiview data
   */
  bool isMultiview;
  /**
   * Gets the URL used to load this texture
   */
  std::string url;
  /**
   * Gets the sampling mode of the texture
   */
  unsigned int samplingMode;
  /**
   * Gets a boolean indicating if the texture needs mipmaps generation
   */
  bool generateMipMaps;
  /**
   * Gets the number of samples used by the texture (WebGL2+ only)
   */
  unsigned int samples;
  /**
   * Gets the type of the texture (int, float...)
   */
  unsigned int type;
  /**
   * Gets the format of the texture (RGB, RGBA...)
   */
  unsigned int format;
  /**
   * Observable called when the texture is loaded
   */
  Observable<InternalTexture> onLoadedObservable;
  /**
   * Gets the width of the texture
   */
  int width;
  /**
   * Gets the height of the texture
   */
  int height;
  /**
   * Gets the depth of the texture
   */
  int depth;
  /**
   * Gets the initial width of the texture (It could be rescaled if the current system does not
   * support non power of two textures)
   */
  int baseWidth;
  /**
   * Gets the initial height of the texture (It could be rescaled if the current system does not
   * support non power of two textures)
   */
  int baseHeight;
  /**
   * Gets the initial depth of the texture (It could be rescaled if the current system does not
   * support non power of two textures)
   */
  int baseDepth;
  /**
   * Gets a boolean indicating if the texture is inverted on Y axis
   */
  bool invertY;

  // Private
  bool _invertVScale;
  int _associatedChannel;
  unsigned int _dataSource;
  std::variant<std::string, ArrayBuffer, Image> _buffer;
  ArrayBuffer _bufferView;
  std::vector<ArrayBufferView> _bufferViewArray;
  std::vector<std::vector<ArrayBuffer>> _bufferViewArrayArray;
  int _size;
  std::string _extension;
  std::vector<std::string> _files;
  ICanvas* _workingCanvas;
  ICanvasRenderingContext2D* _workingContext;
  std::shared_ptr<GL::IGLFramebuffer> _framebuffer;
  std::shared_ptr<GL::IGLRenderbuffer> _depthStencilBuffer;
  std::shared_ptr<GL::IGLFramebuffer> _MSAAFramebuffer;
  Uint32Array _attachments;
  std::unique_ptr<GL::IGLRenderbuffer> _MSAARenderBuffer;
  unsigned int _cachedCoordinatesMode;
  unsigned int _cachedWrapU;
  unsigned int _cachedWrapV;
  unsigned int _cachedWrapR;
  unsigned int _cachedAnisotropicFilteringLevel;
  bool _isDisabled;
  std::string _compression;
  bool _generateStencilBuffer;
  bool _generateDepthBuffer;
  int _comparisonFunction;
  SphericalPolynomialPtr _sphericalPolynomial;
  float _lodGenerationScale;
  float _lodGenerationOffset;

  // Multiview
  /** Hidden */
  std::unique_ptr<GL::IGLTexture> _colorTextureArray;
  /** Hidden */
  std::unique_ptr<GL::IGLTexture> _depthStencilTextureArray;

  // The following three fields helps sharing generated fixed LODs for texture filtering
  // In environment not supporting the textureLOD extension like EDGE. They are for internal use
  // only. They are at the level of the gl texture to benefit from the cache.
  BaseTexturePtr _lodTextureHigh;
  BaseTexturePtr _lodTextureMid;
  BaseTexturePtr _lodTextureLow;
  bool _isRGBD;

  /** Hidden */
  bool _linearSpecularLOD;
  /** Hidden */
  BaseTexturePtr _irradianceTexture;

  std::shared_ptr<GL::IGLTexture> _webGLTexture;
  int _references;

private:
  Engine* _engine;

}; // end of struct InternalTexture

} // end of namespace BABYLON

#endif // end of BABYLON_MATERIALS_TEXTURES_INTERNAL_TEXTURE_H
