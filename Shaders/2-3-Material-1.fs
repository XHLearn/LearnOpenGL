#version 330 core
in vec3 Normal;     // 法向量
in vec3 FragPos;    // 物体坐标

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  // 光源坐标
uniform vec3 viewPos;   // 观察者坐标

void main()
{
    // 环境光照
    float ambientStrength = 0.1;    // 强度
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // 计算光线方向向量
    float diff = max(0.0, dot(normal, lightDir));  // 点乘计算漫反射强度 = cos(θ)
    vec3 diffuse = diff * lightColor;   // 漫反射结果

    // 高光
    float specularStrength = 0.5;   // 高光强度
    vec3 viewDir = normalize(viewPos - FragPos);    // 计算观察方向向量
    // reflect函数要求第一个向量是从光源指向片段位置的向量
    vec3 reflectDir = reflect(-lightDir,normal);
    // 计算镜面分量:
    // 我们先计算视线方向与反射方向的点乘（并确保它不是负值）
    // 然后取它的32次幂。这个32是高光的反光度(Shininess)。
    // 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    float spec = pow(max(dot(reflectDir, viewDir), 0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // (环境光照 + 漫反射) * 物体颜色
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}
