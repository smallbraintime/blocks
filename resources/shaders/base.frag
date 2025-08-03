#version 430 core

in VS_OUT {
    vec4 color;
    vec3 fragPos;
    vec3 cameraPos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 fragPosLightSpace;
    vec3 lightPos;
} vs_in;

out vec4 oFragColor;

layout(binding = 0) uniform sampler2D uNormalMap;
layout(binding = 1) uniform sampler2D uDepthMap;
layout(binding = 2) uniform sampler2D uTexture;
layout(binding = 3) uniform samplerCube uCubeMap;

const vec3 lightColor = vec3(1.0, 0.7, 0.5);
const float ambientStrength = 0.2;
const float diffuseStrength = 3.0;
const float specularStrength = 3.0;
const float shininess = 64.0;

const int PCF_RADIUS = 4;
const float PCF_RANGE = 0.005;

const vec3 FRESNEL_F0 = vec3(0.04);

void pcf(out float shadow, float bias, vec3 projCoords, float currentDepth) {
    vec2 texelSize = 1.0 / textureSize(uDepthMap, 0);
    shadow = 0.0;
    int samples = 0;

    for (int x = -PCF_RADIUS; x <= PCF_RADIUS; ++x) {
        for (int y = -PCF_RADIUS; y <= PCF_RADIUS; ++y) {
            float pcfDepth = texture(uDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            float shadowFactor = smoothstep(pcfDepth, pcfDepth + PCF_RANGE, currentDepth - bias);
            shadow += shadowFactor;
            samples++;
        }
    }
    shadow /= float(samples);
}

vec3 fresnel(vec3 F0, vec3 viewDir, vec3 normal) {
    float cosTheta = max(dot(viewDir, normal), 0.0);
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 gammaCorrection(vec3 color, float gamma) {
    return pow(color, vec3(1.0 / gamma));
}

vec3 reinhard(vec3 color) {
    return color / (1.0f + color);
}

void main() {
    if (vs_in.color.a == 0.0) discard;

    vec3 normalTex = texture(uNormalMap, vs_in.texCoord).rgb * 2.0 - 1.0;
    mat3 TBN = mat3(
        normalize(vs_in.tangent),
        normalize(vs_in.bitangent),
        normalize(vs_in.normal)
    );
    vec3 normal = normalize(TBN * normalTex);

    vec3 lightDir = normalize(vs_in.lightPos - vs_in.fragPos);
    vec3 viewDir = normalize(vs_in.cameraPos - vs_in.fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 projCoords = vs_in.fragPosLightSpace.xyz / vs_in.fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0001);

    float shadow;
    pcf(shadow, bias, projCoords, currentDepth);

    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 reflectDir = reflect(-viewDir, normal);
    vec3 reflectedColor = texture(uCubeMap, reflectDir).rgb;
    vec3 fresnelReflect = fresnel(FRESNEL_F0, viewDir, normal);

    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);
    vec3 baseColor = vs_in.color.rgb * texture(uTexture, vs_in.texCoord).rgb;
    vec3 litColor = lighting * baseColor;

    vec3 result = mix(litColor, reflectedColor, fresnelReflect);

    result = reinhard(result);
    result = gammaCorrection(result, 2.2);

    oFragColor = vec4(result, 1.0);
}
