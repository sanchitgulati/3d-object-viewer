#ifndef _VIEWER_HPP
#define _VIEWER_HPP

#include "Model.hpp"
#include "Matrix.hpp"

#include <string>

#include <GL/glui.h>

/********** User IDs for callbacks ********/
#define LIGHT0_INTENSITY_ID  250

class Viewer {
	public:
		Viewer(std::string name, int width, int height);
		virtual ~Viewer();
		
		void initGlut(int argc, char** argv);
		void initGl();
		void initGlui();
		void initLights();
		
		void setModel(const ModelPtr& model);
		
		void start();
		void stop();
		
		void idle();
		void display();
		void resize(int width, int height);
		void keyDown(unsigned char key, int x, int y);
		void specialKey(int key, int x, int y);
		void mouse(int button, int state, int x, int y);
		void mouseMove(int x, int y);
		void mouseWheel(int button, int dir, int x, int y);
		
		static void controlForwarder(int id);
		
		void control_cb( int control );

		static void setInstance(Viewer* instance);
		static void idleCallback();
		static void displayCallback();
		static void resizeCallback(int width, int height);
		static void keyDownCallback(unsigned char key, int x, int y);
		static void specialKeyCallback(int key, int x, int y);
		static void mouseCallback(int button, int state, int x, int y);
		static void mouseMoveCallback(int x, int y);
		static void mouseWheelCallback(int button, int dir, int x, int y);
		
	private:
		static Viewer* instance_;
		std::string name_;
		int width_, height_, windowId_;
		bool running_, mouseDown_;
		ModelPtr model_;
		Vector2f startPos_;
		Matrix3f xRotation_, yRotation_;
		float farPlane_, modelDistance_, modelRadius_;

		float xy_aspect;
		int   last_x, last_y;
		float rotationX = 0.0, rotationY = 0.0;



		/** Pointers to the windows and some of the controls we'll create **/
		GLUI *glui;
		GLUI_Spinner    *light0_spinner, *light1_spinner;
		GLUI_RadioGroup *radio;
		GLUI_Panel      *obj_panel;


		/** These are the live variables passed into GLUI ***/
		int   wireframe = 0;
		int   obj_type = 1;
		int   segments = 8;
		int   segments2 = 8;
		int   light0_enabled = 1;
		int   light1_enabled = 1;
		float light0_intensity = 1.0;
		float light1_intensity = .4;
		int   main_window;
		float scale = 1.0;
		float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
		float obj_pos[3] = { 0.0, 0.0, 0.0 };

		
		void zoomIn();
		void zoomOut();


		/********** Miscellaneous global variables **********/

		GLfloat light0_ambient[4] =  {0.1f, 0.1f, 0.3f, 1.0f};
		GLfloat light0_diffuse[4] =  {.6f, .6f, 1.0f, 1.0f};
		GLfloat light0_position[4] = {2.f, 2.f, 1.0f, 0.0f};

		GLfloat light1_ambient[4] =  {0.1f, 0.1f, 0.3f, 1.0f};
		GLfloat light1_diffuse[4] =  {.6f, .6f, 1.0f, 1.0f};
		GLfloat light1_position[4] = {2.f, 2.f, 1.0f, 0.0f};

		GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
};



#endif
