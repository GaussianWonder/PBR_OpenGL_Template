#version 410 core

in vec3 fPosition;
in vec3 fNormal; // face normal, not a from the normal map
in vec2 fTexCoords;
in mat3 fTBN;

out vec4 fColor;

//lighting
// uniform vec3 lightDir;
// uniform vec3 lightColor;
// textures
uniform sampler2D diffuseTexture; // albedo
uniform sampler2D metallicTexture; // channel used for metallic 
uniform sampler2D roughnessTexture; // channel used for roughness
uniform sampler2D normalMap;

// uniform vec3 lightPositions[4];
// uniform vec3 lightColors[4];
in vec3 fLightPosition;
in vec3 fLightColor;
in vec3 fCamPos;

const float PI = 3.14159265359;

uniform float gamma;
uniform float exposure;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float distributionGGX(float NdotH, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
  return a2 / max(PI * denom * denom, 0.00001); //`? division by PI? by 4? by some other constant?
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float geometrySmith(float NdotV, float NdotL, float roughness)
{
  float r = roughness + 1.0;
  float k = (r * r) / 8.0;
  float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
  float ggx2 = NdotL / (NdotL * (1.0 - k) + k);
  return ggx1 * ggx2;
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(float HdotV, vec3 baseReflectivity)
{
  return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - HdotV, 5.0);
}

void main() 
{
  // Sample input textures to get shading model params.
	vec3 albedo = texture(diffuseTexture, fTexCoords).rgb;
  float metalness = texture(metallicTexture, fTexCoords).r;
	float roughness = texture(roughnessTexture, fTexCoords).r;

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(fCamPos - fPosition);

	// Get current fragment's normal and transform to world space.
	vec3 N = normalize(2.0 * texture(normalMap, fTexCoords).rgb - 1.0);
	N = normalize(fTBN * N);
	
	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));
		
	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	vec3 F0 = mix(vec3(0.04), albedo, metalness);

	// Direct lighting calculation for analytical lights.
	vec3 directLighting = vec3(0);
	for(int i=0; i<1; ++i)
	{
		vec3 Li = -normalize(fLightPosition - fPosition);
    float dst = length(fLightPosition - fPosition);
    float attenuation = 1.0 / (dst * dst);
		vec3 Lradiance = fLightColor * attenuation;

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
  vec3 minAlbedo = vec3(0.0025) * albedo;
  vec3 color = minAlbedo + ambientLighting + directLighting;

  // HDR tone mapping
  // color = color / (color + vec3(1.0));
  // Exposure tone mapping
  color = vec3(1.0) - exp(-color * exposure);
  // gamma correction
  color = pow(color, vec3(1.0 / gamma));

	// Final fragment color.
	fColor = vec4(color, 1.0);
}
