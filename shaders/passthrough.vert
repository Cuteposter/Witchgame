//combined projection and view matrix
uniform mat4 u_projView;

//"in" attributes from our SpriteBatch
attribute vec2 Position;
attribute vec2 TexCoord;
attribute vec4 Color;

//"out" varyings to our fragment shader
varying vec4 vColor;
varying vec2 vTexCoord;
 
void main() {
	vColor = gl_Color;
	vTexCoord = gl_MultiTexCoord0;
	gl_Position = u_projView * gl_Vertex;
}