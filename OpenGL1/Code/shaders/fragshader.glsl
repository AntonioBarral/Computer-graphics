#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!

// in vec3 vertPos; Using the output from the vertex shader example
in vec3 vertColor;
in vec3 coordinates;
in vec3 normalValue;

// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example
uniform vec3 materialColor;
uniform vec4 intensity;
uniform vec3 position;
uniform vec3 camara;
uniform vec3 lightPosition;


// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    //For colored cube
    fColor = vec4(vertColor, 1.0);
}
