//cant de dimensiones de la FT
uniform float opcionilumina;

//cant de dimensiones de la FT
uniform float tipoftm;

//ver ftm o no
uniform float verftm;

//terminación tempra del rayo
uniform float terminaray;

// minima coordenada de textura
uniform vec3      minTexCoord;

// máxima coordenada de textura
uniform vec3      maxTexCoord;

// distancia entre muestras en coodenadas de volumen
uniform float     ray_step;

//el volumen
uniform sampler3D volumen;

// los vectores gradientes
uniform sampler3D gradseg;

// los vectores gradientes
uniform sampler3D funcion;

// los vectores gradientes
uniform sampler3D seg;

// ft unidimensional
uniform sampler1D ftuni;

void main()
{	
	
	
    vec4  result=vec4(0.0, 0.0, 0.0, 1.0);
    vec4  col=vec4(1.0, 1.0, 1.0, 1.0);
    float opacity = 0.0;
    vec4 posRay  = gl_TexCoord[0];
    vec4 view  = normalize(gl_TexCoord[0] - gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0)); view.w = 0.0;
    
    //Normal
    vec3 N = vec3(0.0,0.0,0.0);
    vec3 L = vec3(0.0,0.0,0.0);
    vec3 V = vec3(0.0,0.0,0.0);
    
   vec4 posLuz =  gl_ModelViewMatrixInverse*gl_LightSource[0].position;

    while (opacity<=terminaray) 
    {
        posRay += ray_step * view;
        
   
        //si se sale del cubo
        vec3 signos = (maxTexCoord-posRay.xyz)*(posRay.xyz-minTexCoord);
        if (signos[0]>=0.0 && signos[1]>=0.0 && signos[2]>=0.0)
        {
            //col = clamp(texture3D(funcion,vec3(texture3D(volumen,posRay.xyz).x,texture3D(gradseg,posRay.xyz).x,texture3D(gradseg,posRay.xyz).w)), 0.0, 1.0);
            //col = clamp(texture3D(funcion,vec3(texture3D(volumen,posRay.xyz).x,1.0,1.0)), 0.0, 1.0);
            
           
            
            //col =clamp(texture1D(ftuni,texture3D(volumen,posRay.xyz).x), 0.0, 1.0);
            
            //ojo creo que va en result despues de armar la muestra
            //float uno = clamp(texture3D(gradseg,posRay.xyz).x, 0.0, 1.0); ;
            //col =vec4(uno,uno,uno,uno);
            
            if(tipoftm == 1.0) col =clamp(texture1D(ftuni,texture3D(volumen,posRay.xyz).x), 0.0, 1.0);
				else if(verftm > 0.0) col = clamp(texture3D(funcion,posRay.xyz), 0.0, 1.0); 
					else col = clamp(texture3D(funcion,vec3(texture3D(volumen,posRay.xyz).x,texture3D(gradseg,posRay.xyz).x,texture3D(seg,posRay.xyz).x)), 0.0, 1.0);     
        
			if(opcionilumina == 4.0){
				N = normalize(vec3(texture3D(gradseg,posRay.xyz).yzw)*2.0 - 1.0);
				L = normalize(vec3(posLuz) - posRay.xyz);
				V = normalize(posRay.xyz); //ojo probar solo con el vector de visualización view
				
				vec3 H = normalize(L+V);
				vec4 amb = gl_LightSource[0].ambient;//gl_LightSource[0]gl_FrontLightProduct
				vec4 diff = gl_LightSource[0].diffuse * max(dot(N, L), 0.0);
				vec4 spec = gl_LightSource[0].specular * pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
				col.rgb = col.rgb * (amb.rgb + diff.rgb + spec.rgb);
			}else{
				if(opcionilumina == 3.0){
					col = texture3D(gradseg,posRay.xyz).x * texture3D(seg,posRay.xyz).x * col;
				}else{
					if(opcionilumina == 2.0){
						col = texture3D(seg,posRay.xyz).x * col;
					}else{
						if( opcionilumina == 1.0){
							col = texture3D(gradseg,posRay.xyz).x * col;
						}
					}
				}
			}
			 
        }else break;
        
       // alfa = result.a;
        col.a = 1.0-exp(- col.a );//lo nuevo
        result.rgb += result.a * col.a * col.rgb;//post clasificaci'on
        result.a   *= 1.0-col.a;  // transparencia 
        opacity += col.a*(1.0-opacity); 
        
    } 
       
    gl_FragColor = result;
    
    //gl_FragColor = texture3D(gradseg,posRay.xyz).x*result;
}
