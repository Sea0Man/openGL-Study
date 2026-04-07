#version 330 core

out vec4 FragColor;

in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{

   float distanceDim = 2.0 / length(lightPos);

   float ambientStrengh = 0.3;
   vec3 ambient = lightColor * ambientStrengh;

   vec3 norm = fragPos;
   vec3 lightDir = normalize(lightPos - fragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   float specularStrength = 2.0;
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
   vec3 specular = specularStrength * spec * lightColor;

   vec3 result = ambient + (diffuse + specular) * distanceDim;

   FragColor = vec4(result * objectColor, 1.0);
};