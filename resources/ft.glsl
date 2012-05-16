#define __VERTEX
#ifdef __VERTEX

#version 330

layout(location=0) in vec4 inPosition;

uniform mat4 modelviewprojection;
uniform vec4 fColor;

flat out vec4 oColor;

void main()
{
	gl_Position=modelviewprojection*inPosition;
	oColor=fColor;
}

#endif //__VERTEX

#define __FRAGMENT
#ifdef __FRAGMENT

#version 330

flat in vec4 oColor;

out vec4 outputColor;

void main()
{
	outputColor=oColor;
}

#endif //__FRAGMENT
