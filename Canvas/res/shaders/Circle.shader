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

void main()
{
    float dis = length(v_LocalPosition);
	float fwd = fwidth(dis);
    float alpha1 = smoothstep(0.0, 2.0 * fwd, 1.0 - dis);
	alpha1 *= smoothstep(v_Thickness - fwd, v_Thickness + fwd, dis);

	float alpha2 = 1.0 - smoothstep(v_Thickness - fwd, v_Thickness + fwd, dis);

	if (dis >= v_Thickness - fwd && dis <= 1.0)
		o_Colour = vec4(v_StrokeColour.xyz, alpha1 * v_StrokeColour.w);

	if (v_FillColour != vec4(0, 0, 0, 0))
	{
		if (dis <= v_Thickness + fwd && dis >= v_Thickness - fwd)
			o_Colour = mix(vec4(v_FillColour.xyz, alpha2 * v_FillColour.w), v_StrokeColour, alpha1);
		else if (dis <= v_Thickness - fwd)
			o_Colour = v_FillColour;
	}
}
