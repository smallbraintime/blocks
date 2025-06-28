#version 430 core

in vec4 ioColor;
in vec3 ioFragPos;
in vec3 ioViewPos;
in vec2 ioTexCoord;
in vec3 ioNormal;
in vec3 ioTangent;
in vec3 ioBitangent;

out vec4 oFragColor;

vec3 lightPos = vec3(0.0, 50.0, 0.0);
vec3 lightColor = vec3(1.0, 0.824, 0.588);
float ambientStrength = 0.2;
float diffuseStrength = 1.0;
float specularStrength = 1.0;
float shininess = 128.0;

uniform sampler2D uNormalMap;

void main() {
    if (ioColor.a == 0.0) discard;

    vec3 normalTex = texture(uNormalMap, ioTexCoord).rgb * 2.0 - 1.0;

    mat3 TBN = mat3(normalize(ioTangent), normalize(ioBitangent), normalize(ioNormal));
    vec3 normal = normalize(TBN * normalTex);

    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - ioFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;

    vec3 viewDir    = normalize(ioViewPos - ioFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // float spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * ioColor.rgb;
    oFragColor = vec4(result, 1.0);
}
