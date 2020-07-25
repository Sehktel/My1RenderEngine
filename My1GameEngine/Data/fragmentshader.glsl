#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	FragColor = texture2D(gSampler, 1.0 - v_TexCoord.st);
}