#define KERNEL_SIZE 9

uniform sampler2D sampler0;
uniform float kernel_weights[KERNEL_SIZE];
uniform vec2 texture_offsets[KERNEL_SIZE];

void main(void)
{

  vec2 offset[KERNEL_SIZE];

  int i;
  vec4 sum = vec4(0.0);

  for(i = 0; i < KERNEL_SIZE; i++){
   vec4 tmp = texture2D(sampler0, gl_TexCoord[0].st + texture_offsets[i]);
   sum += tmp * kernel_weights[i];
  }

  gl_FragColor = sum;
  
  //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}