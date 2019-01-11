/************************************************************
ビルトイン関数(一部)
	http://qiita.com/edo_m18/items/71f6064f3355be7e4f45
************************************************************/
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable


/************************************************************
************************************************************/
uniform sampler2DRect texture_0;
uniform sampler2DRect texture_1;

uniform float alpha;

/************************************************************
************************************************************/

/******************************
******************************/
void main(){
	/* */
	vec2 texCoord = gl_TexCoord[0].xy;
	
	vec4 color_0 = texture2DRect( texture_0, texCoord );
	vec4 color_1 = texture2DRect( texture_1, texCoord );
	
	vec4 color = (1 - alpha) * color_0 + alpha * color_1;
	
	gl_FragColor = color;
}

