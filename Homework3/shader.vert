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
    float heightL, heightR, heightB, heightT, heightTR, heightBL;
    vec2 texCorL, texCorR, texCorB, texCorT, texCorTR, texCorBL;

    vec3 posL, posR, posB, posT, posTR, posBL;
    vec4 colorL, colorR, colorB, colorT, colorTR, colorBL;

    if (aPos.x < widthTexture && aPos.x > 0 && aPos.z < heightTexture && aPos.z > 0) {
        // Get values for all neighboring vertices
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
        
        posTR = aPos + vec3(1, 0, 1);
        texCorTR = vec2(posTR.x / widthTexture, posTR.z / heightTexture);
        colorTR = texture(rgbTexture, texCorTR);
        heightTR = heightFactor * (colorTR.x * 0.2126 + colorTR.y * 0.7152 + colorTR.z * 0.0722);


        posBL = aPos - vec3(1, 0, 1);
        texCorBL = vec2(posBL.x / widthTexture, posBL.z / heightTexture);
        colorBL = texture(rgbTexture, texCorBL);
        heightBL = heightFactor * (colorBL.x * 0.2126 + colorBL.y * 0.7152 + colorBL.z * 0.0722);

        /* Get normal vectors of adjacent triangles
         * For reference:
         *
         *  .---x---x
         *  |  /|2 /|
         *  | / | / |
         *  |/1 |/ 3|
         *  x---o---x     Where  o -> the current vertex
         *  |4 /|6 /|            x -> adjacent vertices
         *  | / | / |            . -> other vertices
         *  |/ 5|/  |
         *  x---x---.
         */

         vec3 norm1 = cross((posT - aPos), (posL - aPos));
         vec3 norm2 = cross((posTR - aPos), (posT - aPos));
         vec3 norm3 = cross((posR - aPos), (posTR - aPos));
         vec3 norm4 = cross((posB - aPos), (posR - aPos));
         vec3 norm5 = cross((posBL - aPos), (posB - aPos));
         vec3 norm6 = cross((posL - aPos), (posBL - aPos));

         // Get the average of the adjacent triangle normals
         vertexNormal = norm1 + norm2 + norm3 + norm4 + norm5 + norm6;
         vertexNormal = normalize(vec3(vertexNormal.x / 6.0f, vertexNormal.y / 6.0f, vertexNormal.z / 6.0f));

    }
    
    // compute toLight vector vertex coordinate in VCS
    ToLightVector = normalize(vec3(widthTexture / 2, widthTexture + heightTexture, heightTexture / 2) - vec3(aPos.x, height, aPos.z));
    
    ToCameraVector = normalize(cameraPosition.xyz - vec3(aPos.x, height, aPos.z));
    
    // set gl_Position variable correctly to give the transformed vertex position
    gl_Position = MVP * MV * vec4(aPos.x, height, aPos.z, 1.0f); // this is a placeholder. It does not correctly set the position
}
