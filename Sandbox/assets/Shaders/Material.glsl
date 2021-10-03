#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_TransformMatrix;

out VS_OUT
{
	vec3 FragPosition;
	vec3 Normal;
	vec2 TexCoord;
} vsOut;

void main()
{
	vec4 position = u_TransformMatrix * vec4(a_Position, 1.0);
	gl_Position =  u_ViewProjection * position;

	vec4 normal = normalize(transpose(inverse(u_TransformMatrix)) * vec4(a_Normal, 1.0));

	//v_FragPosition = vec3(position);
	//v_Normal = vec3(normal);
	//v_TexCoord = a_TexCoord;
	vsOut.FragPosition = vec3(position);
	vsOut.Normal = vec3(normal);
	vsOut.TexCoord = a_TexCoord;
}

#type geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec3 FragPosition;
	vec3 Normal;
	vec2 TexCoord;
} gsIn[];

out GS_OUT
{
	vec3 FragPosition;
	vec3 Normal;
	vec2 TexCoord;
} gsOut;

void main() {
  for(int i = 0; i < 3; i++) 
  { 
    gl_Position = gl_in[i].gl_Position;
	gsOut.FragPosition = gsIn[i].FragPosition;
	gsOut.Normal = gsIn[i].Normal;
	gsOut.TexCoord = gsIn[i].TexCoord;
    EmitVertex();
  }

  EndPrimitive();
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in GS_OUT
{
	vec3 FragPosition;
	vec3 Normal;
	vec2 TexCoord;
};

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


uniform Material material;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirLight dirLights[MAX_DIR_LIGHTS];

uniform vec3 u_ViewPosition;

uniform int u_PointLightCount;
uniform int u_DirLightCount;

uniform int u_ID = -1;

uniform samplerCube u_SkyBoxTexture;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	color2 = u_ID;
	//vec3 I = normalize(FragPosition - u_ViewPosition);
    //vec3 R = reflect(I, normalize(Normal));
    //color = vec4(texture(u_SkyBoxTexture, R).rgb, 1.0);

	//TODO: optimization
	vec3 viewDirection = normalize(u_ViewPosition - FragPosition);

	vec3 result = vec3(0.0);

	for (int i = 0; i < u_DirLightCount; i++)
	{
		result += CalcDirLight(dirLights[i], Normal, viewDirection);
	}

	for (int i = 0; i < u_PointLightCount; i++)
	{
		result += CalcPointLight(pointLights[i], Normal, FragPosition, viewDirection);
	}

	color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

    vec3 ambient  = light.Ambient  * vec3(texture(material.DiffuseTexture, TexCoord));
    vec3 diffuse  = light.Diffuse  * diff * vec3(texture(material.DiffuseTexture, TexCoord));
    vec3 specular = light.Specular * spec * vec3(texture(material.SpecularTexture, TexCoord));
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

	vec3 Ambient = light.Ambient * vec3(texture(material.DiffuseTexture, TexCoord));
	vec3 Diffuse = light.Diffuse * diffuseStrength * vec3(texture(material.DiffuseTexture, TexCoord));
	vec3 Specular = light.Specular * specularStrength * vec3(texture(material.SpecularTexture, TexCoord));
	Ambient *= attenuation;
	Diffuse *= attenuation;
	Specular *= attenuation;

	return (Ambient + Diffuse + Specular);
};
