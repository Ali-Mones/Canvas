#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_LocalPosition;
layout(location = 2) in vec4 a_Colour;
layout(location = 3) in float a_Thickness;

out vec2 v_LocalPosition;
out vec4 v_Colour;
out float v_Thickness;

uniform mat4 u_ViewProjection;

void main()
{
   gl_Position = u_ViewProjection * a_Position;
   v_LocalPosition = a_LocalPosition;
   v_Colour = a_Colour;
   v_Thickness = a_Thickness;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Colour;

in vec2 v_LocalPosition;
in vec4 v_Colour;
in float v_Thickness;

void main()
{
	if (v_LocalPosition.x <= -1.0 + v_Thickness || v_LocalPosition.x >= 1.0 - v_Thickness || v_LocalPosition.y <= -1.0 + v_Thickness || v_LocalPosition.y >= 1.0 - v_Thickness)
		o_Colour = v_Colour;
}