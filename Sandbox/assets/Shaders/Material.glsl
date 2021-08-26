#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 m_Camera;
uniform mat4 u_ViewProjection;
uniform mat4 u_RotationMatrix;
uniform mat4 u_TranslationMatrix;
uniform mat4 u_ModelMat;

//uniform vec3 u_LightDir;

out vec3 v_VertexPosition;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	vec4 position = u_TranslationMatrix * u_RotationMatrix * vec4(a_Position, 1.0);
	gl_Position =  u_ViewProjection * position;

	vec4 normal = normalize(u_RotationMatrix * vec4(a_Normal, 1.0));

	v_VertexPosition = vec3(position);
	v_Normal = vec3(normal);
	v_TexCoord = a_TexCoord;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Material 
{
	sampler2D DiffuseTexture;
	sampler2D SpecularTexture;
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	float Shininess;
};

struct Light
{
	vec3 Position;
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
};


in vec3 v_VertexPosition;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform Material material;
uniform Light light;

uniform vec3 u_ViewPosition;

void main()
{

	vec3 lightDirection = normalize(light.Position - v_VertexPosition);
	float diff = max(dot(v_Normal, lightDirection), 0.0);

	vec3 viewDirection = normalize(u_ViewPosition - v_VertexPosition);
	vec3 reflectionDirection = reflect(-lightDirection, v_Normal);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.Shininess);

	//vec3 ambientColor  = light.AmbientColor  * material.AmbientColor;
	//vec3 diffuseColor  = light.DiffuseColor  * (diff * material.DiffuseColor);
	//vec3 specularColor = light.SpecularColor * (spec * material.SpecularColor);
	vec3 diffuseColor  = light.DiffuseColor  * diff * vec3(texture(material.DiffuseTexture, v_TexCoord));
	vec3 ambientColor  = light.AmbientColor  * vec3(texture(material.DiffuseTexture, v_TexCoord));
	vec3 specularColor = light.SpecularColor * spec * vec3(texture(material.SpecularTexture, v_TexCoord));

	color = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}
