#version 330
layout(location=0) out vec4 vFColor;

uniform sampler2D color_texture;

uniform float blur_width = 5;
uniform vec2 pixel_size;
uniform int num_samples = 10;

smooth in vec2 vUV;


const float w[6] = float[](0.006,   0.061,   0.242, 0.242,  0.061, 0.006 );
const float o[6] = float[](-1.0, -0.6667, -0.3333, 0.3333, 0.6667,   1.0 );


vec4 BlurSSSSPas(float width, vec2 pixel_size, vec2 dir, vec2 tex_coords, sampler2D color_texture)
{
    vec2 filter_step = width * pixel_size * dir;
    vec2 finalStep = filter_step;
	float range = 1;
	float step = 1.0f/float(num_samples/2);
	vec3 colorBlurred = texture2D(color_texture, tex_coords).rgb * vec3(0.382);
	float weigths = 1;
	float despl = -1;
    for (int i = 0; i < num_samples; i++)
    {
        vec2 offset = tex_coords + despl*finalStep;
        vec3 colorS = texture2D(color_texture, offset).rgb;
        colorBlurred +=  colorS;
		weigths += 1;
		despl += step;
    }
    return vec4(colorBlurred/vec3(weigths), 1);
}

void main(void)
{
    vFColor = BlurSSSSPas(blur_width, pixel_size, vec2(1, 0), vUV, color_texture);
}