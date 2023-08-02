#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

uniform vec3 u_BaseColor;

void main()
{
	o_Color = vec4(u_BaseColor, 1.0);
}