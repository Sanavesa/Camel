#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 o_Color;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform vec3 u_LightColor;
uniform vec3 u_BaseColor;
uniform vec3 u_SkyColor;
uniform vec3 u_GroundColor;
uniform sampler2D u_DiffuseImage;

void main()
{
	// ambient
	float ambientStrength = 0.2;
	float h = 0.5 * v_Normal.y + 0.5;
	vec3 ambient = h * u_SkyColor + (1.0 - h) * u_GroundColor;
	ambient *= ambientStrength * u_BaseColor;

	// diffuse
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	vec3 normal = normalize(v_Normal);
	float diff = max(dot(lightDir, normal), 0.1);
	vec3 diffuse = diff * u_LightColor * u_BaseColor;

	// specular
	float specularStrength = 0.2;
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * u_LightColor;

	vec4 texColor = texture(u_DiffuseImage, v_TexCoord);
	o_Color = vec4(ambient + (diffuse * texColor.rgb) + specular, texColor.a);
}
