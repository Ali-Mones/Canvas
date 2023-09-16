#shader vertex
#version 410 core

in vec4 a_Position;
in vec4 a_Colour;
in vec2 a_TexCoords;
in float a_TexIndex;

out vec4 v_Colour;
out vec2 v_TexCoords;
out float v_TexIndex;

uniform mat4 u_ViewProjection;

void main()
{
   gl_Position = u_ViewProjection * a_Position;

   v_Colour = a_Colour;
   v_TexCoords = a_TexCoords;
   v_TexIndex = a_TexIndex;
}


#shader fragment
#version 410 core

out vec4 o_Colour;

in vec4 v_Colour;
in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

float screenPxRange()
{
	int texIndex = int(v_TexIndex);
	const float pxRange = 2.0; // set to distance field's pixel range
	vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_Textures[texIndex], 0));
	vec2 screenTexSize = vec2(1.0) / fwidth(v_TexCoords);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	int texIndex = int(v_TexIndex);
	vec3 msd = texture(u_Textures[texIndex], v_TexCoords).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	o_Colour = mix(vec4(0.0), v_Colour, opacity);
}
