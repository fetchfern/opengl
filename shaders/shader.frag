#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
  vec4 zer = texture(texture0, TexCoord);
  vec4 one = texture(texture1, TexCoord);
  FragColor = mix(zer, one, 0.7);
}
