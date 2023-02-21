#version 430 
#define NUMBER_OF_POINT_LIGHTS 4


struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	sampler2D normal;
	sampler2D height;
	float specularShininess;

	int normalizeTex;
	bool usingNormal;
	bool usingEmission;
	bool usingHeight;
	float heightAmount;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool lightActive;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool lightActive;
};

struct SpotLight
{
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

	bool lightActive;
};


out vec4 fragColor;

in vec3 varyingFragPos; //Fragment positions sent out from vertex shader
in vec2 varyingTexCoords;
in vec3 varyingNormal;
in mat3 TBN;
in vec3 TangentViewPos;
in vec3 TangentFragPos;


uniform vec3 viewPos;				//Camera position
uniform Material material;			//Surface material
uniform PointLight pLight[NUMBER_OF_POINT_LIGHTS];
uniform DirectionalLight dLight;
uniform SpotLight sLight;


vec3 calculateDirLight(DirectionalLight dl, vec3 normal, vec3 viewDir, vec2 alteredTexCoords);
vec3 calculatePointLight(PointLight pl, vec3 normal, vec3 viewDir, vec2 alteredTexCoords);
vec3 calculateSpotLight(SpotLight sl, vec3 normal, vec3 viewDir, vec2 alteredTexCoords);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);
float shadowCalculation(vec4 fragPosLightSpace);

void main(void)
{
	vec3 norm;
	vec3 viewDir;
	vec2 texCoords = varyingTexCoords;
	

	//Surface uses normal map
	if (material.usingNormal)
	{
		viewDir = normalize(TangentViewPos - TangentFragPos);
		//Surface uses height map for parallax mapping (This only gets applied if a normal map is used as well)
		if (material.usingHeight)
		{
			//Adjust texcoords to use the height map
			texCoords = ParallaxMapping(varyingTexCoords, viewDir);
		}
		//Normalize texture or not (obtain normal data from normal map in range 0 to 1
		if (material.normalizeTex == 1)
		{
			norm = normalize(texture(material.normal, texCoords).rgb);
		}
		else if (material.normalizeTex == 0)
		{
			norm = texture(material.normal, texCoords).rgb;
		}
		norm = normalize(norm * 2.0 - 1.0); //Transfer normals to range -1 to 1
	}
	//Surface does not use normal map so do lighting as normal
	else 
	{
		viewDir = normalize(viewPos - varyingFragPos);
		norm = normalize(varyingNormal);
	}	

	vec3 result;

	//Directional Light
	//Ensure a directional light exists
	if (dLight.lightActive)  
	{
		result = calculateDirLight(dLight, norm, viewDir, texCoords);
	}
	
	//Point Light
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		//Ensure point light exists
		if (pLight[i].lightActive)
		{
			result += calculatePointLight(pLight[i], norm, viewDir, texCoords);
		}
		
	}

	//Spot Light
	//Ensure a spot light exists
	if (sLight.lightActive) //Ensure a spotlight exists by checking if it has a diffuse value
	{
		result += calculateSpotLight(sLight, norm, viewDir, texCoords);
	}

	//Apply emission map data (if applicable)
	if (material.usingEmission)
	{
		vec3 emission = texture(material.emission, varyingTexCoords).rgb;
		result += emission;
	}
	

	fragColor = vec4(result, 1.0);

}

vec3 calculateDirLight(DirectionalLight dl, vec3 normal, vec3 viewDir, vec2 alteredTexCoords)
{
	vec3 lightDir;  

	if(material.usingNormal)
	{
		lightDir = normalize((TBN)*-dl.direction);
	}
	else
	{
		lightDir = normalize(-dl.direction); // "-" as its from the surface, not from the light
	}

	//Diffuse
	float diff = max(dot(lightDir, normal), 0.0);

	//Specular 
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir),0.0),material.specularShininess);

	//Combine
	vec3 ambient = dl.ambient * vec3(texture(material.diffuse, alteredTexCoords));
	vec3 diffuse = dl.diffuse * diff * vec3(texture(material.diffuse, alteredTexCoords));
	vec3 specular = dl.specular * spec * vec3(texture(material.specular, alteredTexCoords));

	return (ambient + diffuse + specular);

}

vec3 calculatePointLight(PointLight pl, vec3 normal, vec3 viewDir, vec2 alteredTexCoords)
{
	//Determine light space
	vec3 lightDir;
	float distance;

	if (material.usingNormal)
	{
		lightDir = normalize((TBN*pl.position) - TangentFragPos);
		distance = length((TBN*pl.position) - TangentFragPos);
	}
	else
	{
		lightDir = normalize(pl.position - varyingFragPos);
		distance = length(pl.position - varyingFragPos);
	}

	//Diffuse
	float diff = max(dot(lightDir, normal), 0.0);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularShininess);

	//Attenuation
	float attenuation = 1.0f / (pl.constant + pl.linear * distance + pl.quadratic * (distance * distance));

	//Combine
	vec3 ambient = pl.ambient * texture(material.diffuse, alteredTexCoords).rgb;
	vec3 diffuse = pl.diffuse * diff * texture(material.diffuse, alteredTexCoords).rgb;
	vec3 specular = pl.specular * spec * texture(material.specular, alteredTexCoords).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);

}


vec3 calculateSpotLight(SpotLight sl, vec3 normal, vec3 viewDir, vec2 alteredTexCoords)
{
	vec3 lightDir;
	float distance;
	float theta;

	if(material.usingNormal)
	{
		lightDir = normalize((TBN*sl.position) - TangentFragPos);
		distance = length((TBN*sl.position) - TangentFragPos);
		theta = dot(lightDir, normalize((TBN)*-sl.direction));
	}
	else
	{
		lightDir = normalize(sl.position - varyingFragPos);
		distance = length(sl.position - varyingFragPos);
		theta = dot(lightDir, normalize(-sl.direction));
	}

	//Diffuse
	float diff = max(dot(lightDir, normal), 0.0);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularShininess);

	//Attenuation
	float attenuation = 1.0f / (sl.constant + sl.linear * distance + sl.quadratic * (distance * distance));

	//Spotlight intensity
	float epsilon = sl.cutOff - sl.outerCutOff;
	float intensity = clamp((theta - sl.outerCutOff) / epsilon, 0.0, 1.0);

	//Combine
	vec3 ambient = sl.ambient * texture(material.diffuse, alteredTexCoords).rgb;
	vec3 diffuse = sl.diffuse * diff * texture(material.diffuse, alteredTexCoords).rgb;
	vec3 specular = sl.specular * spec * texture(material.specular, alteredTexCoords).rgb;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

//Parallax mapping requires both a normal map and height map. Uses steep parallax mapping which takes multiple samples
//from the texture to give much better results 
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	//Depth layers
	const float minLayers = 8;
	const float maxLayers = 32;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0,0.0,1.0), viewDir), 0.0));

	float layerDepth = 1.0 / numLayers;
	float currentlayerdepth = 0.0;

	//Amount to shift the texture coordinates every layer
	vec2 P = viewDir.xy * material.heightAmount;
	vec2 deltaTexCoords = P / numLayers;

	//Starting values
	vec2 currentTexCoords = texCoords;
	float currentHeightMapValue = texture(material.height, currentTexCoords).r;

	while(currentlayerdepth < currentHeightMapValue)
	{
		//Shift coords along P
		currentTexCoords -= deltaTexCoords;
		
		//Get heightmap value of current texture coordinate
		currentHeightMapValue = texture(material.height, currentTexCoords).r;

		//Get depth of next layer
		currentlayerdepth += layerDepth;
		if (currentlayerdepth > maxLayers) 
			break;
	}

	//Get texture coords before collision
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	//Get depth after and before collison
	float afterDepth = currentHeightMapValue - currentlayerdepth;
	float beforeDepth = texture(material.height, prevTexCoords).r - currentlayerdepth + layerDepth;

	//Interpolate texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;

}

//UNUSED - used to calculate if a pixel is in shadow or not by comparing it against the depthmap texture generated in the first pass
//float shadowCalculation(vec4 fragPosLightSpace)
//{
//	Find the pixels light space position (range -1 - 1)
//	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//	projCoords = projCoords * 0.5 + 0.5; //Transfer to range 0 - 1 to match depth buffer
//
//	//Sample the depth buffer from the first pass
//	float closestDepth = texture(material.depthMap, projCoords.xy).r;
//
//	//Current depth of this pixel
//	float currentDepth = projCoords.z;
//
//	Add a form of shadow bias to avoid shadow acne on surfaces
//	float bias = 0.05;
//	float shadow = 0.0;
//	vec2 texelSize = 1.0 / textureSize(material.depthMap, 0);
//	for (int x = -1; x <= 1; ++x)
//	{
//		for (int y = -1; y <= 1; ++y)
//		{
//			//Allows softer shadows by sampling the surrounding texels as opposed to a single texel
//			float pcfDepth = texture(material.depthMap, projCoords.xy + vec2(x,y) * texelSize).r;
//			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//		}
//	}
//	shadow /= 9.0;
//
//	//If pixel is simple outside of lights view, ensure its marked as not in shadow
//	if (projCoords.z > 1.0)
//		shadow = 0.0;
//
//	return shadow;
//
//}
