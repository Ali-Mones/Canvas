#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TextureCoords;
layout(location = 3) in float a_TextureIndex;

out vec4 v_Colour;
out vec2 v_TexCoords;
out float v_TextureIndex;

//uniform mat4 u_ModelViewProjection;

void main()
{
   //gl_Position = u_ModelViewProjection * a_Position;
   gl_Position = a_Position;
   v_Colour = a_Colour;
   v_TexCoords = a_TextureCoords;
   v_TextureIndex = a_TextureIndex;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_Colour;
in vec2 v_TexCoords;
in float v_TextureIndex;

uniform sampler2D u_Textures[2];

void main()
{
	int TexIndex = int(v_TextureIndex);
	//colour = texture(u_Textures[TexIndex], v_TexCoords);
	colour = v_Colour;
}