 void main()
{


  // volume verteces in eye space
  gl_TexCoord[0] = gl_Vertex;

  // proyecci�n del v�rtice    
  gl_Position    = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

}
