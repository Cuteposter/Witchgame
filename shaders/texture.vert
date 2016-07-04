#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform mat4 projmat;
uniform mat4 modelmat;
uniform mat4 orthomat;

uniform bool fbo;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projmat * modelmat * vec4(position, 1.0f);
    ourColor = color;
	if (fbo)
	{
		TexCoord = vec2(texCoord.x, texCoord.y);
	}
	else
	{
		TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
	}
}