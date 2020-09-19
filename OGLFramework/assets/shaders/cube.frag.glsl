#version 410 core

layout(location = 0) out vec4 a_Color;

in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_Color;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPos;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float ambientCoff = 0.1;
	float specularCoff = 0.5;

	vec3 ambient = ambientCoff * lightColor;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize( u_LightPosition - v_FragPos );

	float diff = max(dot(v_Normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 32 );
	vec3 specular = specularCoff * spec * lightColor;

	a_Color = vec4(u_Color * (ambient + diff + specular), 1.0);
}