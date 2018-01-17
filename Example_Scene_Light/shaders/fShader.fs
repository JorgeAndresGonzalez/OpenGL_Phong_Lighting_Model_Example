//FRAGMENT SHADER
#version 330 core

out vec4 FragColor;

uniform vec3 objColor;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 camPos;

in vec3 fragNormal;
in vec3 fragPos;

void main()
{
    //DIFUSE//

    //Calculate Diffuse Light
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 normalizedFragNormal = normalize(fragNormal);

    float diffuseOffset = max(dot(normalizedFragNormal,lightDir),0.0f);

    vec3 diffuseLight = diffuseOffset * lightColor;

    ////////////////////////////////////////////////////////////////////////////

    //AMBIENT//

    //Calculate Ambient Light
    float ambientStrength = 0.8f;
    vec3 ambientLight = lightColor * ambientStrength;

    ////////////////////////////////////////////////////////////////////////////

    //SPECULAR//

    //Calculate Specular strength
    float specularStrength = 0.5f;

    //Calculate a vector that extends from the fragment towards the viewer
    vec3 viewDir = normalize(camPos - fragPos);

    //Calculate the vector of reflected light, from the fragment to the
    //viewer
    vec3 reflectLightDirection = reflect(-lightDir,normalizedFragNormal);

    //Calculate the impact of reflected light
    float specularOffset = pow(max(dot(reflectLightDirection,viewDir),0.0f),64);

    //Calculate specularLight
    vec3 specularLight = specularStrength * specularOffset * lightColor;

    ////////////////////////////////////////////////////////////////////////////

    //Compute final color
    vec3 resultColor = (diffuseLight + ambientLight + specularLight) * objColor;

    //Apply color to each fragment
    FragColor = vec4(resultColor , 1.0);
}
