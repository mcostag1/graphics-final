#version 410

in vec4 vPosition;

uniform vec4 disp;

void
main()
{
    gl_Position =  vPosition + disp;
}
