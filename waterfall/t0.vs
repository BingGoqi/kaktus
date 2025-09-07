#version 330 core
layout (location = 0) in vec2 aTexCoord;
uniform int drawtype;
uniform int mouse;
uniform int size;

uniform vec3 clist[11];
uniform int tlist[34];

int gbs[] = { 0,9,18,27,36,45,54,63,72,79,86,93,100,107,114,121,128,134,140,147,154,161,168,175,182,189,196,200,204,208,212,216,220,224,228 };

out vec3 TexCoord;
float dep(int i)
{
    return i*0.0000001f;
}
void main()
{
    float z;
    if(drawtype != 1)
    {
    z = -0.3;
    }
    else
    {
    z = dep(gl_VertexID);
    }
    if(drawtype == 3 && mouse == 1)
    {
    TexCoord = vec3(aTexCoord,0.);
    TexCoord *= clist[0].z;
    TexCoord += clist[0].xy;
    TexCoord /= 105;
    }
    else
    {
    TexCoord = vec3(aTexCoord,0.);
    TexCoord /= 105;
    }
    gl_Position = vec4(TexCoord.xy,z, 1.0);

    TexCoord += 1.;
    TexCoord /=2;
    TexCoord.z = z;
}