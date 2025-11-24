#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform int max_iterations;

out vec4 finalColor;
float palette_r[16] = float[](66, 25, 9, 4, 0, 12, 24, 57, 134, 211, 241, 248, 255, 204, 153, 106);
float palette_g[16] = float[](30, 7, 1, 4, 7, 44, 82, 125, 181, 236, 233, 201, 170, 128, 87, 52);
float palette_b[16] = float[](15, 26, 47, 73, 100, 138, 177, 209, 229, 248, 191, 95, 0, 0, 0, 3);

void main()
{
    vec4 texelColor  = texture(texture0, fragTexCoord);
    int byte0 = int(texelColor.r * 255.0); 
    int byte1 = int(texelColor.g * 255.0) <<  8; 
    int byte2 = int(texelColor.b * 255.0) << 16; 
    int byte3 = int(texelColor.a * 255.0) << 24; 
    int n = byte0 + byte1 + byte2 + byte3;
    if (n > 0 && n < max_iterations)
    {
        int index = n % 16;
        finalColor = vec4(palette_r[index] / 255, palette_g[index] / 255, palette_b[index] / 255, 1.0);
    }
    else
    {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

 