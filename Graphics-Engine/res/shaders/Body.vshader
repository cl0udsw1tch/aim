#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 BodyTransform;

uniform mat4 BodyView;
uniform mat4 BodyProjection;

void main()
{
	gl_Position = BodyProjection * BodyView * BodyTransform * vec4(aPos, 1.0f);
}