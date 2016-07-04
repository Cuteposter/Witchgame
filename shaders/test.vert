#version 150

uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec2 ciTexCoord0;

out vec2 vTexCoord0;

void main()
{	
	vTexCoord0 = gl_MultiTextCoord0;
	gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex;
}