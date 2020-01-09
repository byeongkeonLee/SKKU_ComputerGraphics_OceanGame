#version 330

// inputs from vertex shader
in vec2 tc;	// used for texture coordinate visualization
in vec4 clipSpace;
in vec3 camera_water_vector;
in vec3 sun_water_vector;
in float visibility;
in vec4 particle_color;
// output of the fragment shader
out vec4 fragColor;

// shader's global variables, called the uniform variables
uniform bool bUseSolidColor;
uniform int FragmentShaderMode;
uniform vec4 solid_color;
uniform sampler2D TEX;

//terrain shader
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

//water shader
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float	wavedistance;
uniform bool	use_texture;
uniform vec4	diffuse;

//tree shader
uniform sampler2D alphaTexture;

//blur shader
uniform float blur_intensity;
uniform vec2		texel_offset;

//skill gui shader
uniform float remained_cool;

//ship shader
uniform float ship_alpha;
const float PI = 3.1415926535897932384626433832795;

//hpmp shader
uniform float ratio;
uniform float	ratio_effect;
uniform float	ratio_shield;

//uniform vec3 skyColour;
vec3 skyColour = vec3(124.0f/255.0f,151.0f/255.0f,168.0f/255.0f);

//grayscale
uniform bool grayscale;
void main()
{
	
	switch (FragmentShaderMode) {
	
	//anchor shader
	case -2: fragColor = vec4(tc.y/2,tc.y/2,tc.y/2,1);break;
	//DEFAULT_SHADER
	
	case -1:fragColor = vec4(tc.xy,0,1); break;
	case 0: fragColor = use_texture? texture2D(TEX,tc): diffuse;
	
	break;

	//TERRAIN_SHADER
	case 1: vec4 blendMapColour = texture(blendMap, tc);
			float backTextureAmount = 1 - (blendMapColour.r +blendMapColour.g+blendMapColour.b);
			vec2 tiledCoords = tc * 40.0f;
			vec4 backgroundTextureColour = texture(backgroundTexture, tiledCoords) * backTextureAmount;
			vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
			vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
			vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;
			vec4 totalColour = backgroundTextureColour + rTextureColour +gTextureColour +bTextureColour;

			fragColor = mix(vec4(skyColour,1.0f), totalColour,visibility);
			//fragColor = texture(bTexture,tc);

			break; 

	//WATER_SHADER
	case 2: 
			vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
			vec2 refractTexCoords = vec2(ndc.x, ndc.y);
			vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
						
			vec2 distortion = (texture(dudvMap, vec2(tc.x+wavedistance, tc.y)).rg * 2.0f - 1.0f)*0.011f;

			refractTexCoords = clamp(refractTexCoords+distortion, 0.0f,1.0f);

			reflectTexCoords += distortion;
			reflectTexCoords.x = clamp(reflectTexCoords.x, 0.0f, 1.0f);
			reflectTexCoords.y = clamp(reflectTexCoords.y, -1.0f, 0.0f);

			vec4 reflectionColour = texture(reflectionTexture, reflectTexCoords);
			vec4 refractionColour = texture(refractionTexture, refractTexCoords); 

			vec4 normalMapColour = texture(normalMap, distortion);
			vec3 normal = normalize(vec3(normalMapColour.r *2.0f -1.0f, normalMapColour.g*2.0f-1.0f, normalMapColour.b));

			vec3 reflectedLight = reflect(normalize(sun_water_vector), normal);
			float specular = max(dot(reflectedLight, camera_water_vector), 0.0f);
			vec3 highlight = vec3(1.0f) * specular * 0.002f;

			float fesnel = dot(normalize(camera_water_vector),vec3(0,0,1));
			fragColor = (reflectionColour*0.3f + refractionColour * 0.2f +fesnel*vec4(vec3(0.4f),0.0f)) + 0.2f*vec4(vec3(0.0f,0.2f,0.5f),1) + vec4(highlight, 0.0f);
			fragColor = mix(vec4(skyColour,1.0f), fragColor,visibility);
			//fragColor = texture(normalMap,tc);
			break;

	//stencil outliner
	case 3:
		fragColor = vec4(1.0f,0.0f,0.0f,1.0f);
		break;

	//tree 
	case 4:
		fragColor = use_texture? texture2D(TEX,tc): diffuse;
		fragColor = mix(vec4(skyColour,1.0f), fragColor,visibility);
		vec4 alpha = texture2D(alphaTexture,tc);
		if(alpha.r<0.1f) discard;
		break;

	//radial_blur
	case 5:
		fragColor = vec4(0);
		float distance_intensity= blur_intensity *(length(tc-vec2(0.5f))-0.3f);
		if(distance_intensity>0){
			for(float x=-2.0f*distance_intensity, max=2.0f*distance_intensity;x<=max; x+=1.0f){
				float weight = exp(-0.5*(x*x)/(distance_intensity*distance_intensity));
				vec4 texel = texture2D(TEX, tc+x*texel_offset);
				fragColor += vec4( texel.rgb, 1 )*weight;
				fragColor /= fragColor.a;
			}
		}else{
			fragColor = texture2D(TEX,tc);
		}
		break;

	//Skill Cooltime
	case 6:
		fragColor = texture2D(TEX, tc);
		if(remained_cool>0){
			fragColor -= vec4(vec3(0.5f),0);
			if(tc.y<remained_cool) fragColor +=vec4(0.1f,0.2f,1.0f,0);
		}
		
		break;
	//ship shader
	case 7:
		fragColor = vec4(vec3(diffuse),ship_alpha);
		fragColor = mix(vec4(skyColour,1.0f), fragColor,visibility);
		break;

	//HPMP shader
	case 8:
		if(tc.x < ratio)
			fragColor = vec4(vec3(diffuse),1.0f);
		else if(tc.x < ratio_shield)
			fragColor = vec4(vec3(1,1,1),1.0f);
		else if(tc.x < ratio_effect)
			fragColor = vec4(vec3(diffuse)-vec3(0.4f),1.0f);
		else 
			fragColor = vec4(vec3(diffuse),0.1f);
		break;
	//Particle shader
	case 10:
		fragColor =texture2D( TEX, tc ) * particle_color;//particlecolor;
		break;
	}
	if(grayscale)
		fragColor = vec4(vec3(fragColor.x*0.299f+fragColor.y*0.587f+fragColor.z*0.114),fragColor.a);
}