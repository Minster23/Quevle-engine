#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D heightMap;

uniform sampler2D ambientMap;
uniform sampler2D emissiveMap;
uniform sampler2D opacityMap;
uniform sampler2D displacementMap;
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform int lightCount;
uniform vec3 lightPositions[16];
uniform float lightIntensities[16];
uniform vec3 lightColors[16];

uniform bool selected;
uniform vec3 colorSelected;

// Material struct data (FBX/MTL)
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform vec3 mat_emissive;
uniform float mat_shininess;
uniform float mat_opacity;
uniform float mat_shininessStrength;

// New uniforms for additional features
uniform float u_exposure = 1.0;       // HDR exposure control
uniform float u_gamma = 2.2;          // Gamma correction
uniform float u_iblStrength = 0.3;    // Image-based lighting strength
uniform vec3 u_skyColor = vec3(0.5, 0.7, 1.0); // Sky color for ambient
uniform vec3 u_groundColor = vec3(0.2, 0.2, 0.2); // Ground color for ambient

// PBR functions
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265359 * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * 0.05; 
    vec2 deltaTexCoords = P / numLayers;
    
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(heightMap, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(heightMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }
    
    return currentTexCoords;
}

float computeFakeSSAO(vec3 normal, vec3 fragPos, vec3 viewDir) {
    float facing = clamp(dot(normal, viewDir), 0.0, 1.0);
    float depth = clamp(length(viewDir) / 10.0, 0.0, 1.0);
    return mix(0.4, 1.0, facing) * depth;
}

float computeShadow(vec4 fragPosLightSpace, vec3 N, vec3 L) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0) return 1.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);
    
    // PCF for softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;
        }
    }
    shadow /= 9.0;
    
    return shadow;
}

// Material getters
vec3 getAlbedo(vec2 uv) {
    return texture(diffuse, uv).rgb * mat_diffuse;
}

float getMetallic(vec2 uv) {
    return clamp(texture(metallicMap, uv).r, 0.0, 1.0);
}

float getRoughness(vec2 uv) {
    return clamp(texture(roughnessMap, uv).r, 0.04, 1.0);
}

vec3 getEmissive(vec2 uv) {
    return texture(emissiveMap, uv).rgb + mat_emissive;
}

vec3 getAmbient(vec2 uv, vec3 albedo) {
    return (vec3(0.03) * albedo + texture(ambientMap, uv).rgb * mat_ambient);
}

float getAO(vec3 normal, vec3 fragPos, vec3 viewDir) {
    return clamp(computeFakeSSAO(normal, fragPos, viewDir), 0.0, 1.0);
}

vec3 calculateNormal(vec2 uv) {
    vec3 tangentNormal = texture(normalMap, uv).xyz * 2.0 - 1.0;
    
    vec3 Q1 = dFdx(FragPos);
    vec3 Q2 = dFdy(FragPos);
    vec2 st1 = dFdx(uv);
    vec2 st2 = dFdy(uv);
    
    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * tangentNormal);
}

vec3 tonemapACES(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
    // View direction and parallax mapping
    vec3 viewDir = normalize(viewPos - FragPos);
    vec2 newTexCoords = parallaxMapping(TexCoords, viewDir);
    
    // Material properties
    vec3 albedo = getAlbedo(newTexCoords);
    float metallic = getMetallic(newTexCoords);
    float roughness = getRoughness(newTexCoords);
    float opacity = texture(opacityMap, newTexCoords).r * mat_opacity;
    
    // Normal mapping
    vec3 N = calculateNormal(newTexCoords);
    vec3 V = viewDir;
    vec3 R = reflect(-V, N);
    
    // Fresnel reflectance at normal incidence
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    
    // Reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lightCount; ++i) {
        // Calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * lightIntensities[i] * attenuation;
        
        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;
        
        // Energy conservation
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        
        // Scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);
        
        // Compute shadow with PCF
        float shadow = computeShadow(FragPosLightSpace, N, L);
        
        // Add to outgoing radiance Lo
        Lo += shadow * (kD * albedo / 3.14159265359 + specular) * radiance * NdotL;
    }
    
    // Ambient lighting (IBL approximation)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    
    // Hemisphere lighting approximation
    vec3 ambient = (kD * mix(u_groundColor, u_skyColor, (N.y * 0.5 + 0.5)) * albedo) * u_iblStrength;
    ambient += getAmbient(newTexCoords, albedo) * getAO(N, FragPos, viewDir);
    
    // Emissive lighting
    vec3 emissive = getEmissive(newTexCoords);
    
    // Combine all lighting
    vec3 color = ambient + Lo + emissive;
    
    // HDR and tonemapping
    color *= u_exposure;
    color = tonemapACES(color);
    
    // Gamma correction
    color = pow(color, vec3(1.0/u_gamma));
    
    // Selection highlight
    if(selected) {
        float edge = pow(1.0 - max(dot(normalize(viewPos - FragPos), normalize(Normal)), 0.0), 4.0);
        color = mix(color, colorSelected, edge);
    }
    
    FragColor = vec4(color, opacity);
}