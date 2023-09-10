#shader vertex
#version 410 core

in vec4 a_Position;
in vec4 a_Colour;
in vec2 a_TexCoords;

out vec4 v_Colour;
out vec2 v_TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
   gl_Position = u_ViewProjection * a_Position;

   v_Colour = a_Colour;
   v_TexCoords = a_TexCoords;
}


#shader fragment
#version 410 core

out vec4 o_Colour;

in vec4 v_Colour;
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColour = v_Colour * texture(u_Texture, vec2(v_TexCoords.x, 1.0f - v_TexCoords.y)).r;
	o_Colour = texColour;
}
