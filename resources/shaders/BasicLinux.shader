#shader vertex
#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vColor;

uniform mat4 u_MVP;  // Model View Projection matrix

out vec4 fColor;

void main()
{
   gl_Position = u_MVP * vec4(position, 1.0f);
   fColor = vColor;
}


#shader fragment
#version 400 core

in vec4 fColor;

layout(location = 0) out vec4 color;

void main()
{
   color = fColor;
}