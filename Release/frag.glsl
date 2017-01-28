uniform sampler3D gradseg;
uniform sampler3D funcion;
uniform sampler3D volumen;

void main()
{
	float i = 0.0;
	float j = 0.0;
	float k = 0.0;
	//valor del eje x seria los valores de los datos
	i = texture3D(volumen,vec3(gl_TexCoord[0])).x;
	j = texture3D(gradseg,vec3(gl_TexCoord[0])).x;
	k = texture3D(gradseg,vec3(gl_TexCoord[0])).w;
	
	gl_FragColor = texture3D(funcion,vec3(i,j,k));
	//gl_FragColor = texture1D(funcion,texture3D(volumen,vec3(gl_TexCoord[0])).x);

}
