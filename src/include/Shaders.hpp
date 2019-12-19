#ifndef _SHADERS_HPP
#define _SHADERS_HPP

#include "SmartPtr.hpp"

#include <string>

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif



#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class Shaders {
	public:
		Shaders(std::string vsFile, std::string fsFile);
		
		void setSamplerName(std::string name);
		bool hasSampler();
		unsigned int samplerId();
		
		void use();
		
	private:
		unsigned int prog_;
		
		std::string samplerName_;
		
		std::string load(std::string file);
};

typedef SmartPtr<Shaders> ShadersPtr;

#endif
