#include <string>

namespace Atlas
{
	namespace DefaultShaders
	{
		static std::string FlatShader = R"(
			#type vertex
			#version 330 core
			
			layout(location = 0) in vec2 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;
			layout(location = 3) in float a_TexIndex;
			
			uniform mat4 u_ViewProjection;
			
			out vec4 v_Color;
			out vec2 v_TexCoord;
			out float v_TexIndex;
			
			void main()
			{
				v_Color = a_Color;
				v_TexCoord = a_TexCoord; 
				v_TexIndex = a_TexIndex;
				gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
			}
			
			#type fragment
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec4 v_Color;
			in vec2 v_TexCoord;
			in float v_TexIndex;
			
			uniform sampler2D u_Textures[32];
			
			void main()
			{
				color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
			}
		)";

		static std::string DepthShader = R"(
			#type vertex
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_ModelMat;
			
			void main()
			{
			    gl_Position = u_ViewProjection * u_ModelMat * vec4(a_Position, 1.0);
			}  
			
			#type fragment
			
			#version 330 core
			
			void main()
			{             
			}  
		)";

		static std::string LightTextureShader = R"(
			#type vertex
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in float a_ID;
			
			uniform mat4 u_ViewMatrix;
			
			out int vs_ID;
			
			void main()
			{
			    vs_ID = int(a_ID);
			    gl_Position = u_ViewMatrix * vec4(a_Position, 1.0); 
			}
			
			#type geometry
			#version 330 core
			
			layout (points) in;
			layout (triangle_strip, max_vertices = 4) out;
			
			uniform mat4 u_ProjectionMatrix;
			
			uniform vec3 u_ViewPosition;
			uniform float u_Size = 0.3;
			
			in int vs_ID[];
			
			out vec2 gs_TexCoord;
			flat out int gs_ID;
			
			void main()
			{
			
			    gs_ID = vs_ID[0];
			    vec4 position = gl_in[0].gl_Position;
			
			    gl_Position = u_ProjectionMatrix * (position + vec4(-u_Size, -u_Size, 0, 0));
			    gs_TexCoord = vec2(0, 0);
			    EmitVertex();
			    gl_Position = u_ProjectionMatrix * (position + vec4(u_Size, -u_Size, 0, 0));
			    gs_TexCoord = vec2(1, 0);
			    EmitVertex();
			
			    gl_Position = u_ProjectionMatrix * (position + vec4(-u_Size, u_Size, 0, 0));
			    gs_TexCoord = vec2(0, 1);
			    EmitVertex();
			    gl_Position = u_ProjectionMatrix * (position + vec4(u_Size, u_Size, 0, 0));
			    gs_TexCoord = vec2(1, 1);
			    EmitVertex();
			    EndPrimitive();
			
			}
			
			#type fragment
			#version 330 core
			
			layout(location = 0) out vec4 color;
			layout(location = 1) out int color2;
			
			in vec2 gs_TexCoord;
			flat in int gs_ID;
			
			uniform sampler2D u_LightTexture;
			uniform vec4 u_TextureColor;
			
			void main()
			{
			    color = u_TextureColor * texture(u_LightTexture, gs_TexCoord);
			    color2 = gs_ID;
			}  
		)";

		static std::string MaterialShader = R"(
			#type vertex
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout (location = 1) in vec3 a_Normal;
			layout (location = 2) in vec2 a_TexCoord;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_TransformMatrix;
			uniform mat4 u_LightSpace;
			
			out VS_OUT
			{
				vec3 FragPosition;
				vec3 Normal;
				vec2 TexCoord;
				vec4 LightSpacePosition;
			} vsOut;
			
			void main()
			{
				vec4 position = u_TransformMatrix * vec4(a_Position, 1.0);
			
				vec4 normal = normalize(transpose(inverse(u_TransformMatrix)) * vec4(a_Normal, 1.0));
			
				vsOut.FragPosition = vec3(position);
				vsOut.Normal = vec3(normal);
				vsOut.TexCoord = a_TexCoord;
				vsOut.LightSpacePosition = u_LightSpace * position;
				gl_Position =  u_ViewProjection * position;
			}
			
			
			#type fragment
			#version 330 core
			
			layout(location = 0) out vec4 color;
			layout(location = 1) out int color2;
			
			in VS_OUT
			{
				vec3 FragPosition;
				vec3 Normal;
				vec2 TexCoord;
				vec4 LightSpacePosition;
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
			
			//uniform samplerCube u_SkyBoxTexture;
			uniform sampler2D u_ShadowMap;
			
			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
			vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
			float ShadowCalculation(vec4 fragPosLightSpace, float bias);
			
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
			
			float ShadowCalculation(vec4 lightSpacePos, float bias)
			{
				vec3 projection = lightSpacePos.xyz / lightSpacePos.w;
				projection = projection * 0.5 + 0.5;
			
				float closestDepth = texture(u_ShadowMap, projection.xy).r;   
				float currentDepth = projection.z;
			
				//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
				float shadow = 0.0;
				vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
				for(int x = -1; x <= 1; ++x)
				{
				    for(int y = -1; y <= 1; ++y)
				    {
				        float pcfDepth = texture(u_ShadowMap, projection.xy + vec2(x, y) * texelSize).r; 
				        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
				    }    
				}
				shadow /= 9.0;
				return shadow;
			}
			
			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
			{
			    vec3 lightDir = normalize(light.Direction);
			    float diff = max(dot(lightDir, normal), 0.0);
			    vec3 reflectDir = reflect(-lightDir, normal);
			    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
			
			    vec3 ambient  = light.Ambient  * vec3(texture(material.DiffuseTexture, TexCoord));
			    vec3 diffuse  = light.Diffuse  * diff * vec3(texture(material.DiffuseTexture, TexCoord));
			    vec3 specular = light.Specular * spec * vec3(texture(material.SpecularTexture, TexCoord));
			
				//float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
				float shadow = (1 - ShadowCalculation(LightSpacePosition, 0.005));
			
			    //return ((shadow / 2 + 0.5 ) * ambient + shadow * (diffuse + specular));
			    return  (ambient + shadow * (diffuse + specular));
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
		)";

		static std::string NormalShader = R"(
			#type vertex
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec3 a_Normal;
			
			out VS_OUT {
			    vec3 normal;
			} vsOut;
			
			uniform mat4 u_ViewMatrix;
			uniform mat4 u_ModelMatrix;
			
			void main()
			{
			    gl_Position = u_ViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0); 
			    mat3 normalMatrix = mat3(transpose(inverse(u_ViewMatrix * u_ModelMatrix)));
			    vsOut.normal = normalize(vec3(vec4(normalMatrix * a_Normal, 0.0)));
			}
			
			#type geometry
			#version 330 core
			
			layout (triangles) in;
			layout (line_strip, max_vertices = 6) out;
			
			in VS_OUT {
			    vec3 normal;
			} gsIn[];
			
			const float MAGNITUDE = 0.4;
			  
			uniform mat4 u_ProjectionMatrix;
			
			void DrawLine(int index)
			{
			    gl_Position = u_ProjectionMatrix * gl_in[index].gl_Position;
			    EmitVertex();
			    gl_Position = u_ProjectionMatrix * (gl_in[index].gl_Position + vec4(gsIn[index].normal, 0.0) * MAGNITUDE);
			    EmitVertex();
			    EndPrimitive();
			}
			
			void main()
			{
			    DrawLine(0);
			    DrawLine(1);
			    DrawLine(2);
			}  
			
			#type fragment
			#version 330 core
			
			layout(location = 0) out vec4 color;
			layout(location = 1) out int color2;
			
			void main()
			{
			    color = vec4(1.0, 1.0, 0.0, 1.0);
			    color2 = -1;
			}  
		)";

		static std::string OutlineShader = R"(
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
			layout(location = 1) out int color2;
			
			uniform vec4 u_OutlineColor;
			
			void main()
			{
				color = u_OutlineColor;
				color2 = -1;
			}

		)";

		static std::string PostProcessingShader = R"(
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
		)";

		static std::string SkyBoxShader = R"(
			#type vertex
			
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			
			out vec3 v_TexCoords;
			
			uniform mat4 u_View;
			uniform mat4 u_Projection;
			
			void main()
			{
			    v_TexCoords = a_Position;
			    vec4 pos = u_Projection * u_View * vec4(a_Position, 1.0);
			    gl_Position = pos.xyww;
			}
			
			#type fragment
			
			#version 330 core
			layout(location = 0) out vec4 color;
			layout(location = 1) out int color2;
			
			in vec3 v_TexCoords;
			
			uniform samplerCube u_CubeMapTexture;
			
			void main()
			{    
			    color = texture(u_CubeMapTexture, v_TexCoords);
			    color2 = -1;
			}
		)";
	}
}