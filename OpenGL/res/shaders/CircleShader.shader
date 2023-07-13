#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_LocalPosition;
layout(location = 2) in vec4 a_Colour;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;

out vec2 v_LocalPosition;
out vec4 v_Colour;
out float v_Thickness;
out float v_Fade;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * a_Position;

	v_LocalPosition = a_LocalPosition;
	v_Colour = a_Colour;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Colour;

in vec2 v_LocalPosition;
in vec4 v_Colour;
in float v_Thickness;
in float v_Fade;


void main()
{
    float dis = 1.0 - length(v_LocalPosition);
    
    float alpha = smoothstep(0.0, v_Fade, dis);
    alpha *= smoothstep(v_Thickness + v_Fade, v_Thickness, dis);
    o_Colour = v_Colour;
	o_Colour.a = alpha;
}