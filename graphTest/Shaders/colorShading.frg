#version 130

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform float time;
uniform sampler2D sampleTex;

void main()
{

	vec4 textureColor = texture(sampleTex, fragUV);
	
	color = textureColor * fragColor;
	//dynamic color
	color = vec4(fragColor.r * (cos(fragPosition.x * 4.0 + time) + 1.0) * 0.5,
                 fragColor.g * (cos(fragPosition.y * 8.0 + time) + 1.0) * 0.5,
                 fragColor.b * (cos(fragPosition.x * 2.0 + time) + 1.0) * 0.5, fragColor.a) * textureColor;
}