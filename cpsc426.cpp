#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include "math.h"
#include "time.h"
#include "Simulation.h"
#include <string>       // std::string
#include <sstream>      // std::stringstream, std::stringbuf

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

float camZ;

int Width = 800;
int Height = 800;

double movement_speed = 3.0;

// time increment between calls to idle() in ms,
// currently set to 30 FPS
float dt = 1000.0f*1.0f/30.0f;

unsigned char camera = 'r';

// 0 & 1 are left and right
// 2 & 3 are up and down
// 4 & 5 are forward and back
bool pushedDirections[6];

uint32_t tickCount;
uint32_t lastCount;

Vector3 ropeConnectionVel;
Simulation* ropeSimulation = new Simulation(
                          50,           // # of particles
                          0.05f,        // Mass
                          10000.0f,     // Spring Constant in rope
                          0.05f,        // Normal length of springs in rope
                          0.2f,         // Spring Inner Friction Constant
                          Vector3(0.0, -9.81f, 0.0), // Gravity
                          0.15f,        // Air Friction
                          50.0f,        // Ground Repel
                          0.2f,         // Ground Slide Friction
                          2.0f,         // Ground Absorption
                          -1.5f);       // Ground Height


void reshapeCallback(int w, int h)
{
 Width = w;
 Height = h;
 glViewport(0, 0, w, h);
}

void adjustSpeed() {
  if (pushedDirections[2] && pushedDirections[3]) {

  }
  else if (pushedDirections[3]) {
    ropeConnectionVel[1] = -movement_speed;
  }
  else if (pushedDirections[2]) {
    ropeConnectionVel[1] = movement_speed;
  }
  else {
    //std::cout << "Hello";
    ropeConnectionVel[1] = 0.0;
  }

  if (pushedDirections[0] && pushedDirections[1]) {

  }
  else if (pushedDirections[1]) {
    ropeConnectionVel[0] = movement_speed;
  }
  else if (pushedDirections[0]) {
    ropeConnectionVel[0] = -movement_speed;
  }
  else {
    //std::cout << "Hello";
    ropeConnectionVel[0] = 0.0;
  }

  if (pushedDirections[4] && pushedDirections[5]) {

  }
  else if (pushedDirections[4]) {
    ropeConnectionVel[2] = -movement_speed;
  }
  else if (pushedDirections[5]) {
    ropeConnectionVel[2] = movement_speed;
  }
  else {
    //std::cout << "Hello";
    ropeConnectionVel[2] = 0.0;
  }

  ropeSimulation->setRopeConnectionVel(ropeConnectionVel);

}

void keyboardCallback(unsigned char c, int x, int y) {
  switch (c) {
    case 'q':
      exit (0);
      break;
    case 'w':
      camera = 'w';
      camZ += 0.5;
      break;
    case 's':
      camera = 's';
      camZ -= 0.5;
      break;
    case '1':
      pushedDirections[4] = true;
      ropeConnectionVel[2] = -movement_speed;
      break;
    case '2':
      pushedDirections[5] = true;
      ropeConnectionVel[2] = movement_speed;
      break;
    case '+':
      movement_speed += 0.1;
      break;
    case '-':
      movement_speed -= 0.1;
      break;
    case 'g':
      ropeSimulation->setGravity(Vector3(0,ropeSimulation->gravity[1]-0.5,0));
      break;
    case 'G':
      ropeSimulation->setGravity(Vector3(0,ropeSimulation->gravity[1]+0.5,0));
      break;
    case 'a':
      ropeSimulation->setAirFriction(ropeSimulation->airFriction+=0.01);
      break;
    case 'A':
      ropeSimulation->setAirFriction(ropeSimulation->airFriction-=0.01);
      break;
    case 'r':
      ropeSimulation->setGroundRepulsion(ropeSimulation->groundRepulsion+=1);
      break;
    case 'R':
      ropeSimulation->setGroundRepulsion(ropeSimulation->groundRepulsion-=1);
      break;
    case 'f':
      ropeSimulation->setGroundFriction(ropeSimulation->groundFriction+=0.05);
      break;
    case 'F':
      ropeSimulation->setGroundFriction(ropeSimulation->groundFriction-=0.05);
      break;
    case 'b':
      ropeSimulation->setGroundAbsorption(ropeSimulation->groundAbsorption+=0.1);
      break;
    case 'B':
      ropeSimulation->setGroundAbsorption(ropeSimulation->groundAbsorption-=0.1);
      break;
  }
  adjustSpeed();
  //ropeSimulation->setRopeConnectionVel(ropeConnectionVel);
  //glutPostRedisplay();
}

void keyboardUpCallback(unsigned char c, int x, int y) {

  switch (c) {
    case '1':
    pushedDirections[4] = false;
    //ropeConnectionVel[2] = 0.0;
    break;
    case '2':
    pushedDirections[5] = false;
    //ropeConnectionVel[2] = 0.0;
    break;
  }
  adjustSpeed();
  //ropeSimulation->setRopeConnectionVel(ropeConnectionVel);
}

void SpecialInput(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP:
      pushedDirections[2] = true;
      ropeConnectionVel[1] = movement_speed;
      break;  
    case GLUT_KEY_DOWN:
      pushedDirections[3] = true;
      ropeConnectionVel[1] = -movement_speed;
      break;
    case GLUT_KEY_LEFT:
      pushedDirections[0] = true;
      ropeConnectionVel[0] = -movement_speed;
      break;
    case GLUT_KEY_RIGHT:
      pushedDirections[1] = true;
      ropeConnectionVel[0] = movement_speed;
      break;
  }
  adjustSpeed();
  //ropeSimulation->setRopeConnectionVel(ropeConnectionVel);
  //float maxPossibleDt = 0.002f;
  //float secs = dt / 1000.0f;
  //int numIterations = (int)secs / maxPossibleDt;
  //for (int i = 0; i < numIterations; i++) {
  //  std::cout << "HELLO!";
  //for (int i = 0; i < 10; i++) {
  //  ropeSimulation->operate(0.001);
  //}

  //glutPostRedisplay();
}

void SpecialUpInput(int key, int x, int y) {
  switch(key)
  {
    case GLUT_KEY_UP:
    pushedDirections[2] = false;
    //ropeConnectionVel[1] = 0;
    break;  
    case GLUT_KEY_DOWN:
    pushedDirections[3] = false;
    //ropeConnectionVel[1] = 0;
    break;
    case GLUT_KEY_LEFT:
    pushedDirections[0] = false;
    //ropeConnectionVel[0] = 0;
    break;
    case GLUT_KEY_RIGHT:
    pushedDirections[1] = false;
    //ropeConnectionVel[0] = 0;
    break;
  }
  adjustSpeed();
  //ropeSimulation->setRopeConnectionVel(ropeConnectionVel);

  //for (int i = 0; i < 10; i++) {
  //  ropeSimulation->operate(0.001);
 // }

  //glutPostRedisplay();
}

void drawAxis() {
  float axisHeight = 0.75; // TODO
  float axisBase = 0.075;
  glColor3f(0, 0, 1);
  glutSolidCone(axisBase, axisHeight, 8, 2);
  glPushMatrix();
  glRotatef(90, 0, 1, 0);
  glColor3f(1, 0, 0);
  glutSolidCone(axisBase, axisHeight, 8, 2);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  glColor3f(0, 1, 0);
  glutSolidCone(axisBase, axisHeight, 8, 2);
  glPopMatrix();
}

void motionUpdate() {
  //ropeSimulation->setRopeConnectionVel(ropeConnectionVel);

  float dtt = dt / 1000.0f;
  float maxPossibleDt = 0.002f;

  int numIterations = (int)(dtt / maxPossibleDt) + 1;

  if (numIterations != 0)
    dtt = dtt/numIterations;
  for (int i = 0; i < numIterations; i++) {
    ropeSimulation->operate(dtt);
  }
}

void drawSpeed() {
  glPushMatrix();

  std::ostringstream strs;
  strs << movement_speed;
  std::string str = strs.str();

  // Movement Speed
  //glScalef( 1.5f, 2.0f, 1.0f );
  std::string identifier = "Speed: ";
  int i = 0;
  for (i = 0; i < identifier.length(); i++ ) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, identifier[i]);
  }

  for (i = 0; i < str.length(); i++) {
    glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i]);
  }
  glPopMatrix();
}

void drawGravity() {
  glPushMatrix();
  glTranslatef(0,200,0);

  std::ostringstream strs;
  strs << ropeSimulation->gravity[1];
  std::string str = strs.str();
  
  std::string identifier = "Gravity: ";
  int i = 0;
  for (i = 0; i < identifier.length(); i++) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, identifier[i]);
  }

  for (i = 0; i < str.length(); i++) {
    glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i]);
  }

  glPopMatrix();

}

void drawAirFriction() {
  glPushMatrix();
  glTranslatef(0,400,0);

  std::ostringstream strs;
  strs << ropeSimulation->airFriction;
  std::string str = strs.str();
  
  std::string identifier = "Air Friction: ";
  int i = 0;
  for (i = 0; i < identifier.length(); i++) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, identifier[i]);
  }

  for (i = 0; i < str.length(); i++) {
    glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i]);
  }

  glPopMatrix();
}

void drawGroundAbsorption() {
  glPushMatrix();
  glTranslatef(1500,0,0);
  std::ostringstream strs;
  strs << ropeSimulation->groundAbsorption;
  std::string str = strs.str();
  
  std::string identifier = "Ground Absorption: ";
  int i = 0;
  for (i = 0; i < identifier.length(); i++) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, identifier[i]);
  }

  for (i = 0; i < str.length(); i++) {
    glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i]);
  }

  glPopMatrix();
}

void drawGroundSlideFriction() {
  glPushMatrix();
  glTranslatef(1500,200,0);
  std::ostringstream strs;
  strs << ropeSimulation->groundFriction;
  std::string str = strs.str();
  
  std::string identifier = "Ground Friction: ";
  int i = 0;
  for (i = 0; i < identifier.length(); i++) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, identifier[i]);
  }

  for (i = 0; i < str.length(); i++) {
    glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i]);
  }

  glPopMatrix();
}

void drawGroundRepulsion() {
  glPushMatrix();
  glTranslatef(1500,400,0);
  std::ostringstream strs;
  strs << ropeSimulation->groundRepulsion;
  std::string str = strs.str();
  
  std::string identifier = "Ground Repulsion: ";
  int i = 0;
  for (i = 0; i < identifier.length(); i++) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, identifier[i]);
  }

  for (i = 0; i < str.length(); i++) {
    glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i]);
  }

  glPopMatrix();
}

void drawHUD() {
      // Disable lighting
  glDisable( GL_LIGHTING );

    // Make the HUD a simple rectangle
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );
  glMatrixMode( GL_MODELVIEW );

  // FPS on HUD
  glPushMatrix();
  glLoadIdentity();
  glTranslatef( -90.0f, -90.0f, 0.0f );
  glScalef( 0.05f, 0.05f, 0.05f );
  glColor3f(0,0,0);

  drawSpeed();
  drawGravity();
  drawAirFriction();
  drawGroundAbsorption();
  drawGroundSlideFriction();
  drawGroundRepulsion();

  glPopMatrix();
  
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();

  glEnable( GL_LIGHTING );
}


void displayCallback()
{
  // clear the color buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set up camera
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)(Width)/(GLfloat)(Height),1.0f,100.0f);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
  glTranslatef( 0.0, 0.0, -10.0 );
  switch (camera) {
    case 'w':
    glTranslatef( 0, 0, camZ );
    break;
    case 's':
    glTranslatef( 0, 0, camZ );
    break;

  }

  //glTranslatef(0,-2,0);

  //drawAxis();

  glBegin(GL_QUADS);
  glColor3ub(238, 233, 233);                       
  glVertex3f(20, ropeSimulation->groundHeight-0.05, 20);
  glVertex3f(-20, ropeSimulation->groundHeight-0.05, 20);
  glColor3ub(0, 0, 0);                       
  glVertex3f(-20, ropeSimulation->groundHeight-0.05, -20);
  glVertex3f(20, ropeSimulation->groundHeight-0.05, -20);
  glEnd();

  // Shadow
  glColor3ub(0, 0, 0);
  for (int index = 0; index < ropeSimulation->totalParticles - 1; ++index)
  {
    Particle* particle1 = ropeSimulation->getParticle(index);
    Vector3* pos1 = &particle1->pos;

    Particle* particle2 = ropeSimulation->getParticle(index + 1);
    Vector3* pos2 = &particle2->pos;

    glLineWidth(6);
    glBegin(GL_LINES);
    glVertex3d(pos1->getX(), ropeSimulation->groundHeight-0.025, pos1->getZ());   
    glVertex3d(pos2->getX(), ropeSimulation->groundHeight-0.025, pos2->getZ());
    glEnd();
  }
  // Drawing Shadow Ends Here.
  
  // Rope.
  glColor3ub(255, 133, 0);
  for (int index = 0; index < ropeSimulation->totalParticles - 1; ++index)
  {
    Particle* particle1 = ropeSimulation->getParticle(index);
    Vector3* pos1 = &particle1->pos;

    Particle* particle2 = ropeSimulation->getParticle(index + 1);
    Vector3* pos2 = &particle2->pos;

    glLineWidth(6);
    glBegin(GL_LINES);
    glVertex3d(pos1->getX(), pos1->getY(), pos1->getZ());
    glVertex3d(pos2->getX(), pos2->getY(), pos2->getZ());
    glEnd();
  }

  drawHUD();

  glFlush (); 
  glutSwapBuffers();

}

// not exactly a callback, but sets a timer to call itself
// in an endless loop to update the program
void idle( int value ){

  // set a timer to call this function again after the
  // required number of milliseconds
  tickCount++;
  motionUpdate();
  glutPostRedisplay();
  lastCount = tickCount;
  glutTimerFunc( dt, idle, 0 );

}

int main(int argc, char **argv)
{
  // create window and rendering context
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE );
  glutInitWindowSize( Width, Height );
  glutCreateWindow( "Rope Simulation" );

  srand((int)time(NULL));
  camZ = 0;
  ropeConnectionVel = Vector3(0,0,0);
  ropeSimulation->getParticle(ropeSimulation->totalParticles - 1)->vel[2] = 10.0f;

  // register display callback
  glutDisplayFunc( displayCallback );
  glutKeyboardFunc( keyboardCallback );
  glutKeyboardUpFunc( keyboardUpCallback );
  glutSpecialFunc(SpecialInput);
  glutSpecialUpFunc(SpecialUpInput);
  
  glViewport( 0, 0, Width, Height );
  //glEnable( GL_DEPTH_TEST );
  glDepthFunc(GL_NEVER); 
  glEnable( GL_NORMALIZE );
  
  // Turn on blending (for floor).
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


   //  Enable smooth shading (i.e. gouraud shading)
  glShadeModel(GL_SMOOTH);

  // Set our clear colour to black with full opacity. velocity[1]ntax is (r, g, b, alpha).
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Set the accumulation buffer clearing colour to black at 0,0f alpha
  glClearAccum(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_POINT_SMOOTH);

  // lighting stuff
  GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat diffuse[] = {0.9, 0.9, 0.9, 1.0};
  GLfloat specular[] = {0.4, 0.4, 0.4, 1.0};
  GLfloat position0[] = {1.0, 1.0, 1.0, 0.0};
  glLightfv( GL_LIGHT0, GL_POSITION, position0 );
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
  GLfloat position1[] = {-1.0, -1.0, -1.0, 0.0};
  glLightfv( GL_LIGHT1, GL_POSITION, position1 );
  glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
  glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
  glLightfv( GL_LIGHT1, GL_SPECULAR, specular );
  
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHT1 );
  glEnable( GL_COLOR_MATERIAL );

  // start the idle on a fixed timer callback
  idle( 0 );

  // pass control over to GLUT
  glutMainLoop();
  
  return 0;       // never reached
}

