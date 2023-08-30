#shader vertex
#version 330 core

in vec4 a_Position;
in vec2 a_LocalPosition;
in vec4 a_FillColour;
in vec4 a_StrokeColour;
in float a_ThicknessX;
in float a_ThicknessY;
in vec2 a_TexCoords;
in float a_TexIndex;
in float a_TilingFactor;

out vec2 v_LocalPosition;
out vec4 v_FillColour;
out vec4 v_StrokeColour;
out float v_ThicknessX;
out float v_ThicknessY;
out vec2 v_TexCoords;
out float v_TexIndex;
out float v_TilingFactor;

uniform mat4 u_ViewProjection;

void main()
{
   gl_Position = u_ViewProjection * a_Position;

   v_LocalPosition = a_LocalPosition;
   v_FillColour = a_FillColour;
   v_StrokeColour = a_StrokeColour;
   v_ThicknessX = a_ThicknessX;
   v_ThicknessY = a_ThicknessY;
   v_TexCoords = a_TexCoords;
   v_TexIndex = a_TexIndex;
   v_TilingFactor = a_TilingFactor;
}


#shader fragment
#version 330 core

out vec4 o_Colour;

in vec2 v_LocalPosition;
in vec4 v_FillColour;
in vec4 v_StrokeColour;
in float v_ThicknessX;
in float v_ThicknessY;
in vec2 v_TexCoords;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	int texIndex = int(v_TexIndex);

	float err = 0.000001;
	vec4 texColour = vec4(0.0);
	if (v_LocalPosition.x <= -1.0 + v_ThicknessX + err || v_LocalPosition.x + err >= 1.0 - v_ThicknessX || v_LocalPosition.y <= -1.0 + v_ThicknessY + err || v_LocalPosition.y + err >= 1.0 - v_ThicknessY)
		texColour = v_StrokeColour * texture(u_Textures[texIndex], v_TexCoords * v_TilingFactor);
	else
		texColour = v_FillColour * texture(u_Textures[texIndex], v_TexCoords * v_TilingFactor);

	if (texColour.a == 0.0)
		discard;

	o_Colour = texColour;
}