#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex;
layout(binding = 2) uniform GlobalUniformBufferObject {
    vec3 lightPos;    // position of the spotlight
    vec3 lightDir;    // direction of the spotlight
    vec4 lightColor;  // color of the spotlight   
    vec3 eyePos;      // position of the viewer
} gubo;

layout(binding = 3) uniform sampler2D texEmit;

const float beta = 2.0f;    // decay exponent of the spotlight
const float g = 3;          // target distance of the spotlight
const float cosout = 0.5;  // cosine of the outer angle of the spotlight
const float cosin = 0.95;  // cosine of the inner angle of the spotlight

const float gamma = 160.0f;  // cosine power for the Blinn specular reflection

// coefficients for the spherical harmonics ambient light term
const vec3 C00 = vec3(0.038, 0.043, 0.045);
const vec3 C1m1 = vec3(0.029, 0.036, 0.041);
const vec3 C10 = vec3(0.004, 0.003, 0.001);
const vec3 C11 = vec3(-0.01, -0.01, -0.009);
const vec3 C2m2 = vec3(-0.006, -0.006, -0.004);
const vec3 C2m1 = vec3(0.001, -0.001, -0.005);
const vec3 C20 = vec3(-0.009, -0.013, -0.015);
const vec3 C21 = vec3(-0.006, -0.005, -0.004);
const vec3 C22 = vec3(0.002, 0.0, -0.005);

void main() {
    vec3 N = normalize(fragNorm);                // surface normal
    vec3 V = normalize(gubo.eyePos - fragPos);   // viewer direction
    vec3 MD = texture(tex, fragUV).rgb;          // diffuse color
    vec3 MA = MD;                                // ambient color
    vec3 MS = vec3(1);                           // specular color
    vec3 ME = texture(texEmit, fragUV).rgb;      // emission color

    // Lambert diffuse lighting
    vec3 L = normalize(gubo.lightPos - fragPos); // light direction
    float lambertTerm = clamp(dot(L, N), 0.0, 1.0);

    // Blinn specular lighting
    vec3 H = normalize(L + V);
    float blinnTerm = clamp(pow(dot(N,H),gamma), 0.0, 1.0);

    // Spotlight attenuation
    vec3 disNorm = normalize(gubo.lightPos - fragPos);
    float attenuation = pow(g / (g + beta * length(disNorm)), beta);
    float spotAngle = dot(L, gubo.lightDir);
    float spotFactor = max((spotAngle - cosout) / (cosin - cosout), 0.0);

    
    // Spherical harmonics ambient lighting
    vec3 ambientSH = C00 * 1.0 +
                     C1m1 * (N.x * 2.0) +
                     C10 * (N.y * 2.0) +
                     C11 * (N.x * N.y * 2.0) +
                     C2m2 * (N.x * N.x - N.y * N.y) +
                     C2m1 * (N.y * N.x * 2.0) +
                     C20 * (N.z * N.z - 0.5) +
                     C21 * (N.y * N.z * 2.0) +
                     C22 * (N.z * N.z - N.x * N.x);

    // Final lighting calculation
    vec3 lighting = (lambertTerm * MD + blinnTerm * MS) * attenuation * spotFactor + (ambientSH * MA);

    outColor = vec4(lighting + ME, 1.0f);    // output color
}
