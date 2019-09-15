#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

void main()
{
   vColor = aColor;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#shader fragment
#version 330 core

in vec3 vColor;
uniform vec4 u_Color;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vColor, 1.0) + u_Color / 2.0;
}