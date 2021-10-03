#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in int a_ID;

out int vs_ID;

uniform mat4 u_ViewMatrix;

void main()
{
    vs_ID = a_ID;
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

    vec4 position = gl_in[0].gl_Position;

    gl_Position = u_ProjectionMatrix * (position + vec4(-u_Size, -u_Size, 0, 0));
    gs_TexCoord = vec2(0, 0);
    gs_ID = vs_ID[0];
    EmitVertex();
    gl_Position = u_ProjectionMatrix * (position + vec4(u_Size, -u_Size, 0, 0));
    gs_TexCoord = vec2(1, 0);
    gs_ID = vs_ID[0];
    EmitVertex();

    gl_Position = u_ProjectionMatrix * (position + vec4(-u_Size, u_Size, 0, 0));
    gs_TexCoord = vec2(0, 1);
    gs_ID = vs_ID[0];
    EmitVertex();
    gl_Position = u_ProjectionMatrix * (position + vec4(u_Size, u_Size, 0, 0));
    gs_TexCoord = vec2(1, 1);
    gs_ID = vs_ID[0];
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

void main()
{
    color = texture(u_LightTexture, gs_TexCoord);
    if (color.a == 0) discard;
    color2 = gs_ID;
}  
