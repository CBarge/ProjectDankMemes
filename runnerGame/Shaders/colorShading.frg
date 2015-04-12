#version 130

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform sampler2D sampleTex;

void main()
{

	vec4 textureColor = texture(sampleTex, fragUV);
	
	color = textureColor * fragColor;
	//dynamic color
	color = fragColor * textureColor;
}