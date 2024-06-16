#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 texCoord;

layout(set = 0, binding = 0) uniform UBO{
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 model1;
	mat4 view1;
	mat4 lightSpaceMat;
	vec3 lightPos;
}ubo;

void main()
{
	texCoord = inTexCoord;
	vec4 vPosition = vec4(inPos, 1.0f);
	gl_Position = vec4(ubo.proj * ubo.view1 * ubo.model1 * (vec4(vPosition)));
}