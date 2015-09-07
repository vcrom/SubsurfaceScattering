#version 330
layout(location=0) out vec4 vFColor;

uniform sampler2D color_texture;

uniform float blur_width = 1;
uniform vec2 pixel_size;

smooth in vec2 vUV;


const float w[6] = float[](0.006,   0.061,   0.242, 0.242,  0.061, 0.006 );
const float o[6] = float[](-1.0, -0.6667, -0.3333, 0.3333, 0.6667,   1.0 );


vec4 BlurSSSSPas(float width, vec2 pixel_size, vec2 dir, vec2 tex_coords, sampler2D color_texture)
{
    vec2 filter_step = width * pixel_size * dir;
    vec2 finalStep = filter_step;
	//finalStep/= 3;

	vec3 colorBlurred = texture2D(color_texture, tex_coords).rgb * vec3(0.382);
	float weigths = 0.382;
    for (int i = 0; i < 7; i++)
    {
		vec2 despl = o[i] * finalStep;
        vec2 offset = tex_coords + despl;
        vec3 colorS = texture2D(color_texture, offset).rgb;
        colorBlurred += vec3(w[i]) * colorS;
		weigths += w[i];
    }
    return vec4(colorBlurred/vec3(weigths), 1);
}

void main(void)
{
    vFColor = BlurSSSSPas(blur_width, pixel_size, vec2(1, 0), vUV, color_texture);
}