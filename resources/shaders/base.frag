#version 430 core

in vec4 ioColor;
in vec3 ioFragPos;
in vec3 ioViewPos;
in vec2 ioTexCoord;
in vec3 ioNormal;
in vec3 ioTangent;
in vec3 ioBitangent;
in vec4 ioFragPosLightSpace;
in vec3 ioLightPos;

out vec4 oFragColor;

vec3 lightPos = ioLightPos;
vec3 lightColor = vec3(1.0, 0.7, 0.5);
float ambientStrength = 0.3;
float diffuseStrength = 2.0;
float specularStrength = 2.5;
float shininess = 512.0;

uniform sampler2D uNormalMap;
uniform sampler2D uDepthMap;
uniform sampler2D uTexture;
uniform samplerCube uCubeMap;

float fresnelSchlick(float cosTheta, float F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


void main() {
    if (ioColor.a == 0.0) discard;

    vec3 normalTex = texture(uNormalMap, ioTexCoord).rgb * 2.0 - 1.0;

    mat3 TBN = mat3(normalize(ioTangent), normalize(ioBitangent), normalize(ioNormal));
    vec3 normal = normalize(TBN * normalTex);

    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - ioFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;

    vec3 viewDir = normalize(ioViewPos - ioFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    float rim = 1.0 - max(dot(viewDir, normal), 0.0);
    rim = pow(rim, 2.5);
    vec3 rimColor = vec3(0.3, 0.4, 0.6) * rim * 0.5;

    float shadow;
    vec3 projCoords = ioFragPosLightSpace.xyz / ioFragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        shadow = 0.0;

    float closestDepth = texture(uDepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);
    shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(uDepthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);
    vec3 baseColor = ioColor.rgb * texture(uTexture, ioTexCoord).rgb;
    vec3 result = lighting * baseColor;

    vec3 I = normalize(ioFragPos - ioViewPos);
    vec3 R = reflect(I, normalize(ioNormal));

    float F0 = 0.03;
    float fresnel = fresnelSchlick(max(dot(viewDir, normal), 0.0), F0);
    result = mix(result, texture(uCubeMap, R).rgb, fresnel);

    vec3 sssColor = vec3(0.6, 0.7, 1.0);
    result += rim * sssColor * 0.3;

    result = result / (result + vec3(1.0));
    result = pow(result, vec3(1.0 / 2.2));
    oFragColor = vec4(result, 1.0);
}
