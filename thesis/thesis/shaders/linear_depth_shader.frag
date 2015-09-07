#version 330
//precision highp float;
layout(location = 0) out /*highp*/ float fragDeepth;

smooth in /*highp*/ float linear_depth;
void main()
{
    fragDeepth = linear_depth;
}
