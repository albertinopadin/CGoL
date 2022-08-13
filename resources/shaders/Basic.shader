#shader vertex
#version 410 core

layout(location = 0) in vec3 position;

void main()
{
   gl_Position = vec4(position, 1.0f);
}


#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform float u_Alpha;

void main()
{
   color = vec4(u_Color.rgb, u_Alpha);
}