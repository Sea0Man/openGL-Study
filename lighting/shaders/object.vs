#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightingColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;


void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);

   vec3 fragPos = vec3(model * vec4(aPos, 1.0));
   vec3 normal = vec3(model * vec4(aNormal, 1.0));  

   float distanceDim = 2.0 / length(lightPos);

   float ambientStrengh = 0.2;
   vec3 ambient = lightColor * ambientStrengh;

   vec3 norm = normalize(normal);
   vec3 lightDir = normalize(lightPos - fragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   float specularStrength = 1.0;
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;

   LightingColor = ambient + (diffuse + specular) * distanceDim;
};