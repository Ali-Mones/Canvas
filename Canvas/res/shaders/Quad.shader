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

out vec2 v_LocalPosition;
out vec4 v_FillColour;
out vec4 v_StrokeColour;
out float v_ThicknessX;
out float v_ThicknessY;
out vec2 v_TexCoords;
out float v_TexIndex;

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

uniform sampler2D u_Texture;

void main()
{
	//int TexIndex = int(v_TextureIndex);
	//o_Colour = v_Colour * texture(u_Texture, v_TexCoords);

	float err = 0.000001;

	if (v_LocalPosition.x <= -1.0 + v_ThicknessX + err || v_LocalPosition.x + err >= 1.0 - v_ThicknessX || v_LocalPosition.y <= -1.0 + v_ThicknessY + err || v_LocalPosition.y + err >= 1.0 - v_ThicknessY)
		o_Colour = v_StrokeColour;
	else
		o_Colour = v_FillColour;
}