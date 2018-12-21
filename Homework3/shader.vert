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
    
    vertexNormal = vec3(0.0, 1.0, 0.0);
    
    // compute normal vector using also the heights of neighbor vertices
    float heightL, heightR, heightB, heightT;
    vec2 texCorL, texCorR, texCorB, texCorT;
    vec3 posL, posR, posB, posT;
    vec4 colorL, colorR, colorB, colorT;
    if (aPos.x < widthTexture && aPos.x > 0 && aPos.z < heightTexture && aPos.z > 0) {
        posL = aPos - vec3(1, 0, 0);
        texCorL = vec2(posL.x / widthTexture, posL.z / heightTexture);
        colorL = texture(rgbTexture, texCorL);
        heightL = heightFactor * (colorL.x * 0.2126 + colorL.y * 0.7152 + colorL.z * 0.0722);
        
        posR = aPos + vec3(1, 0, 0);
        texCorR = vec2(posR.x / widthTexture, posR.z / heightTexture);
        colorR = texture(rgbTexture, texCorR);
        heightR = heightFactor * (colorR.x * 0.2126 + colorR.y * 0.7152 + colorR.z * 0.0722);
        
        posB = aPos - vec3(0, 0, 1);
        texCorB = vec2(posB.x / widthTexture, posB.z / heightTexture);
        colorB = texture(rgbTexture, texCorB);
        heightB = heightFactor * (colorB.x * 0.2126 + colorB.y * 0.7152 + colorB.z * 0.0722);
        
        posT = aPos + vec3(0, 0, 1);
        texCorT = vec2(posT.x / widthTexture, posT.z / heightTexture);
        colorT = texture(rgbTexture, texCorT);
        heightT = heightFactor * (colorT.x * 0.2126 + colorT.y * 0.7152 + colorT.z * 0.0722);
        
        vertexNormal = normalize(vec3(heightL - heightR, 2.0, heightB - heightT));
    }
    
    // compute toLight vector vertex coordinate in VCS
    ToLightVector = normalize(vec3(widthTexture / 2, widthTexture + heightTexture, heightTexture / 2) - vec3(aPos.x, height, aPos.z));
    
    ToCameraVector = normalize(cameraPosition.xyz - vec3(aPos.x, height, aPos.z));
    
    // set gl_Position variable correctly to give the transformed vertex position
    //
    gl_Position = MVP * MV * vec4(aPos.x, height, aPos.z, 1.0f); // this is a placeholder. It does not correctly set the position
}
