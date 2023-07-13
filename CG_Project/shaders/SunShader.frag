#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;


layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;


void main() {
	
	//Ambient Lighting
	vec3 light_Color = vec3(1, 1, 1);
	const float AmbStr = 0.2;
	vec3 Amblight = AmbStr * light_Color;

	//Diffsed Lighting
	const vec3 SunPos = vec3(0,0,0);	
	const vec3 light_Dir = normalize(SunPos - fragPos);
	const float DiffStr = max(dot(fragNorm,light_Dir), 1);
	const vec3 DiffLight = DiffStr * light_Color.rgb;

	//Specular Lighting
	const float SpecStr = 3;
	const vec3 viewDir = normalize(gubo.eyePos - fragPos);
	//const vec3 reflectDirection = reflect(-light_Dir, fragNorm);
	//const float SpecPow = pow(max(dot(reflectDirection, viewDir), 1.0), 10);
	const vec3 SpecLight = SpecStr * light_Color.rgb;
	
	outColor = vec4(SpecLight + Amblight + DiffLight, 1.0f) * texture(tex, fragUV);
}