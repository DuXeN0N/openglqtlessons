#version 330

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;

uniform mat3 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

in vec4 vertex;

in vec3 normal;

in vec2 textureCoordinate;

out vec3 varyingNormal;
out vec3 varyingViewerDirection;
out vec3 varyingLightDirection;
out vec3 varyingVertex;
out vec3 varyingLightingPosition;
out vec2 varyingTextureCoordinate;

void main(void)
{
  vec4 eyeVertex = mvMatrix * vertex;

  eyeVertex /= eyeVertex.w;

  varyingLightDirection    = normalize(lightPosition - eyeVertex.xyz);
  varyingNormal            = normalize(normalMatrix * normal);
  varyingViewerDirection   = -eyeVertex.xyz;
  varyingTextureCoordinate = textureCoordinate;
  varyingVertex            = vertex.xyz;
  varyingLightingPosition  = lightPosition;

  gl_Position  = mvpMatrix * vertex;
}
