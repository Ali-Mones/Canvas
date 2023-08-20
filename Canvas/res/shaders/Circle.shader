#shader vertex
#version 330 core

in vec4 a_Position;
in vec2 a_LocalPosition;
in vec4 a_Colour;
in float a_Thickness;
in float a_Fade;

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

out vec4 o_Colour;

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
