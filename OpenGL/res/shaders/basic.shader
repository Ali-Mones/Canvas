#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

uniform vec4 u_Color;

void main()
{
	colour = u_Color;
}