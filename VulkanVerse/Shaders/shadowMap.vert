#version 450

layout(location = 0) in vec3 inPos;

layout(push_constant) uniform constant{
	mat4 model;
	mat4 lightSpaceMat;
}pushConstant;
 
 
void main()
{
	vec4 pos = vec4(inPos, 1.0);
	vec4 worldPos = pushConstant.model * vec4(inPos, 1.0f);
	gl_Position = pushConstant.lightSpaceMat * pushConstant.model * worldPos;
}