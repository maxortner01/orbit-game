#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 projection;
uniform mat4 view;

out vec4 outColor;

void main()
{
    vec4 pos = projection * view * vec4(position, 1.0);
    gl_Position = pos;
    outColor = color;
}