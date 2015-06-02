#version 330

layout(location = 0) out vec4 vFragColor;

float distance(vec3 posW, vec3 normalW, int i) {
  // Shrink the position to avoid artifacts on the silhouette:
  posW = posW - 0.005 * normalW;

  // Transform to light space:
  float4 posL = mul(float4(posW, 1.0), lights[i].viewproj);

  // Fetch depth from the shadow map:
  // (Depth from shadow maps is expected to be linear)
  float d1 = shwmaps[i].Sample(sampler, posL.xy / posL.w);
  float d2 = posL.z;

  // Calculate the difference:
  return abs(d1 - d2);
}

// This function can be precomputed for efficiency
float3 T(float s) {
  return float3(0.233, 0.455, 0.649) * exp(-s * s / 0.0064) +
         float3(0.1,   0.336, 0.344) * exp(-s * s / 0.0484) +
         float3(0.118, 0.198, 0.0)   * exp(-s * s / 0.187)  +
         float3(0.113, 0.007, 0.007) * exp(-s * s / 0.567)  +
         float3(0.358, 0.004, 0.0)   * exp(-s * s / 1.99)   +
         float3(0.078, 0.0,   0.0)   * exp(-s * s / 7.41);
}

smooth in vec3 Normal;
void main()
{
    float col = (dot(Normal, vec3(0,1,0)) + 1)/2;
    //vFragColor = vec4(col,col, col, 1);
	vFragColor = vec4(1,0, 1, 1);
}
