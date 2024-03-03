#version 330 core

in vec2 texpos;
out vec4 FragColor;

uniform sampler2D inner_chr_tx;
uniform sampler2D outer_chr_tx;
uniform vec4 inner_color;
uniform vec4 outer_color;
uniform int nTextures;

void main(void) {

  if (nTextures == 2)
  {
    vec4 inner = vec4(1.0f, 1.0f, 1.0f, texture(inner_chr_tx, texpos).r) * inner_color;
    vec4 outer = vec4(1.0f, 1.0f, 1.0f, texture(outer_chr_tx, texpos).r) * outer_color;
    FragColor = outer + (inner - outer) * inner.a;
    FragColor.a = min(1.0f, texture(outer_chr_tx, texpos).r + texture(inner_chr_tx, texpos).r);
    return;
  } 
  else if (nTextures == 1) 
  {
    FragColor = vec4(1.0f, 1.0f, 1.0f, texture(inner_chr_tx, texpos).r) * inner_color;
    return;
  }
  
}