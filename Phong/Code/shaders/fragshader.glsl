#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!

// in vec3 vertPos; Using the output from the vertex shader example
in vec3 color;
in vec3 coordinates;
in vec3 normalValue;
// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example
uniform vec3 materialColor;
uniform vec3 lightpos;
uniform vec3 pos;
uniform vec3 camara;

uniform vec4 intensity;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    // Calculate phong shading values
    vec3 lightColor = vec3(1,1,1);
    vec3 N = normalize(normalValue);
    vec3 L = normalize(lightpos - pos);
    vec3 V = normalize(camara - pos);
    vec3 R = reflect(L, N);

    // Obtain specular, ambient and diffuse light
    vec3 ambient = materialColor * intensity.x;
    vec3 diffuse = max(0.0,dot(N,L)) * materialColor * lightColor * intensity.y ;
    vec3 specular = pow(max(0.0,dot(R,V)),intensity.w) * lightColor * intensity.z;
    vec3 phongIllumination= ambient+specular+diffuse;

    // For colored spheres
    fColor = vec4(phongIllumination,1.0);

}
