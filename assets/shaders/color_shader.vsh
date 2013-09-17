uniform mat4 u_MvpMatrix;
attribute vec4 a_Position;  
void main()                    
{                                	  	  
    gl_Position = u_MvpMatrix * a_Position;            
}          