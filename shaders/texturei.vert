#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec2 scale;	//x is width, y is height

uniform mat4 projmat;
uniform mat4 modelmat;
uniform mat4 orthomat;

out vec3 ourColor;
out vec2 TexCoord;

uniform vec2 offsets[100];

void main()
{
	vec2 offset = offsets[gl_InstanceID];
	vec4 real_position = vec4((position.x + offset.x), (position.y + offset.y), position.z, 1.0f);
    gl_Position = orthomat * real_position;
    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0f - texCoord.y) / scale;
}