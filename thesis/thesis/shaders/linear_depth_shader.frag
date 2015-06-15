#version 330
precision highp float;
layout(location = 0) out float fragDeepth;

smooth in highp float linear_depth;
void main()
{
    /*highp float*/ fragDeepth = linear_depth;
}
