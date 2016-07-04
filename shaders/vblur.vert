#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec2 blurTextureCoords[11];

uniform float targetHeight;
uniform mat4 projmat;
uniform mat4 modelmat;
uniform mat4 orthomat;

void main(void){
	gl_Position = vec4(position.x-320, 1.0-position.y+240, 0.0, 1.0);
	vec2 centerTexCoords = vec2(position.x, 1.0-position.y) * 0.5 + 0.5;
	float pixelSize = 1.0 / targetHeight;
	
	for(int i=-5;i<=5;i++)
	{
		blurTextureCoords[i+5] = centerTexCoords + vec2(0.0, pixelSize * i);
	}
}