#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

float palette[32] = float[](0, 15, 31, 46, 63, 78, 94, 109, 127, 142, 158, 173, 190, 205, 221, 236, 255, 236, 221, 205, 190, 173, 158, 142, 127, 109, 94, 78, 63, 46, 31, 15);

out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord) * fragColor;
    float grey = palette[int(texelColor.r * 255) % 32] / 255.0;
    finalColor = vec4(grey, grey, grey, 1.0);
}