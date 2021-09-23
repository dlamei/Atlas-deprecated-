#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 m_Camera;
uniform mat4 u_ViewProjection;
uniform mat4 u_TransformMatrix;
uniform mat4 u_ModelMat;

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	vec4 position = u_TransformMatrix * vec4(a_Position, 1.0);
	gl_Position =  u_ViewProjection * position;

	vec4 normal = normalize(transpose(inverse(u_TransformMatrix)) * vec4(a_Normal, 1.0));

	v_FragPosition = vec3(position);
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

struct DirLight
{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct PointLight
{
	vec3 Position;

	float Constant;
	float Linear;
	float Quadratic;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

#define MAX_POINT_LIGHTS 16
#define MAX_DIR_LIGHTS 4


in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform Material material;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirLight dirLights[MAX_DIR_LIGHTS];

uniform vec3 u_ViewPosition;

uniform int u_PointLightCount;
uniform int u_DirLightCount;

uniform int u_DisplayMode = 0;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	if (u_DisplayMode == 1)
	{
		color = vec4(v_Normal, 1.0);
		return;
	}

	vec3 viewDirection = normalize(u_ViewPosition - v_FragPosition);

	vec3 result = vec3(0.0);

	for (int i = 0; i < u_DirLightCount; i++)
	{
		result += CalcDirLight(dirLights[i], v_Normal, viewDirection);
	}

	for (int i = 0; i < u_PointLightCount; i++)
	{
		result += CalcPointLight(pointLights[i], v_Normal, v_FragPosition, viewDirection);
	}

	color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

    vec3 ambient  = light.Ambient  * vec3(texture(material.DiffuseTexture, v_TexCoord));
    vec3 diffuse  = light.Diffuse  * diff * vec3(texture(material.DiffuseTexture, v_TexCoord));
    vec3 specular = light.Specular * spec * vec3(texture(material.SpecularTexture, v_TexCoord));
    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 lightDir = normalize(light.Position - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	float specularStrength = pow(max(dot(viewDirection, reflectDir), 0.0), material.Shininess);

	float dist = length(light.Position - fragPosition);
	float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));

	vec3 Ambient = light.Ambient * vec3(texture(material.DiffuseTexture, v_TexCoord));
	vec3 Diffuse = light.Diffuse * diffuseStrength * vec3(texture(material.DiffuseTexture, v_TexCoord));
	vec3 Specular = light.Specular * specularStrength * vec3(texture(material.SpecularTexture, v_TexCoord));
	Ambient *= attenuation;
	Diffuse *= attenuation;
	Specular *= attenuation;

	return (Ambient + Diffuse + Specular);
};
