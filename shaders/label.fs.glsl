#version 330
in vec2 texpos;
uniform sampler2D tex;
uniform vec4 color;
out vec4 finalColor;

void main(void) {
  finalColor = vec4(1, 1, 1, texture(tex, texpos).r) * color;
}

