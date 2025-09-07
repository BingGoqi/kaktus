#version 330 core
out vec4 FragColor;

int gbs[] = { 0,9,18,27,36,45,54,63,72,79,86,93,100,107,114,121,128,134,140,147,154,161,168,175,182,189,196,200,204,208,212,216,220,224,228 };

in vec3 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    if(TexCoord.z == -0.3)
    {
    FragColor = vec4(1,1,1,1);
    }
    else
    {
        FragColor = vec4(TexCoord.rg,TexCoord.b + 0.33,1);
    }
    //FragColor = vec4(texture(ourTexture, TexCoord.xy).rgb,1);
}