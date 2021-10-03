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

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform float u_Gamma = 2.2;

void main()
{
	vec4 fragColor = texture(u_Texture, v_TexCoord);
    color.rgb = pow(fragColor.rgb, vec3(1.0/u_Gamma));
	color.a = 1.0;
}
