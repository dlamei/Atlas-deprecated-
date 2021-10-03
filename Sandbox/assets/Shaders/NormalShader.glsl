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