#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_TexCoords;

uniform mat4 u_ModelViewProjection;

void main()
{
   gl_Position = u_ModelViewProjection * position;
   v_TexCoords = texCoords;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;
in vec2 v_TexCoords;

uniform vec4 u_Colour;
uniform sampler2D u_Texture;

void main()
{
	colour = texture(u_Texture, v_TexCoords);
}