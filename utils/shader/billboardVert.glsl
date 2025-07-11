#version 330 core
layout (location = 0) in vec3 aPos;       // Quad corner position (e.g., [-0.5, -0.5, 0.0])
layout (location = 1) in vec2 aTexCoords; // Texture coordinates

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 billboardPos;  // Position of the billboard in world space
uniform vec3 cameraPos;     // Camera position in world space
uniform bool LookAt;

void main()
{
    // Calculate billboard's local right and up vectors
    vec3 right;
    vec3 up;
    if (LookAt) {
        vec3 lookDir = normalize(cameraPos - billboardPos);
        up = vec3(0.0, 1.0, 0.0); // World up vector
        right = normalize(cross(up, lookDir));
        up = normalize(cross(lookDir, right));
    } else {
        // If not looking at, assume a default orientation (e.g., aligned with world X and Y)
        right = vec3(1.0, 0.0, 0.0);
        up = vec3(0.0, 1.0, 0.0);
    }
    
    // Compute world position
    vec3 worldPos = billboardPos + right * aPos.x + up * aPos.y;
    
    // Output clip space position
    gl_Position = projection * view * vec4(worldPos, 1.0);

    // Pass UV to fragment shader
    TexCoords = aTexCoords;
}
