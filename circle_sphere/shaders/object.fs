#version 450 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 norm;

struct Material{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct Light{
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{
   vec3 ambient = light.ambient * material.ambient;


   vec3 lightDir = normalize(light.direction);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);


   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(normalize(-light.direction), norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;

   FragColor = vec4(result, 1.0);
};