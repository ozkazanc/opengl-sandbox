#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = u_MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
//=========================================================
#shader fragment
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_TextureSlot;

out vec4 fragColor;

void main()
{
	fragColor = texture(u_TextureSlot, v_TexCoord);
}