#version 330
uniform sampler2D texture;

uniform vec3 diffuseColor;

in vec3 varyingLightDirection;
in vec3 varyingNormal;

in vec2 varyingTextureCoordinate;

out vec4 fragColor;

void main(void)
{
  vec3 nVaryingLightDirection = normalize(varyingLightDirection);  // Captain Obvious!
  vec3 nVaryingNormal         = normalize(varyingNormal);

  fragColor = texture2D(texture, varyingTextureCoordinate) *
              max(dot((varyingLightDirection), varyingNormal), 0.25f);
}
