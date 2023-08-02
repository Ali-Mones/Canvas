#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;

out vec4 v_Colour;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * a_Position;
	v_Colour = a_Colour;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Colour;

in vec4 v_Colour;

void main()
{
    o_Colour = v_Colour;
}