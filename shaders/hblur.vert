// Vertex shader
#version 140
precision highp float;

uniform mat4 ModelviewProjection; // loaded with orthographic projection (-1,-1):(1,1)
uniform mat4 TextureMatrix;       // loaded with identity matrix
in vec3 Position;
in vec2 TexCoord;

out vec4 TextureCoordinates0;

void main(void)
{
    gl_Position = ModelviewProjection * vec4(Position, 1.0);
    TextureCoordinates0 = (TextureMatrix * vec4(TexCoord, 0.0, 0.0));
}