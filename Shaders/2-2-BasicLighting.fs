#version 330 core
in vec3 Normal;     // 法向量
in vec3 FragPos;    // 物体坐标

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

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

    // (环境光照 + 漫反射) * 物体颜色
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0f);
}
