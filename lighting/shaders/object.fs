#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


#define NUMBER_POINT_LIGHT 4

struct Material{
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct DirectionalLight{
   vec3 direction;

   vec3 diffuse;
   vec3 specular;
};

struct PointLight{
   vec3 position;

   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

struct SpotLight{
   vec3 position;
   vec3 direction;

   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
   float cutOff;
   float outerCutOff;
};

uniform vec3 viewPos;
uniform vec3 ambient;
uniform Material material;
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLight[NUMBER_POINT_LIGHT];


vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
   vec3 lightDir = normalize(light.direction);

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

   return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir){
   vec3 lightDir = normalize(light.position - FragPos);

   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

   return (diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir){
   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));


   vec3 lightDir = normalize(light.position - FragPos);


   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

   if(theta > light.outerCutOff) 
   {       
      float diff = max(dot(norm, lightDir), 0.0);
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));


      vec3 reflectDir = reflect(-lightDir, norm);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));


      return (diffuse + specular) * attenuation * intensity;
   }
   else{
      return vec3(0.0);
   }
}

void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   vec3 Result = ambient * vec3(texture(material.diffuse, TexCoords));

   // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
   Result += CalcDirLight(dirLight, norm, viewDir);

   for(int i = 0; i < NUMBER_POINT_LIGHT; i++)
   {
      Result += CalcPointLight(pointLight[i], norm, viewDir);
   }

   Result += CalcSpotLight(spotLight, norm, viewDir);

   FragColor = vec4(Result, 1.0);
};