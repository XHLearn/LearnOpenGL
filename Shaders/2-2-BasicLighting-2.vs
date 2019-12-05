#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   // 法向量也转换为世界空间坐标 - 使用inverse和transpose函数自己生成这个法线矩阵
   Normal = mat3(transpose(inverse(model))) * aNormal;
}
