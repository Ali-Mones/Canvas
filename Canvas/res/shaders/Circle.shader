#shader vertex
#version 330 core

in vec4 a_Position;
in vec2 a_LocalPosition;
in vec4 a_FillColour;
in vec4 a_StrokeColour;
in float a_Thickness;

out vec2 v_LocalPosition;
out vec4 v_FillColour;
out vec4 v_StrokeColour;
out float v_Thickness;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * a_Position;

	v_LocalPosition = a_LocalPosition;
	v_FillColour = a_FillColour;
	v_StrokeColour = a_StrokeColour;
	v_Thickness = a_Thickness;
}


#shader fragment
#version 330 core

out vec4 o_Colour;

in vec2 v_LocalPosition;
in vec4 v_FillColour;
in vec4 v_StrokeColour;
in float v_Thickness;

float circle(vec2 pos, float radius)
{
	return length(pos) - radius;
}

void main()
{
	float c = circle(v_LocalPosition, 1.0);
	float fwd = fwidth(c);

	float s = smoothstep(-fwd * 2.0, 0.0, c);
	float s2 = smoothstep(-v_Thickness - fwd, -v_Thickness + fwd, c);

	vec4 color = mix(v_FillColour, v_StrokeColour, s2);
	float a = mix(color.a, 0.0, s);
	color.a = a;
	o_Colour = color;
} 
