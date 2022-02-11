# OpenGL Template

- [OpenGL Template](#opengl-template)
  - [Specs](#specs)
  - [Build](#build)
    - [Help menu](#help-menu)
    - [Get up and running](#get-up-and-running)
    - [Fast rebuild and run](#fast-rebuild-and-run)
    - [Build without the helper script](#build-without-the-helper-script)
  - [Recommendations](#recommendations)
  - [Walkthrough](#walkthrough)
    - [PBR](#pbr)
      - [Vertex shader](#vertex-shader)
      - [Geometry shader](#geometry-shader)
      - [Fragment shader](#fragment-shader)
    - [Textures](#textures)
    - [Remaining Code](#remaining-code)
    - [Conclusion](#conclusion)

## Specs

This is an OpengGL project template written c++.

**It features:**

- Model loading
  - Mesh handling
  - Texture handling
- Skybox loading
- Shader loading
  - Basic shader program
  - +Geometry shader
  - Clunky uniform updaters
- Skybox loading
- Logger
  - Clunky opengl error handler
- Controllable Widgets
- PBR
  - Metallic texture
  - Roughness texture
  - Normal Map
  - Albedo
  - Exposure
  - Gamma correction
- Controllable Lights

> **TODO:** hdri, IBL, specular maps, efficient TBN, higher resolution textures

<div class="page" />

## Build

At the time of writing this, the conan version used is `Conan version 1.44.0` with experimental features enabled.

The following aliases are used in this document:

```bash
alias run="./run.sh"
alias log="./log_run.sh"
```

Both **run** and **log** accept the same arguments, that of the command to **run**. Both commands will log runtime information, however `log.sh` will also log the whole building process.

### Help menu

```bash
run help
```

```text
Example:
  run clean build opengl_template
  run opengl_template
  run build
  run clean build
  run clean
  run conan

  :help   displays this message
  :clean  clean build folder
  :build  build project
  :exec   execute the executable
  :cb     clean build shorthand
  :conan  same as build
  :dependencies  conan install dependencies
```

The run script will take its arguments and execute them sequentially.

Custom commands can be provided as stringified paths to the run command.

Since this is a simple script, i recommend to read it for further customisation and understanding of the build process.

### Get up and running

```bash
run clean dependencies build execute
```

### Fast rebuild and run

```bash
run build execute
```

### Build without the helper script

```bash
# Create build folder
mkdir build 
cd build
# Copy dependencies that will be compiled alongside this project
conan source .. --source-folder dependencies
# Build and Link other dependencies that do not require dependency management
conan install .. --build missing
# Build the project
conan build ..
```

## Recommendations

If using VSCode, I recommend the following settings for **command-runner**.

```json
"command-runner.terminal.autoClear": true,
"command-runner.terminal.autoFocus": true,
"command-runner.commands": {
  "install": "./run.sh dependencies",
  "build": "./run.sh conan",
  "run": "./run.sh conan execute",
  "clean": "./run.sh clean",
  "build run": "./run.sh dependencies conan execute",
  "log build": "./log_run.sh dependencies conan",
  "log run": "./log_run.sh conan execute",
  "log build run": "./log_run.sh dependencies conan execute"
}
```

Now useful commands are accesible via the `CTRL + SHIFT + R` shortcut.

<div class="page" />

## Walkthrough

Code can be inspected, so i'll walk only through bits and pieces.

### PBR

Most of the research is done on [this](https://learnopengl.com/PBR/Theory) article, thus the implementation is *somewhat* similar, containing tiny changes to light handling and the calculation of the TBN matrix as follows:

1. My current approach is to load a **geometry shader** and perform the necessary calculations for the **TBN matrix** there, and pass it along to the fragment shader. Currently, the **normalMap** is *mapped* to tanget space in the **fragment** shader, however a better approach is to leave everything as is, and **multiply light variables** with the *TBN* matrix inside the *geometry shader*.
2. When dealing with light attenuation, I use **linear** and **quadratic** parameters to control the radius in effect of the light source (for point lights).
3. Although not necessary, there is a **minimum albedo color** *(0.005f)* mixed in as a means of poor global illumination. Although it has nothing to do with the real ambient lighting, it is there to prevent pitch black objects from being completely black due to lack of illumination.

> inputs and outputs are prepended with the initial letter of the shader type, e.g. `fPosition`

#### Vertex shader

**No calculation** is performed here, not even the *model transformation*. This is because everything is passed into the *geometry shader*, and the *vPosition* is needed.

```glsl
// ... in & out

void main()
{
  gTexCoords = vTexCoords;
  gNormal = vNormal;
  gModel = model;

  gProjection = projection;
  gView = view;
  gCamPos = cameraPosition;

  gl_Position = vec4(vPosition, 1.0);
}
```

<div class="page" />

#### Geometry shader

The geometry shader first defines the **vertex layout** that is going to process. In this case triangles.

```glsl
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
// ... in & out
```

This is where the TBN matrix is calculated.

```glsl
  // Edges of the triangle
  vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  // Lengths of UV differences
  vec2 deltaUV0 = gTexCoords[1] - gTexCoords[0];
  vec2 deltaUV1 = gTexCoords[2] - gTexCoords[0];

  // one over the determinant
  float invDet = 1.0 / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

  vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
  vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

  vec3 T = normalize(vec3(gModel[0] * vec4(tangent, 0.0)));
  vec3 B = normalize(vec3(gModel[0] * vec4(bitangent, 0.0)));
  vec3 N = normalize(vec3(gModel[0] * vec4(cross(edge1, edge0), 0.0)));

  mat3 TBN = mat3(T, B, N);
```

At this point the TBN matrix can be multiplied with N (normal in fragment shader) to bring it to tanget space.

If a better solution is seeked, I would suggest inverting the TBN matrix, and applying it on every light information available (such as direction and position).

In order for this to work, light variable uniforms need to be moved to the geometry shader first.

```glsl
  // TBN is an orthogonal matrix and so its inverse is equal to its transpose
  TBN = transpose(TBN);
```

<div class="page" />

Now we can take each vertex, setup its *input* and *output*, and **emit them sequentially**.

```glsl
  for (int i=0; i<3; ++i)
  {
    vec4 pos = gModel[i] * gl_in[i].gl_Position;
    gl_Position = gProjection[i] * gView[i] * pos;
    fNormal = gNormal[i];
    fTexCoords = gTexCoords[i];
    // Change all lighting variables to TBN space
    fPosition = pos.xyz;
    fCamPos = gCamPos[i];
    EmitVertex();
  }

  EndPrimitive();
```

#### Fragment shader

This is where the magic happens.

Unfortunatelly it *only* supports textures for every attribute of the basic **principle BRDF**, no solid colors or vec3 values for roughness and metallic. The minimum texture size is about 64x64, so memory wise it is not the worst thing ever.

```glsl
  vec3 albedo = texture(diffuseTexture, fTexCoords).rgb;
  float metalness = texture(metallicTexture, fTexCoords).r;
  float roughness = texture(roughnessTexture, fTexCoords).r;
  vec3 N = normalize(2.0 * texture(normalMap, fTexCoords).rgb - 1.0);
  N = normalize(fTBN * N);
```

First we calculate **THE** allmighty pair of vectors that **define light directions**.

```glsl
  // Outgoing light direction (vector from world-space fragment position to the "eye").
  vec3 Lo = normalize(fCamPos - fPosition);

  // Angle between surface normal and outgoing light direction.
  float cosLo = max(0.0, dot(N, Lo));

  // Specular reflection vector.
  vec3 Lr = 2.0 * cosLo * N - Lo;
```

<div class="page" />

Get the **base reflectance** color using Fresnel constant for dielectric materials.

```glsl
// Fresnel reflectance at normal incidence (for metals use albedo color).
vec3 F0 = mix(vec3(0.04), albedo, metalness);
```

Now we can start **accumulating light information**. For directional light:

```glsl
  vec3 directLighting = vec3(0);
  for(int i=0; i<point_light_count; ++i)
  {
    // Light direction
    vec3 Li = -normalize(pointLPos[i] - fPosition);
    // irradiance attenuation
    float dst = length(pointLPos[i] - fPosition);
    float attenuation = 1.0 / (1.0 + pointLLin[i] * dst +  pointLQuad[i] * (dst * dst));
    // Actual light radiance
    vec3 Lradiance = pointLCol[i] * attenuation;
    // Half-vector between Li and Lo.
    vec3 Lh = normalize(Li + Lo);
    // Calculate angles between surface normal and various light vectors.
    float cosLi = max(0.0, dot(N, Li));
    float cosLh = max(0.0, dot(N, Lh));
    // Calculate Fresnel term for direct lighting. 
    vec3 F  = fresnelSchlick(max(0.0, dot(Lh, Lo)), F0);
    // Calculate normal distribution for specular BRDF.
    float D = distributionGGX(cosLh, roughness);
    // Calculate geometric attenuation for specular BRDF.
    float G = geometrySmith(cosLi, cosLo, roughness);
    // Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
    // Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
    // To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
    vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);
    // Lambert diffuse BRDF.
    // We don't scale by 1/PI for lighting & material units to be more convenient.
    // See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
    vec3 diffuseBRDF = kd * albedo;
    // Cook-Torrance specular microfacet BRDF.
    vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * cosLi * cosLo);
    // Total contribution for this light.
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
  }
```

<div class="page" />

*Shlick*'s *approximation* of the **Fresnel factor**, *Schlick-GGX approximation* of *geometric attenuation* function using *Smith's method* and the **GGX/Towbridge-Reitz normal distribution** function are mathematical formulas described in the reference above.

As to ambient lighting:

```glsl
  // TODO Ambient lighting (IBL).
  vec3 ambientLighting;
  {
    // Sample diffuse irradiance at normal direction.
    // vec3 irradiance = texture(irradianceTexture, N).rgb;
    vec3 irradiance = vec3(1.0);

    // Calculate Fresnel term for ambient lighting.
    // Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
    // use cosLo instead of angle with light's half-vector (cosLh above).
    // See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
    vec3 F = fresnelSchlick(cosLo, F0);

    // Get diffuse contribution factor (as with direct lighting).
    // vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);
    vec3 kd = (1.0 - F) * (1.0 - metalness);

    vec3 diffuseIBL = kd * albedo * irradiance;

    // Total ambient lighting contribution.
    ambientLighting = diffuseIBL * 0.5;
  }
  vec3 minAlbedo = vec3(0.005) * albedo;
  vec3 color = minAlbedo + ambientLighting + directLighting;
```

For now, this abomination will do for our purposes, eventhough no diffuse irradiance is generated from a hdri.

<div class="page" />

Now that we're done calculating color information it is time to apply **gamma correction** and introduce the concept of **exposure**.

```glsl
  // HDR tone mapping
  // color = color / (color + vec3(1.0));
  // Exposure tone mapping
  color = vec3(1.0) - exp(-color * exposure);
  // gamma correction
  color = pow(color, vec3(1.0 / gamma));

  // Final fragment color.
  fColor = vec4(color, 1.0);
```

In order for this to work, the loaded samples must **NOT** be color corrected when loaded. See more in the **Texture** section.

### Textures

Textures are loaded using `stb_image.h` library.

Interesting things to be noted:

1. In order for gamma correction to **NOT** have a diminishing effect, we need to load textures **without** gamma correction. This can be done by specifying the internal image format as `GL_SRGB_ALPHA`.
2. In order for the normalMap to be correcty displayed we need to load it as `GL_RGB`, not `GL_RGBA`. Gamma correction is not a problem.

```cpp
  if (type == "normalMap") {
    GLERR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data) );
  }
  else {
    // color correction and hdr exposure adjustment is done in the shader
    GLERR( glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data) );
  }
```

> GLERR is a macro that checks for GL errors, and then it prints the error message. See `logger.h`

### Remaining Code

The remaining code will not be thoroughly documented as the codeflow can be traced.

### Conclusion

This project, although without **shadow mapping**, has proven success so far. It opened the door to a complex world for me to explore.
After fixing the current issues with this project, I will probably proceed to procedurally generate terrain, with PBR textures applied on them
