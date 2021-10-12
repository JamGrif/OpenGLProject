#version 430 
out vec4 color;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube sky;
uniform bool usingReflection;
uniform bool usingRefraction;

void main()
{
    if (usingReflection)
    {
        vec3 I = normalize(Position - viewPos); //Vector from camera to surface
        vec3 R = reflect(I, normalize(Normal)); //Vector reflecting from surface to cubemap
        color = vec4(texture(sky, R).rgb, 1.0); //Sample the cubemap on where vector R hit
    }

    if (usingRefraction)
    {
        float ratio = 1.00 / 1.33;
        vec3 I = normalize(Position - viewPos);         //Vector from camera to surface
        vec3 R = refract(I, normalize(Normal), ratio);  //Vector refracting through surface to cubemap
        color = vec4(texture(sky, R).rgb, 1.0);         //Sample the cubemap where vector R hit
    }
    
}