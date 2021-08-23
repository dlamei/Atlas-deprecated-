#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_RotationMatrix;
uniform mat4 u_TranslationMatrix;
uniform vec3 u_LightDir;
uniform mat4 u_ModelMat;

out vec4 v_Color;
out vec4 v_Position;

void main()
{
	vec4 position = u_TranslationMatrix * u_RotationMatrix * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * position;

	vec4 normal = u_RotationMatrix * vec4(a_Normal, 1.0);
	float brightness = abs(dot(vec3(normal), u_LightDir));

	v_Color = vec4(vec3(brightness), 1.0) * a_Color;

	v_Position = gl_Position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec4 v_Position;

void main()
{
	color = v_Color;
}
