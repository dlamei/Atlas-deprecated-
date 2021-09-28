#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_TransformMatrix;
uniform float u_Thickness = 0.05;

void main()
{
	//vec4 normal = normalize(transpose(inverse(u_TransformMatrix)) * vec4(a_Normal, 1.0));
	vec3 pos = a_Position + a_Normal * u_Thickness;
	vec4 position = u_TransformMatrix * vec4(pos, 1.0);

	gl_Position =  u_ViewProjection * position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_OutlineColor;

void main()
{
	color = u_OutlineColor;
}

