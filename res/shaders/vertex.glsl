#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 outColor;

void main()
{
    vec4 pos = projection * view * model * vec4(position, 1.0);
    gl_Position = pos;
    outColor = color;
    outColor = color * max(0, dot(normal, normalize(vec3(1, 2, 1.5))));
    outColor.w = 1.0;
}