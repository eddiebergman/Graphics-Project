#version 330
out vec4 FragColor;

in vec3 modelVertexPosition;
in vec3 modelVertexNormal;

uniform mat4 view;
uniform vec3 cameraPosition;

struct Material {
	vec3 diffuseReflectance;
	vec3 specularReflectance;
	float shininess;
};

struct DirLight {
	int on;
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
	int on;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	int on;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

vec3 CalcDirLight(DirLight light, vec3 modelVertexNormal, vec3 viewDirection);
vec3 CalcSpotLight(SpotLight light, vec3 modelVertexNormal, vec3 modelVertexPosition, vec3 cameraDirection);
vec3 CalcPointLight(PointLight light, vec3 modelVertexNormal, vec3 modelVertexPosition, vec3 cameraDirection);

#define DIRECTIONAL_LIGHT_LIMIT 5
#define SPOT_LIGHT_LIMIT 5
#define POINT_LIGHT_LIMIT 22

uniform DirLight directionalLights[DIRECTIONAL_LIGHT_LIMIT];
uniform SpotLight spotLights[SPOT_LIGHT_LIMIT];
uniform PointLight pointLights[POINT_LIGHT_LIMIT];
uniform Material material;

void main(){

	vec3 cameraDirection = normalize(cameraPosition - modelVertexPosition);
	vec3 result = vec3(0.0, 0.0, 0.0);
	
	for(int i = 0; i < DIRECTIONAL_LIGHT_LIMIT; i++) {
		result += CalcDirLight(directionalLights[i], modelVertexNormal, cameraDirection);
	}
	for(int i = 0; i < SPOT_LIGHT_LIMIT; i++) {
		result += CalcSpotLight(spotLights[i], modelVertexNormal, modelVertexPosition, cameraDirection);
	}
	for(int i = 0; i < POINT_LIGHT_LIMIT; i++) {
		result += CalcPointLight(pointLights[i], modelVertexNormal, modelVertexPosition, cameraDirection);
	}
	FragColor = vec4 (result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 vertexNormalDirection, vec3 cameraDirection)
{
	//If the light is not on then return nothing
	if(light.on != 1){
		return (vec3(0.0f, 0.0f, 0.0f));
	}

    vec3 lightDirection = normalize(-light.direction);
	
	//Ambience Shading
	vec3 ambientComponent = light.ambient * material.diffuseReflectance;
	
	// diffuse shading
    float diffuse = max(dot(vertexNormalDirection, lightDirection), 0.0);
	vec3 diffuseComponent = light.diffuse * diffuse * material.diffuseReflectance;
    
	// specular shading
    vec3 reflectDirection = reflect(-lightDirection, vertexNormalDirection);
    float specularity = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);
    vec3 specularComponent = light.specular * specularity * material.specularReflectance; 
	
	// combine results
    return (ambientComponent + diffuseComponent + specularComponent);
}

vec3 CalcSpotLight(SpotLight light, vec3 modelVertexNormal, vec3 modelVertexPosition, vec3 cameraDirection)
{
	//If the light is not on then return nothing
	if(light.on != 1){
		return (vec3(0.0f, 0.0f, 0.0f));
	}
	
	//Light direction from its position to the the vertex
	vec3 lightDirection = normalize(light.position - modelVertexPosition);
    
	//Ambience
	vec3 ambientComponent = light.ambient * material.diffuseReflectance;

	// diffuse shading
    float diffuse = max(dot(modelVertexNormal, lightDirection), 0.0);
	vec3 diffuseComponent = light.diffuse * diffuse * material.diffuseReflectance;
    
	// specular shading
    vec3 reflectDirection = reflect(-lightDirection, modelVertexNormal);
    float specularity = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);
	vec3 specularComponent = light.specular * specularity * material.specularReflectance;
    
	// attenuation
    float distance = length(light.position - modelVertexPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// spotlight intensity
    float theta = dot(lightDirection, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
      
	//Adjust for attenuation and intensity
    ambientComponent *= attenuation * intensity;
    diffuseComponent *= attenuation * intensity;
    specularComponent *= attenuation * intensity;
    //ambientComponent = vec3(0.0f,0.0f,0.0f);
	//combine results
	return (ambientComponent + diffuseComponent + specularComponent);
}


vec3 CalcPointLight(PointLight light, vec3 modelVertexNormal, vec3 modelVertexPosition, vec3 cameraDirection)
{
	//If the light is not on then return nothing
	if(light.on != 1){
		return (vec3(0.0f, 0.0f, 0.0f));
	}
	
	//Light direction from the light to the vertex
	vec3 lightDirection = normalize(light.position - modelVertexPosition);
    
	//Ambience
	vec3 ambientComponent = light.ambient * material.diffuseReflectance;
	
	// diffuse shading
    float diffuse = max(dot(modelVertexNormal, lightDirection), 0.0);
	vec3 diffuseComponent = light.diffuse * diffuse * material.diffuseReflectance;
    
	// specular shading
    vec3 reflectDirection = reflect(-lightDirection, modelVertexNormal);
    float specularity = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);
	vec3 specularComponent = light.specular * specularity * material.specularReflectance;
    
	// attenuation
    float distance = length(light.position - modelVertexPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	//Adjust for attenuation
    ambientComponent *= attenuation;
    diffuseComponent *= attenuation;
    specularComponent *= attenuation;

	//Combine results
    return (ambientComponent + diffuseComponent + specularComponent);
}