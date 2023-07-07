#shader vertex 
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aUVCoord;

uniform mat4 moduleMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVCoord;

void main()
{
    gl_Position = projMat * viewMat * moduleMat * vec4(aPos, 1.0);
    FragPos = (moduleMat * vec4(aPos.xyz, 1.0)).xyz;
    Normal = normalize(mat3(transpose(inverse(moduleMat))) * normal);    
    UVCoord = aUVCoord;
}

#shader fragment
#version 330 core

#define POINT_LIGHT_NUMBER 3
#define SPOT_LIGHT_NUMBER 2

// 材质设置
struct Material
{
    vec3 ambient;
    sampler2D specular;
    sampler2D diffuse;
    float shininess;
};

// 平行光设置
struct LightDirectional
{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
};

// 点光源设置
struct LightPoint
{
    vec3 pos;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

// 聚光灯
struct LightSpot
{
    vec3 pos;
    vec3 dirToLight;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float cosPhiInner;
    float cosPhiOuter;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 UVCoord;

uniform Material material;
uniform LightDirectional lightD;
uniform LightPoint lightP[POINT_LIGHT_NUMBER];
uniform LightSpot lightS[SPOT_LIGHT_NUMBER];

uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;

out vec4 color;

// 计算平行光函数
vec3 CaclLightDirectional(LightDirectional light, vec3 Normal, vec3 dirToCamera)
{
    vec3 object_diffuse = texture(material.diffuse, UVCoord).rgb;
    vec3 object_specular = texture(material.specular, UVCoord).rgb;

    // 环境光

    vec3 ambient = ambientColor * object_diffuse;

    // 漫反射 
    float diffuseIntensity = max(dot(light.dirToLight, Normal), 0);
    vec3 diffuse = diffuseIntensity * light.color * object_diffuse;

    // 镜面反射
    vec3 reflectDir = normalize(reflect(-light.dirToLight, Normal));
    float specularIntensity = pow(max(dot(reflectDir, dirToCamera), 0), material.shininess);
    vec3 specular = specularIntensity * light.color * object_specular;
 
    vec3 result = (specular + diffuse  ) * objectColor;

    return result;
}

// 计算点光源函数
vec3 CaclLightPoint(LightPoint light, vec3 Normal, vec3 dirToCamera)
{
    vec3 object_diffuse = texture(material.diffuse, UVCoord).rgb;
    vec3 object_specular = texture(material.specular, UVCoord).rgb;

    // 衰减值
    float dist = length(light.pos - FragPos);
    float attenuation = 1 / ( light.constant + light.linear * dist + light.quadratic * dist * dist);

    // 环境光
    vec3 ambient = ambientColor * object_diffuse;

    // 漫反射
    vec3 lightDir = normalize(light.pos - FragPos);
    float diffuseIntensity =  max(dot(lightDir, Normal), 0) * attenuation;
    vec3 diffuse = diffuseIntensity * light.color * object_diffuse;

    // 镜面反射  
    vec3 dirToLight = normalize(FragPos - light.pos);
    vec3 reflectDir = normalize(reflect(dirToLight, Normal));
    float specularDistant = pow(max(dot(reflectDir, dirToCamera), 0), material.shininess) * attenuation;
    vec3 specular = specularDistant * light.color * object_specular;

    vec3 result = (diffuse + specular ) * objectColor;
    return result;
}

vec3 CaclLightSpot(LightSpot light, vec3 Normal, vec3 dirToCamera)
{
    vec3 object_diffuse = texture(material.diffuse, UVCoord).rgb;
    vec3 object_specular = texture(material.specular, UVCoord).rgb;

    // 衰减值
    float dist =  length(light.pos - FragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * light.quadratic * dist);   

    float spotIntensity;
    float cosTheta = dot(normalize(light.pos - FragPos), -light.dirToLight);
    if(cosTheta > light.cosPhiOuter)
    {
        spotIntensity = cosTheta;
    }
    else
    {
        spotIntensity = 0.0f;
    }

    // 环境光
    vec3 ambient = ambientColor * object_diffuse;

    // 漫反射
    float diffuseIntensity = max(dot(Normal, normalize(light.pos - FragPos)), 0) * attenuation * spotIntensity;
    vec3 diffuse = diffuseIntensity * light.color * object_diffuse;

    // 镜面反射
    vec3 reflectDir = normalize(reflect(normalize(FragPos - light.pos), Normal));
    float specularIntensity = pow(max(dot(reflectDir, dirToCamera), 0), material.shininess) * attenuation * spotIntensity;
    vec3 specular = specularIntensity * light.color * object_specular;

    vec3 result = (diffuse + specular + ambient * 0.5) * objectColor;
    return result;
}

void main()
{
    vec3 finalResult = vec3(0.0, 0.0, 0.0);   
    vec3 dirToCamera = normalize(cameraPos - FragPos);

    vec3 object_diffuse = texture(material.diffuse, UVCoord).rgb;

    // 计算平行光
    finalResult += CaclLightDirectional(lightD, Normal, dirToCamera);

    // 计算点光源
    for(int i=0; i<POINT_LIGHT_NUMBER; i++)
    {
        finalResult += CaclLightPoint(lightP[i], Normal, dirToCamera);
    }

    // 计算聚光灯
    for(int i=0; i< SPOT_LIGHT_NUMBER; i++)
    {
        finalResult += CaclLightSpot(lightS[i], Normal, dirToCamera);
    }

    color = vec4(finalResult, 1.0);
    // color = vec4(vec3 (gl_FragColor.x), 1.0);
}

