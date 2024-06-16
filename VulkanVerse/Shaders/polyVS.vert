#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 1) out vec3 lightIntensity;
layout(location = 2) out vec2 fragTexCoord;

layout(set = 0, binding = 3) uniform sampler2D shadowMap;

layout (set = 0, binding = 0) uniform UBO0
{
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 model1;
	mat4 view1;
	mat4 lightSpaceMat;
	vec3 lightPos;
}ubo0;

float shadowFactor_compute(vec4 light_space_pos, sampler2D shadow_map)
{
   vec3 light_space_ndc = light_space_pos.xyz /= light_space_pos.w;

   if (abs(light_space_ndc.x) > 1.0 ||
       abs(light_space_ndc.y) > 1.0 ||
       abs(light_space_ndc.z) > 1.0)
      return 0.0;
 
   vec2 shadow_map_coord = light_space_ndc.xy * 0.5 + 0.5;
 
   if (light_space_ndc.z > texture(shadow_map, shadow_map_coord.xy).x)
      return 0.0;
 
   return 1.0;
}

void main()
{
	vec4 vPosition = vec4(inPos, 1.0f) * vec4(1.0f, -1.0f, 1.0f, 1.0f) - vec4(90.0f, 80.0f, 90.0f, 0.0f);
	vec4 worldPos = ubo0.model * vec4(inPos, 1.0f);
	vec3 fragNormal = mat3(ubo0.model) * inNormal;
	vec3 lightDir = vec3(ubo0.lightSpaceMat * vec4(1.0f));

	vec3 N = normalize(fragNormal);

	vec3 ambient = vec3(1.0f, 0.96f, 0.80f) * 0.15f;
	vec3 diffuse = max(dot(N, lightDir), 0.0f) * vec3(1.0f, 0.96f, 0.80f) * 0.05f;

	vec4 lightSpace = ubo0.lightSpaceMat * worldPos;
	
	float shadow = shadowFactor_compute(lightSpace, shadowMap);
	lightIntensity = vec3(ambient + diffuse) + shadow;
	fragTexCoord = inTexCoord;
	

	gl_Position = ubo0.proj * ubo0.view * ubo0.model * vPosition;
}