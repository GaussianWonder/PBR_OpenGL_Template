#ifndef __PATHS_H__
#define __PATHS_H__


// useful for consteval const char *
#define PathConcat(PATH1, PATH2) PATH1 PATH2
#define ShaderFolder "./assets/shaders"
#define ModelFolder "./assets/models"
#define TextureFolder "./assets/textures"
#define AssetsFolder "./assets"

// Useful later on, when absolute paths are required
// #include <filesystem>
// std::filesystem::current_path()

#endif // __PATHS_H__
