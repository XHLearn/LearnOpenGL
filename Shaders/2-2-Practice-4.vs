#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;  // 光源坐标
uniform vec3 lightColor;  // 光源颜色
uniform vec3 viewPos;   // 观察者坐标

out vec3 LightingColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    // 法向量也转换为世界空间坐标 - 使用inverse和transpose函数自己生成这个法线矩阵
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

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

    // (环境光照 + 漫反射 + 高光) * 物体颜色
    LightingColor = ambient + diffuse + specular;
}
