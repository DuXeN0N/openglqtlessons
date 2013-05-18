#version 330
uniform vec4 diffuseColor;

uniform float diffusePower;

uniform sampler2D texture;

in vec3 varyingLightDirection;
in vec3 varyingNormal;

in vec2 varyingTextureCoordinate;

out vec4 fragColor;

void main(void)
{
  vec3 nVaryingLightDirection = normalize(varyingLightDirection);  // Captain Obvious!
  vec3 nVaryingNormal         = normalize(varyingNormal);

  float diffuse = 0.2f + max(dot(nVaryingNormal, nVaryingLightDirection), 0.1f);
  float koeff;

  if (diffuse <= 0.25f)
  {
    koeff = 0.4f;
  }
  else if (diffuse <= 0.5f)
  {
    koeff = 0.8f;
  }
  else if (diffuse <= 0.75f)
  {
    koeff = 1.2f;
  } else {
    koeff = 1.6f;
  }

  fragColor = texture2D(texture, varyingTextureCoordinate) *
              diffuseColor * diffusePower * koeff;
}
