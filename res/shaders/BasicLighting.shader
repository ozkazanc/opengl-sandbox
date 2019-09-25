#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
	v_Normal = aNormal;
	v_FragPos = vec3(u_Model * vec4(aPos, 1.0));
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}
//=========================================================
#shader fragment
#version 330 core

in vec3 v_Normal;
in vec3 v_FragPos;
out vec4 fragColor;

uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

void main()
{
	// Ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;
	
	// Diffuse lighting
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPosition - v_FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_ViewPosition - v_FragPos);
	vec3 reflection = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(reflection, viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * u_LightColor;

	// Resulting lighting
	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	fragColor = vec4(result, 1.0);
}