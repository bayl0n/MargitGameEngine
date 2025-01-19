#version 460 core

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform float textureVisibility;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureVisibility) * vec4(ourColor, 1.0);
}