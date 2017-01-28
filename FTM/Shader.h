#ifndef _MY_SHADER
#define _MY_SHADER

#include "glew/glew.h"
#include <QGLWidget>
#include <QFile>

class Shader{
public:
	Shader(QString fileVertex,QString fileFragment,QString fileGeometry=""){
		QString f=loadFile(fileFragment);
		QString v=loadFile(fileVertex);
		if(fileGeometry!="" && GL_EXT_geometry_shader4){
			QString g=loadFile(fileGeometry);
			geometry=new char[g.size()];
			strcpy(geometry,g.toAscii().data());
		}else geometry=NULL;

		fragment=new char[f.size()];
		vertex=new char[v.size()];
		strcpy(fragment,f.toAscii().data());
		strcpy(vertex,v.toAscii().data());
		build();
	}

	Shader(){
		fragment=vertex=geometry=NULL;
		vs=fs=gs=program=0;
	}

	void loadFragment(QString fileFragment){
		QString f=loadFile(fileFragment);
		fragment=new char[f.size()];
		strcpy(fragment,f.toAscii().data());
	}

	void loadVertex(QString fileVertex){
		QString v=loadFile(fileVertex);
		vertex=new char[v.size()];
		strcpy(vertex,v.toAscii().data());
	}
	void loadGeometry(QString fileGeometry){
		QString g=loadFile(fileGeometry);
		geometry=new char[g.size()];
		strcpy(geometry,g.toAscii().data());
	}

	void setFragment(QString srcFragment){
		fragment=new char[srcFragment.size()];
		strcpy(fragment,srcFragment.toAscii().data());
	}

	void setVertex(QString srcVertex){
		vertex=new char[srcVertex.size()];
		strcpy(vertex,srcVertex.toAscii().data());
	}

	void setGeometry(QString srcGeometry){
		geometry=new char[srcGeometry.size()];
		strcpy(geometry,srcGeometry.toAscii().data());
	}

	void build(){

		vs = glCreateShader(GL_VERTEX_SHADER);
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		if(geometry!=NULL) gs = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	
		const char * ff = fragment;	
		const char * vv = vertex;	
		const char * gg = geometry;

		glShaderSource(vs, 1, &vv,NULL);
		glShaderSource(fs, 1, &ff,NULL);
		if(geometry!=NULL) glShaderSource(gs, 1, &gg,NULL);

		glCompileShader(vs);
		glCompileShader(fs);
		if(geometry!=NULL) glCompileShader(gs);

		program=glCreateProgram();
		glAttachShader(program,vs);
		glAttachShader(program,fs);
		if(geometry!=NULL) {
			glAttachShader(program,gs);
			glProgramParameteriEXT(program,GL_GEOMETRY_INPUT_TYPE_EXT,GL_TRIANGLES);
			glProgramParameteriEXT(program,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_TRIANGLE_STRIP );

			int temp;
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
			glProgramParameteriEXT(program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

		}
		glLinkProgram(program);

		printShaderInfoLog(vs);
		printShaderInfoLog(fs);
		if(geometry!=NULL) printShaderInfoLog(gs);
		printProgramInfoLog(program);

	}
	
	void applyShader(){
		glUseProgram(program);
	}
	void unApplyShader(){
		glUseProgram(0);
	}
	GLint getUniLoc(const GLchar *name)
	{
		return glGetUniformLocation(program, name);
	}
	void setInt1(const GLchar *name,int value){
		GLint src=getUniLoc(name);
		glUniform1iARB(src, value);	
	}
	void setFloat1(const GLchar *name,float value){
		GLint src=getUniLoc(name);
		glUniform1fARB(src, value);	
	}
	void setVFloat(const GLchar *name,float *value){
		GLint src=getUniLoc(name);
		glUniform3fvARB(src, 1, value);	
	}
private:
	QString loadFile(QString file){
		QFile f(file);
		if (!f.open(QIODevice::ReadOnly | QIODevice::Text)){
			error+="No se pudo leer el archivo "+file;
			return QString::null;
		}
		return QString(f.readAll());
	}
	void printShaderInfoLog(GLuint obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;
	    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	    if (infologLength > 1)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("printShaderInfoLog[%d]: %s\n",infologLength,infoLog);
	        free(infoLog);
	    }else{
			printf("Shader Info Log: OK\n");
	    }
	}
	void printProgramInfoLog(GLuint obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;
	    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	    if (infologLength > 1)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("printProgramInfoLog[%d]: %s\n",infologLength,infoLog);
	        free(infoLog);
	    }else{
	 	printf("Program Info Log: OK\n");
	    }
	}

	GLuint vs;
	GLuint fs;
	GLuint gs;
	GLuint program;
	char *fragment;
	char *vertex;
	char *geometry;
	QString error;
};

#endif
