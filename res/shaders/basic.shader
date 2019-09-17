#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 texCoord;

out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
   v_Color = aColor;
   v_TexCoord = texCoord;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#shader fragment
#version 330 core

in vec3 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_TextureSlot;
uniform sampler2D u_TextureSlot2;
uniform float u_MixValue;

out vec4 fragColor;

void main()
{
	//fragColor = vec4(v_Color, 1.0) + u_Color / 2.0;
	//fragColor = texture(u_TextureSlot, v_TexCoord);
	vec2 temp = vec2(v_TexCoord.x, -v_TexCoord.y);
	fragColor = mix(texture(u_TextureSlot, v_TexCoord), texture(u_TextureSlot2, temp), u_MixValue);
}