#version 330 core
out vec4 FragColor;

in vec3 FragPos;//世界空间坐标
in vec3 Normal;//法向量
in vec2 TexCoords;//纹理

//单向光源
struct DirLight {
    vec3 direction;//单向光方向

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//点光源
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4


//聚光灯
struct SpotLight{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

//材质结构体
struct Material{
    
    sampler2D diffuse;//漫反射系数
    sampler2D specular;//镜面反射强度
    float shininess;//反光度，如32
};


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//计算定向光影响
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//计算点光源影响
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//计算聚光源影响

uniform float mixValue;
uniform vec3 viewPos;//相机位置
uniform vec3 lightColor;

uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];//点光源数组
uniform Material material;//材质对象

void main()
{
    
    //归一化向量
    vec3 norm = normalize(Normal);//法向量标准化
    vec3 viewDir = normalize(viewPos - FragPos);//视线方向
 
    //计算定向光影响
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    //计算点光源影响
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    
    //计算聚光影响
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

   //着色结果(点光源+单项光+聚光)
    FragColor = vec4(result,1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
   // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
   // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
     
     vec3 lightDir = normalize(light.position - FragPos);
     float diff = max(dot(norm, lightDir), 0.0);
     
     // specular
     vec3 reflectDir = reflect(-lightDir, norm);  
     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

     // attenuation
     float distance    = length(light.position - FragPos);
     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
     // spotlight (soft edges)
     float theta = dot(lightDir, normalize(-light.direction)); 
     float epsilon = (light.cutOff - light.outerCutOff);
     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

     vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
     vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

     return (ambient + diffuse + specular);
}