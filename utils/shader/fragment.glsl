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

// PBR helpers
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

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
    float heightScale = 0.05;
    float height = texture(heightMap, texCoords).r;
    return texCoords - viewDir.xy * (height * heightScale);
}

float computeFakeSSAO(vec3 normal, vec3 fragPos, vec3 viewDir) {
    float facing = clamp(dot(normal, viewDir), 0.0, 1.0);
    float depth = clamp(length(viewDir) / 10.0, 0.0, 1.0);
    return mix(0.4, 1.0, facing) * depth;
}

float computeShadow(vec4 fragPosLightSpace, vec3 N, vec3 L) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) return 1.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);
    return currentDepth - bias > closestDepth ? 0.0 : 1.0;
}

// Material Getters
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

void main() {
    vec3 viewDir = normalize(viewPos - FragPos);
    vec2 newTexCoords = parallaxMapping(TexCoords, viewDir);

    vec3 albedo = getAlbedo(newTexCoords);
    float metallic = getMetallic(newTexCoords);
    float roughness = getRoughness(newTexCoords);
    float opacity = texture(opacityMap, newTexCoords).r * mat_opacity;

    vec3 N = normalize(Normal);
    vec3 V = viewDir;
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lightCount; ++i) {
        vec3 L = normalize(lightPositions[i] - FragPos);
        vec3 H = normalize(V + L);

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 specular = (NDF * G * F) /
                        (4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001);
        specular *= mat_shininessStrength;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        vec3 radiance = lightColors[i] * lightIntensities[i] * NdotL;

        float shadow = computeShadow(FragPosLightSpace, N, L);
        Lo += shadow * ((kD * albedo / 3.14159265359 + specular) * radiance);
    }

    vec3 ambient = getAmbient(newTexCoords, albedo) * getAO(N, FragPos, viewDir);
    vec3 emissive = getEmissive(newTexCoords);

    vec3 color = ambient + Lo + emissive;

    // HDR tonemap & gamma
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

if (selected) {
    float edge = pow(1.0 - max(dot(normalize(viewPos - FragPos), normalize(Normal)), 0.0), 4.0);
    color = mix(color, colorSelected, edge);
}


    FragColor = vec4(color, opacity);
}
