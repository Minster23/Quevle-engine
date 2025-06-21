#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuse;
uniform sampler2D normalMap;       // Tidak digunakan saat ini
uniform sampler2D metallicMap;     // Tidak digunakan
uniform sampler2D roughnessMap;    // Tidak digunakan

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float lightIntensity;

void main()
{
    // Albedo color
    vec3 albedo = texture(diffuse, TexCoords).rgb;

    // Lighting vectors
    vec3 norm = normalize(Normal); // <- NORMAL digunakan langsung
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Ambient (selalu kelihatan walau tidak kena cahaya)
    vec3 ambient = 0.1 * albedo;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * albedo;

    // Specular (Phong)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.5) * spec;

    // Total lighting
    vec3 lighting = (ambient + diffuse + specular) * lightIntensity;

    FragColor = vec4(lighting, 1.0);
}
