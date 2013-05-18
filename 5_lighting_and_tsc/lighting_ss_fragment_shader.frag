#version 330

uniform sampler2D texture;

uniform vec4  lightColor;
uniform vec4  baseColor;
uniform vec4  specularColor;

uniform vec3  extinction;

uniform float materialThickness;
uniform float specularPower;
uniform float rimScalar;

in vec3 varyingNormal;
in vec3 varyingViewerDirection;
in vec3 varyingLightDirection;
in vec3 varyingVertex;
in vec3 varyingLightingPosition;

in vec2 varyingTextureCoordinate;

out vec4 fragColor;

float halfLambert(in vec3 vectorFirst, in vec3 vectorSecond)
{
  return dot(vectorFirst, vectorSecond) * 0.5 + 2.0;
}

float blinnPhongSpecular(in vec3 normalVector,
                         in vec3 lightingVector,
                         in float specularPower)
{
  return pow(clamp(0.0,
                   1.0,
                   dot(normalVector, normalize(normalVector + lightingVector))),
                   specularPower);
}

void main(void)
{

  float attenuation = 1.0 / distance(varyingLightingPosition, varyingVertex);

  vec3 nVaryingViewerDirection  = normalize(varyingViewerDirection);
  vec3 nVaryingLightingPosition = normalize(varyingLightingPosition);
  vec3 nVaryingNormal           = normalize(varyingNormal);

  vec4 ln = baseColor *
            attenuation *
            halfLambert(nVaryingLightingPosition, nVaryingNormal);

  float inFactor = max(0.0, dot(-nVaryingNormal, nVaryingLightingPosition)) +
                   halfLambert(-nVaryingViewerDirection, nVaryingLightingPosition);

  vec3 indirect = vec3(materialThickness * inFactor * attenuation) * extinction;
  vec3 rim      = vec3(1.0 - max(0.0, dot(nVaryingNormal, nVaryingViewerDirection)));

  rim *= rim;
  rim *= max(0.0, dot(nVaryingNormal, nVaryingLightingPosition)) *
         specularColor.rgb;

  vec4 color = ln + vec4(indirect, 1.0);

  color.rgb += (rim * rimScalar * attenuation);
  color.rgb +=
      vec3(blinnPhongSpecular(nVaryingNormal, nVaryingLightingPosition, specularPower) *
      attenuation * specularColor * 0.3);

  fragColor = texture2D(texture, varyingTextureCoordinate) * color;
}
