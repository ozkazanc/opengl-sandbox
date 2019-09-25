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

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_ViewPosition;

void main()
{
	// Ambient lighting
	vec3 ambient = u_Light.ambient * u_Material.ambient;
	
	// Diffuse lighting
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.position - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Material.diffuse * diff * u_Light.diffuse;

	// Specular lighting
	vec3 viewDir = normalize(u_ViewPosition - v_FragPos);
	vec3 reflection = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflection, viewDir), 0.0), u_Material.shininess);
	vec3 specular = u_Material.specular * spec * u_Light.specular;

	// Resulting lighting
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}