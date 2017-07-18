#ifndef BABYLON_TOOLS_TEXTURE_TOOLS_H
#define BABYLON_TOOLS_TEXTURE_TOOLS_H

#include <babylon/babylon_global.h>

namespace BABYLON {

/**
 * @brief Represents the texture tools class.
 */
struct BABYLON_SHARED_EXPORT TextureTools {

  /**
   * @brief Uses the GPU to create a copy texture rescaled at a given size.
   * @param texture Texture to copy from
   * @param width Desired width
   * @param height Desired height
   * @return Generated texture
   */
  static std::unique_ptr<Texture>
  CreateResizedCopy(Texture* texture, int width, int height,
                    bool useBilinearMode = true);

}; // end of struct TextureTools

} // end of namespace BABYLON

#endif // end of BABYLON_TOOLS_TEXTURE_TOOLS_H