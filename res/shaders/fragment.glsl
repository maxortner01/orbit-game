#version 410 core

out vec4 FragColor;  
in vec4 outColor;
  
void main()
{
    FragColor = outColor;
}