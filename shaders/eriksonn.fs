#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
    vec4 texelColor  = texture(texture0, fragTexCoord);
    int byte0 = int(texelColor.r * 255.0); 
    int byte1 = int(texelColor.g * 255.0) <<  8; 
    int byte2 = int(texelColor.b * 255.0) << 16; 
    int byte3 = int(texelColor.a * 255.0) << 24; 
    float n = float(byte0 + byte1 + byte2 + byte3);
    float a = 0.1;
    finalColor = vec4(0.5*sin(a*n) + 0.5, 0.5*sin(a*n + 2.094) + 0.5, 0.5*sin(a*n + 4.188) + 0.5, 1.0);
}