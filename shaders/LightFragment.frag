#version 330 core
struct Light{
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 position;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texcods;

uniform Light light;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_specular5;


void main()
{    


vec3 ambient = light.ambient*(texture(texture_diffuse1, Texcods).rgb+texture(texture_diffuse2, Texcods).rgb+texture(texture_diffuse3, Texcods).rgb+texture(texture_diffuse4, Texcods).rgb+texture(texture_diffuse5, Texcods).rgb);

vec3 lightDir = normalize(light.position - FragPos);
vec3 norm = normalize(Normal);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * light.diffuse*(texture(texture_diffuse1, Texcods).rgb+texture(texture_diffuse2, Texcods).rgb+texture(texture_diffuse3, Texcods).rgb+texture(texture_diffuse4, Texcods).rgb+texture(texture_diffuse5, Texcods).rgb);

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0),32.0);
vec3 specular = spec*light.specular*(texture(texture_specular1, Texcods).rgb+texture(texture_specular2, Texcods).rgb+texture(texture_specular3, Texcods).rgb+texture(texture_specular4, Texcods).rgb+texture(texture_specular5, Texcods).rgb);

vec3 result = ambient+diffuse+specular;
FragColor = vec4(result, 1.0);
}