#version 330 core
in vec3 Normal;     // 法向量
in vec3 FragPos;    // 物体坐标
in vec2 TexCoord;   // 纹理坐标

struct Material {
    sampler2D diffuse;   // 漫反射贴图
    sampler2D specular;  // 镜面光贴图
    float shininess;// 反光度
};

struct Light {  // 聚光灯
    vec3 position;  // 光源坐标
    vec3 direction; // 方向
    float cutoff;    // 切光角
    float outerCutoff; // 外切光角

    vec3 ambient;   // 环境光照强度
    vec3 diffuse;   // 漫反射光照强度
    vec3 specular;  // 镜面光照强度

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;   // 观察者坐标
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    // 环境光照 - 环境光颜色在几乎所有情况下都等于漫反射颜色，所以这里直接使用漫反射材质颜色
    vec3 ambient =vec3(texture(material.diffuse, TexCoord)) * light.ambient;

    // 漫反射
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); // 计算光线方向向量
    float diff = max(0.0, dot(normal, lightDir));  // 点乘计算漫反射强度 = cos(θ)
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoord)) * light.diffuse;   // 漫反射结果

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);    // 计算观察方向向量
    // reflect函数要求第一个向量是从光源指向片段位置的向量
    vec3 reflectDir = reflect(-lightDir,normal);
    // 计算镜面分量:
    // 我们先计算视线方向与反射方向的点乘（并确保它不是负值）
    // 然后取它的32次幂。这个32是高光的反光度(Shininess)。
    // 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, TexCoord)) * light.specular;

    // 计算光线 平滑/软化边缘
    float theta = dot(normalize(light.direction), -lightDir);
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0, 1);
    diffuse *= intensity;
    specular *= intensity;

    // 计算光线衰减
    float dis = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dis + light.quadratic * dis * dis);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // 环境光照 + 漫反射 + 镜面光
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}
