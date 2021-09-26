#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_Position, 0.0, 1.0);
	v_TexCoord = a_TexCoord; 
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

#extension GL_EXT_gpu_shader4 : require

in vec2 v_TexCoord;

uniform isampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord) / 3.0 + 0.5;
}
