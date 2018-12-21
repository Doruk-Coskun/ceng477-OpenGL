#version 410

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoordinate;

// Data from CPU
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{
    // get texture value, compute height
    textureCoordinate = aTexCoordinate;
    vec4 texColor = texture(rgbTexture, textureCoordinate);
    
    float height = heightFactor * (texColor.x * 0.2126 + texColor.y * 0.7152 + texColor.z * 0.0722);
    
    // compute normal vector using also the heights of neighbor vertices
    
    // compute toLight vector vertex coordinate in VCS
    
    // set gl_Position variable correctly to give the transformed vertex position
    //
    gl_Position = MVP * MV * vec4(aPos.x, height, aPos.z, 1.0f); // this is a placeholder. It does not correctly set the position
}
