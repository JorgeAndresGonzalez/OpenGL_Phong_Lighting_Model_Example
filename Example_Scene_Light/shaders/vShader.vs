//VERTEX SHADER
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 fragNormal;
out vec3 fragPos;

void main()
{
    //Calculate the position of each vertex in clipped space
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);

    //Calculate Normal Matrix
    mat3 normalMat = mat3(transpose(inverse(modelMat)));

    //Calculate the normal vector for each fragment
    fragNormal = normalMat * aNormal;

    //Calculate the position of each fragment 
    fragPos = vec3(modelMat * vec4(aPos, 1.0));

}
