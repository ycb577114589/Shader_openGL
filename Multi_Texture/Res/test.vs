attribute vec4 position;
attribute vec4 color;
attribute vec2  texcoord;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

varying vec4 V_Color;    
varying vec2 V_Texcoord;

//原来cgshader 的UNITY_MATRIX_MVP大体是这个意思。
 //在opengl中，数据传入显卡， 每个显卡会有诸多核心，每个核心会获得一个顶点数据处理， 因此是个并行的概念     
 
void main(){
    
    V_Color=color;
    V_Texcoord= texcoord; 
    gl_Position= ProjectionMatrix*ViewMatrix*ModelMatrix * position;
   
}   