#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;

out vec4 v_Colour;
out vec2 v_TexCoords;
out float v_TexIndex;

uniform mat4 u_ViewProjection;

void main()
{
   gl_Position = u_ViewProjection * a_Position;
   //gl_Position = a_Position;
   v_Colour = a_Colour;
   v_TexCoords = a_TexCoords;
   v_TexIndex = a_TexIndex;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Colour;

in vec4 v_Colour;
in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Texture;

void main()
{
	//int TexIndex = int(v_TextureIndex);
	//o_Colour = v_Colour * texture(u_Texture, v_TexCoords);
	o_Colour = v_Colour;
}