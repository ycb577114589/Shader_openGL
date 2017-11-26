#ifdef GL_ES
precision mediump float;
#endif
//保证跨平台
 //在此时 三个点已经变成了面,所有面像素都会被处理，因此比顶点数据多
 //如果渲染片元会更耗费性能，但是效果自然更好
 
varying vec4 V_Color;    
 void main(){

    gl_FragColor=V_Color ;
    //获取纹理颜色 调用openGL的shader中内置函数 texture2D,第一个参数传纹理采样器，第二个参数传入UV坐标
 }
