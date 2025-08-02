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

vec3 lightColor = vec3(1.0, 0.7, 0.5);
float ambientStrength = 0.2;
float diffuseStrength = 3.0;
float specularStrength = 3.0;
float shininess = 64.0;

void main() {
    if (vs_in.color.a == 0.0) discard;

    vec3 normalTex = texture(uNormalMap, vs_in.texCoord).rgb * 2.0 - 1.0;
    mat3 TBN = mat3(normalize(vs_in.tangent), normalize(vs_in.bitangent), normalize(vs_in.normal));
    vec3 normal = normalize(TBN * normalTex);

    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(vs_in.lightPos - vs_in.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;

    vec3 viewDir = normalize(vs_in.cameraPos - vs_in.fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    float shadow;
    vec3 projCoords = vs_in.fragPosLightSpace.xyz / vs_in.fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(uDepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.009 * (1.0 - dot(normal, lightDir)), 0.0009);
    shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(uDepthMap, 0);
    int samples = 0;
    int radius = 6;

    for (int x = -radius; x <= radius; ++x)
    {
        for (int y = -radius; y <= radius; ++y)
        {
            float pcfDepth = texture(uDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            float range = 0.01;
            float shadowFactor = smoothstep(pcfDepth, pcfDepth + range, currentDepth - bias);
            shadow += shadowFactor;
            samples++;
        }
    }
    shadow /= float(samples);

    vec3 I = normalize(vs_in.cameraPos - vs_in.fragPos);
    vec3 R = reflect(-I, normal);

    float cosTheta = max(dot(I, normal), 0.0);
    vec3 F0 = vec3(0.06);
    vec3 fresnelReflect = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

    vec3 reflectedColor = texture(uCubeMap, R).rgb;

    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);
    vec3 baseColor = vs_in.color.rgb * texture(uTexture, vs_in.texCoord).rgb;

    vec3 result = mix(lighting * baseColor, reflectedColor, fresnelReflect);

    result = result / (result + vec3(1.0));
    result = pow(result, vec3(1.0 / 2.2));

    oFragColor = vec4(result, 1.0);
}
