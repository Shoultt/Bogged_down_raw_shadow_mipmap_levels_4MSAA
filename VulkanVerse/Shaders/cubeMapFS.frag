#version 450

layout(location = 0) in vec2 texCoord;

layout(set = 0, binding = 4) uniform sampler2D texSampler;
layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture(texSampler, texCoord);
}