#include "Viewer.hpp"
#include "Matrix.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

static float degToRad(float x) {
	return (x * (M_PI / 180.0));
}

Viewer* Viewer::instance_ = NULL;

Viewer::Viewer(std::string name, int width, int height) : name_(name), width_(width), height_(height), 
	running_(false), mouseDown_(false) {
	xRotation_ = Matrix3f::identity();
	yRotation_ = Matrix3f::identity();
}

Viewer::~Viewer() {
}

void Viewer::initGlut(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(width_, height_);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width_) / 4, (glutGet(GLUT_SCREEN_HEIGHT) - height_) / 12);
	main_window = glutCreateWindow(name_.c_str());
}

void Viewer::initGl() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
  	glEnable(GL_DEPTH_TEST);
}

void Viewer::setModel(const ModelPtr& model) {
	model_ = model;
	modelRadius_ = model->radius();
	modelDistance_ = modelRadius_ * 2.0f;
	// arcball_.setRadius(modelRadius_);
	
	Vector3f v;
	v(0) = 0.0f;
	v(1) = 0.0f;
	v(2) = -modelDistance_;
	// arcball_.setCenter(v);
	
	farPlane_ = modelDistance_ + modelRadius_;
}

void Viewer::start() {
	if (!running_) {
		running_ = true;
		glutMainLoop();
	}
}

void Viewer::stop() {
	if (running_)
		exit(0);
}

void Viewer::idle() {
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if ( glutGetWindow() != main_window )
		glutSetWindow(main_window);

	/*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
									application  */

	glutPostRedisplay();
}

void Viewer::display() {
	glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );

	glMatrixMode( GL_MODELVIEW );

	glLoadIdentity();
	glMultMatrixf( lights_rotation );
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glLoadIdentity();
	glTranslatef( 0.0, 0.0, -2.6f );
	glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );
	glMultMatrixf( view_rotate );

	glScalef( scale, scale, scale );
	// glEnable( GL_LIGHTING );
	//   glutSwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width_/(GLfloat)height_, 0.1f, farPlane_);
	gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Matrix3f rotation;
	rotation = xRotation_ * yRotation_;
	


	Vector3f translation;
	translation(0) = 0.0f;
	translation(1) = 0.0f;
	translation(2) = -modelDistance_;

	Matrix4f transform = isometryMatrix(rotation, translation);
	glMultTransposeMatrixf(transform.data());

//exp
	glMultMatrixf( view_rotate );

	
	model_->render();
	
	glutSwapBuffers();
}

void Viewer::resize(int width, int height) {

	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	xy_aspect = (GLfloat)tw / (GLfloat)th;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, xy_aspect, 0.1f, farPlane_);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
	
}

void Viewer::keyDown(unsigned char key, int x, int y) {
	switch (key) {
		case 'q':
		case 27:	//escape
			stop();
			exit(0);
			break;
			
		case '+':
			zoomOut();
			break;
			
		case '-':
			zoomIn();
			break;
	}
  glutPostRedisplay();
}

void Viewer::specialKey(int key, int x, int y) {
	Vector3f xAxis = Vector3f::zero();
	Vector3f yAxis = Vector3f::zero();
	
	xAxis(0) = 1.0f;
	yAxis(1) = 1.0f;
	
	switch(key)	{
		case GLUT_KEY_UP:		
			xRotation_ = xRotation_ * rotationMatrix(xAxis, degToRad(-5.0f));
			break;

		case GLUT_KEY_DOWN:
			xRotation_ = xRotation_ * rotationMatrix(xAxis, degToRad(5.0f));
			break;
		
		case GLUT_KEY_LEFT:
			yRotation_ = yRotation_ * rotationMatrix(yAxis, degToRad(-5.0f));
			break;
		
		case GLUT_KEY_RIGHT:
			yRotation_ = yRotation_ * rotationMatrix(yAxis, degToRad(5.0f));
			break;
	}
}

void Viewer::mouse(int button, int state, int x, int y) {
}

void Viewer::mouseMove(int x, int y) {
}

void Viewer::zoomIn() {
	if (modelDistance_ - modelRadius_ > 0.0f) {
		modelDistance_ -= modelRadius_ / 10.0f;
		farPlane_ = modelDistance_ + modelRadius_;
		
		Vector3f center;
		center(0) = 0.0f;
		center(1) = 0.0f;
		center(2) = -modelDistance_;
	}
}

void Viewer::zoomOut() {	
	modelDistance_ += modelRadius_ / 10.0f;
	farPlane_ = modelDistance_ + modelRadius_;
	
	Vector3f center;
	center(0) = 0.0f;
	center(1) = 0.0f;
	center(2) = -modelDistance_;
}

/*
 * Static callbacks
 */
void Viewer::setInstance(Viewer* instance) {
	instance_ = instance;
	glutIdleFunc(idleCallback);
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyDownCallback);
	glutSpecialFunc(specialKeyCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMoveCallback);
	//glutMouseWheelFunc(mouseWheelCallback);
}

void Viewer::idleCallback() {
	instance_->idle();
}

void Viewer::displayCallback() {
	instance_->display();
}

void Viewer::resizeCallback(int width, int height) {
	instance_->resize(width, height);
}

void Viewer::keyDownCallback(unsigned char key, int x, int y) {
	instance_->keyDown(key, x, y);
}

void Viewer::specialKeyCallback(int key, int x, int y) {
	instance_->specialKey(key, x, y);
}

void Viewer::mouseCallback(int button, int state, int x, int y) {
	instance_->mouse(button, state, x, y);
}

void Viewer::mouseMoveCallback(int x, int y) {
	instance_->mouseMove(x, y);
  	glutPostRedisplay();
}

void Viewer::initLights(){
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
}

void Viewer::controlForwarder(int id){
	static_cast<Viewer*>(instance_)->control_cb(id);
}

void Viewer::initGlui(){
	printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

	/*** Create the side subwindow ***/
	glui = GLUI_Master.create_glui_subwindow( main_window,
						GLUI_SUBWINDOW_BOTTOM );

	/******** Add some controls for lights ********/
	new GLUI_Column( glui, false );
	GLUI_Panel *light0 = new GLUI_Panel( glui, "Light 1" );

	light0_spinner =
	new GLUI_Spinner( light0, "Intensity:",
						&light0_intensity, LIGHT0_INTENSITY_ID,
						controlForwarder );
	light0_spinner->set_float_limits( 0.0, 1.0 );
	GLUI_Scrollbar *sb;
	auto temp = new GLUI_StaticText(light0,"Red0");
	temp->set_text("Red");
	temp->set_alignment(GLUI_ALIGN_CENTER);
	sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,
							&light0_diffuse[0],LIGHT0_INTENSITY_ID,controlForwarder);
	sb->set_float_limits(0,1);
	temp = new GLUI_StaticText(light0,"Green0");
	temp->set_text("Green");
	temp->set_alignment(GLUI_ALIGN_CENTER);
	sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,
							&light0_diffuse[1],LIGHT0_INTENSITY_ID,controlForwarder);
	sb->set_float_limits(0,1);

	temp = new GLUI_StaticText(light0,"Blue0");
	temp->set_text("Blue");
	temp->set_alignment(GLUI_ALIGN_CENTER);
	sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,
							&light0_diffuse[2],LIGHT0_INTENSITY_ID,controlForwarder);
	sb->set_float_limits(0,1);

	new GLUI_Column( glui, true );
	GLUI_Rotation *model_rot = new GLUI_Rotation(glui, "Rotate", view_rotate );
	model_rot->set_spin( .82 );
	model_rot->set_alignment(GLUI_ALIGN_CENTER);
	new GLUI_Column( glui, false );


	new GLUI_Column( glui, true );
	/****** A 'quit' button *****/
	GLUI_Button* quit_button = new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );
	quit_button->set_alignment(GLUI_ALIGN_RIGHT);


	/**** Link windows to GLUI, and register idle callback ******/
	glui->set_main_gfx_window( main_window );

	#if 0
	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
	#endif

}

/* GLUI control callback*/

void Viewer::control_cb( int control )
{if ( control == LIGHT0_INTENSITY_ID ) {
    float v[] = {
      light0_diffuse[0],  light0_diffuse[1],
      light0_diffuse[2],  light0_diffuse[3] };

    v[0] *= light0_intensity;
    v[1] *= light0_intensity;
    v[2] *= light0_intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
  }
}
