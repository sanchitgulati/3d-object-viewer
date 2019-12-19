#include "ObjParser.hpp"
#include "Shaders.hpp"
#include "Viewer.hpp"
#include <string>


#ifdef __APPLE__
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#endif

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <stdexcept>
#include "InputParser.cpp"

int main(int argc, char* argv[])
{
	InputParser input(argc, argv);
	bool useParams = true;
	std::string filepath = input.getCmdOption("-f");
	std::string vsFilepath = input.getCmdOption("-vs");
	std::string fsFilepath = input.getCmdOption("-fs");
    if (filepath.empty() || vsFilepath.empty() || fsFilepath.empty())
	{
		std::cout << "Usage: -f [object filepath] -vs [vertex shader filepath] -fs [fragment shader filepath]" << std::endl;
		return -1;
	}
	std::cout << "Object Filepath " << filepath << std::endl;
	std::cout << "Shaders  vs " << vsFilepath << " fs " << fsFilepath  << std::endl;


	ModelPtr model;
	ObjParser parser;
	
	try {	
		model = parser.parseObj(filepath);
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	
	model->setUseParams(useParams);
	
	//compute centroid and center model
	model->centroid();
	model->center();
	
	//setup viewer
	Viewer viewer("Model Viewer", 1024, 768);
	viewer.setModel(model);
	viewer.initGlut(argc, argv);
	viewer.initGl();

	//load shaders
	ShadersPtr shaders(new Shaders(vsFilepath, fsFilepath));
	shaders->setSamplerName("texMap");
	
	//prepare for rendering
	model->loadTextures();
	model->compileLists();
	model->setShaders(shaders);
	
	Viewer::setInstance(&viewer);
	viewer.initGlui();
	viewer.initLights();
	
	viewer.start();

  return EXIT_SUCCESS;
  
}