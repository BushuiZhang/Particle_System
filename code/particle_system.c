////////////////////////////////////////////////////////////////
// Fountain simulator(particle system)                        //
// Author: Bushui Zhang                                       //
// Date 11/16/2020                                            //
////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

// Display list for coordinate axis
GLuint axisList;
#define TURN_ANGLE 4.0
#define MAX_PARTICLES 200000
#define POINT_SIZE 1
#define AXIS_SIZE 200
#define PI 3.14159
#define DEG_TO_RAD 0.017453
#define DEFAULT_LIFETIME 3
#define COLOUR_CHANGE 0.1
#define EJECTION_CHANGE 10
#define MAX_EJECTION 1000
#define VEL_CHANGE 0.1
#define MAX_VEL 20
#define MAX_GRAV 20
#define MIN_GRAV -20
#define GRAV_CHANGE 0.1
#define WIND_CHANGE 0.1
#define TOP_VIEW 1
#define DEG_TO_RAD 0.01745329
#define NORMAL_VIEW 2
#define FLYING_VIEW 3
typedef struct{
  float x;
  float y;
  float z;
} coord;


typedef struct{
  float r;
  float g;
  float b;
} color;
///////////////////////////////////////////////
typedef struct{
  coord pos;
  coord vel;
  color col;
  int lifetime; //three levels
}water_particle;

typedef struct{
  water_particle particles[MAX_PARTICLES];
  int particles_ejection;
  //int discharge;
  int alive;

}fountion_system;

int axisEnabled= 1;
int fC = 0;
int pT = 0;
int cT = 0;
int render_points = 1;
int water_bouncing = 0;
int fps;
float radius = 0.03;
float gravity = -9.81;
float angle = 30;
char textPrinted[60];
float particles_ejection_per_frame = 40;
float initial_water_color_R = 0.9;
float initial_water_color_G = 0.9;
float initial_water_color_B = 0.9;
float initial_vel = 8;
coord normal = {0,1,0};
coord wind_direction = {1, 0, 0};
float wind_speed = 0;
fountion_system fountain;
int current_view;


GLdouble lat,     lon;              /* View angles (degrees)    */
GLfloat  eyex,    eyey,    eyez;    /* Eye point                */
GLfloat  centerx, centery, centerz; /* Look point               */
GLfloat  upx,     upy,     upz;

float myRandom(void)//Return random double within range [0,1]
{
  return (float) rand()/RAND_MAX;
}
//return a random number of [-range, range]
float randomUni(double range)
{
  return range * ((float)rand()/RAND_MAX * 2 - 1.0);
}


void initPaticleSystem()
{
  fountain.particles_ejection = particles_ejection_per_frame;
  fountain.alive = 0;
}


//You could render the particles either with points or solid sphere
void particlesDrawing()
{
  //glBegin (GL_POINTS);
  if(render_points == 0)
  {
    for (int i = 0; i <= fountain.alive; i++)
    {
      glPushMatrix();
      //glVertex3f(fountain.particles[i].pos.x, fountain.particles[i].pos.y, fountain.particles[i].pos.z);
        glColor3f(fountain.particles[i].col.r , fountain.particles[i].col.g, fountain.particles[i].col.b);
        glTranslatef(fountain.particles[i].pos.x, fountain.particles[i].pos.y, fountain.particles[i].pos.z);
        glutSolidSphere(0.015, 10, 10);
      glPopMatrix();

    }
  }
  else
  {
    glBegin (GL_POINTS);
    for (int i = 0; i <= fountain.alive; i++)
    {
      glColor3f(fountain.particles[i].col.r , fountain.particles[i].col.g, fountain.particles[i].col.b);
      glVertex3f(fountain.particles[i].pos.x, fountain.particles[i].pos.y, fountain.particles[i].pos.z);
    }
    glEnd();
  }
}


void createPaticles()
{
  int alive = fountain.alive;
  for(int i = alive; i < fountain.particles_ejection + alive; i++)
  {
    coord record;
    // float r1 = myRandom();
    // float r2 = myRandom();
    // float r3 = myRandom();
    // float r4 = myRandom();
    //
    // float a1 =  randomUni(angle*PI/360);
    // float a2 =  randomUni(angle*PI/360);



    float ratio_square;
    record.y = 0;
    record.x = randomUni(radius-0.02);
    record.z = randomUni(radius-0.02);
    while(pow(record.x,2) + pow(record.z,2) > pow(radius,2))
    {
      record.x = randomUni(radius-0.02);
      record.z = randomUni(radius-0.02);
    }


    float r_square = pow(record.x,2) + pow(record.z,2);
    float r = sqrt(r_square);
    ratio_square = r_square/pow(radius,2);
    float ratio = sqrt(ratio_square);
    float vec_x_d = record.x/r;
    float vec_z_d = record.z/r;
    fountain.particles[i].pos = record;


    // float v_x = sin(r1*a1) * cos(r2  * 2 * PI);
    // float v_z = sin(r1*a1) * sin(r2  * 2 * PI);
    // fountain.particles[i].vel.x = v_x/(sqrt(pow(v_x,2) + pow(v_z,2))) * 5;
    // fountain.particles[i].vel.z = v_z/(sqrt(pow(v_x,2) + pow(v_z,2))) * 5;
    // fountain.particles[i].vel.x = v_x * 5.0;
    // fountain.particles[i].vel.z = v_z * 5.0;
    // float vel_y = 8 * (1 - ratio_square);
    // fountain.particles[i].vel.y = vel_y;


    float vel_y = initial_vel * (1 - ratio_square);
    float vel_xz = vel_y * tan(angle/2 * DEG_TO_RAD);//*ratio;
    float vel_x = vec_x_d * vel_xz;
    float vel_z = vec_z_d * vel_xz;
    fountain.particles[i].vel.y = vel_y;
    fountain.particles[i].vel.x = vel_x;
    fountain.particles[i].vel.z = vel_z;
    fountain.particles[i].col.r = initial_water_color_R;
    fountain.particles[i].col.g = initial_water_color_G;
    fountain.particles[i].col.b = initial_water_color_B;
    fountain.particles[i].lifetime = DEFAULT_LIFETIME;
    //printf("x: %f\n", fountain.particles[i].vel.x);
    fountain.alive++;
  }
}

void updateAllParticles()
{
  int i;
  for(i = 0; i < fountain.alive; i++)
  {
    if(water_bouncing == 1)
    {
      if((fountain.particles[i].pos.y < 0 && fountain.particles[i].lifetime == 0))
      {
        fountain.particles[i] = fountain.particles[fountain.alive-1];
        fountain.alive--;
        //printf("cut: %u\n", fountain.particles[i].pos.y);
      }
      else if(fountain.particles[i].pos.y < 0 && fountain.particles[i].lifetime > 0)
      {
        float dot_vel_nor = fountain.particles[i].vel.x * normal.x
                              + fountain.particles[i].vel.y * normal.y
                              + fountain.particles[i].vel.z * normal.z;
        fountain.particles[i].vel.x = 0.1 * (fountain.particles[i].vel.x - 2 * dot_vel_nor
                                          * normal.x);
        fountain.particles[i].vel.y = 0.1 * (fountain.particles[i].vel.y - 2 * dot_vel_nor
                                          * normal.y);
        fountain.particles[i].vel.z = 0.1 * (fountain.particles[i].vel.z - 2 * dot_vel_nor
                                          * normal.z);
        fountain.particles[i].pos.y = 0;
        fountain.particles[i].lifetime--;
        // fountain.particles[i].vel.z = 0.1 * (fountain.particles[i].vel.z - 2 * dot_vel_nor
        //                                   * fountain.particles[i].vel.z);
        //printf("here: %u\n", fountain.particles[i].pos.y);
      }
      else
      {
        fountain.particles[i].pos.x += (float)(fountain.particles[i].vel.x/fps)
                                        + wind_speed * wind_direction.x/fps;
        //printf("%f\n", fountain.particles[i].pos.x);
        fountain.particles[i].pos.y += (float)(fountain.particles[i].vel.y/fps)
                                        + wind_speed * wind_direction.y/fps;
        fountain.particles[i].pos.z += (float)(fountain.particles[i].vel.z/fps)
                                        + wind_speed * wind_direction.z/fps;
        fountain.particles[i].vel.y += (float)(gravity/fps);
        //printf("x: %f\n", fountain.particles[i].vel.x);
        // printf("y: %f\n",fountain.particles[i].pos.y);
        // printf("%d\n", fountain.alive);
      }
    }
    else
    {
      if((fountain.particles[i].pos.y < 0))
      {
        fountain.particles[i] = fountain.particles[fountain.alive-1];
        fountain.alive--;
      }
      else
      {
        fountain.particles[i].pos.x += (float)(fountain.particles[i].vel.x/fps)
                                        + wind_speed * wind_direction.x/fps;;
        //printf("%f\n", fountain.particles[i].pos.x);
        fountain.particles[i].pos.y += (float)(fountain.particles[i].vel.y/fps);
                                        + wind_speed * wind_direction.y/fps;
        fountain.particles[i].pos.z += (float)(fountain.particles[i].vel.z/fps);
                                        + wind_speed * wind_direction.z/fps;
        fountain.particles[i].vel.y += (float)(gravity/fps);
      }
    }
  }
}

//////////////////////////////////////////////
void countFPS()
{
  fC++;
  //time_interval in milliseconds.
  int tI = cT - pT;
  float sec;
  cT= glutGet(GLUT_ELAPSED_TIME);
  if(tI >= 500)
  {
    //printf("%i\n",fC);
    sec = (float)tI/1000;
    fps = fC/sec;
    fC = 0;
    pT = cT;
  }
}

void drawFloor()
{
  int x,z;
  int L = 20;
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glDepthRange (0.1, 1.0);
  glColor3f(0.4, 0.4, 0.4);
  glBegin(GL_QUADS);
  glVertex3f(-L, 0, -L);
  glVertex3f(L, 0, -L);
  glVertex3f(L, 0, L);
  glVertex3f(-L, 0, L);
  glEnd();

  glDepthRange (0.0, 0.9);
  glColor3f(0.2, 0.2, 0.2);
  glLineWidth(1.0);
  glBegin(GL_LINES);
  for (x= -L; x <= L; x++)
  {
    glVertex3f((GLfloat) x, 0.01, -L);
    glVertex3f((GLfloat) x, 0.01,  L);
   }
   for (z= -L; z <= L; z++)
   {
     glVertex3f(-L, 0.01, (GLfloat) z);
     glVertex3f( L, 0.01, (GLfloat) z);
   }
   glEnd();

}

void calculate_lookpoint(void)
{ /* Given an eyepoint and latitude and longitude angles, will
     compute a look point one unit away */

    centery = eyey + 1 * sin((lat)*DEG_TO_RAD);
    centerz = eyez + 1 * cos((lon)*DEG_TO_RAD) * cos((lat)*DEG_TO_RAD);
    centerx = eyex + 1 * sin((lon)*DEG_TO_RAD) * cos((lat)*DEG_TO_RAD);
}
void printString(char* stringPrinted, float x, float y)
{
  glRasterPos2f(x,y);
  char* c;
  for (c = stringPrinted; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void printText()
{
  countFPS();
  glColor4f(1, 1, 1, 1);
  //sprintf(textPrinted, "fps: %u", fps);
  sprintf(textPrinted, "Particles alive: %u", fountain.alive);
  printString(textPrinted, -7, 5.8);
  sprintf(textPrinted, "FPS: %u", fps);
  printString(textPrinted, -7, 5.6);
  sprintf(textPrinted, "Gravity: %.2f", gravity);
  printString(textPrinted, -7.05, 5.4);
  sprintf(textPrinted, "Wind speed: %.2f", wind_speed);
  printString(textPrinted, -7.1, 5.2);
  sprintf(textPrinted, "Max Inital speed: %.2f", initial_vel);
  printString(textPrinted, -7.1, 5.0);
  //glRasterPos2f(-12,10.5);
  // char* c;
  // for (c = textPrinted; *c != '\0'; c++)
  //   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  //printf("%d\n", fps);

}

///////////////////////////////////////////////
void setView()
{

  switch(current_view){
    case TOP_VIEW:
      gluLookAt(0, 20,  0,//Set camera position
                0,  0,  0,//Set center position
                1,  0,  0);//Set up position
      break;
    case NORMAL_VIEW:
      gluLookAt(2, 1.8, 10.0,
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0);
      break;
    case FLYING_VIEW:
      calculate_lookpoint();
      gluLookAt(eyex,    eyey,    eyez,//Set camera position
                centerx, centery, centerz,//Set center position
                upx,     upy,     upz);
      break;

    }
}

void display()
{
  glLoadIdentity();
  // gluLookAt(0, 1.8, 10.0,
  //           0.0, 0.0, 0.0,
  //           0.0, 1.0, 0.0);
  setView();
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawFloor();
  createPaticles();
  particlesDrawing();
  updateAllParticles();
  glutPostRedisplay();
  // If enabled, draw coordinate axis
  if(axisEnabled)
    glCallList(axisList);
  printText();
  glutSwapBuffers();

}

///////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case  27: exit(0);
              break;
    case 'r': if (initial_water_color_R - COLOUR_CHANGE >= 0.0)
                initial_water_color_R  -= COLOUR_CHANGE;
              break;
    case 'R': if (initial_water_color_R + COLOUR_CHANGE <= 1.0)
                initial_water_color_R += COLOUR_CHANGE;
              break;
    case 'g': if (initial_water_color_G - COLOUR_CHANGE >= 0.0)
                initial_water_color_G -= COLOUR_CHANGE;
              break;
    case 'G': if (initial_water_color_G + COLOUR_CHANGE <= 1.0)
                initial_water_color_G += COLOUR_CHANGE;
              break;
    case 'b': if (initial_water_color_B - COLOUR_CHANGE >= 0.0)
                initial_water_color_B -= COLOUR_CHANGE;
              break;
    case 'B': if (initial_water_color_B + COLOUR_CHANGE <= 1.0)
                initial_water_color_B += COLOUR_CHANGE;
              break;
    case 'e': if (fountain.particles_ejection -  EJECTION_CHANGE >= 0)
                fountain.particles_ejection -= EJECTION_CHANGE;
              break;
    case 'E': if (fountain.particles_ejection +  EJECTION_CHANGE <= MAX_EJECTION)
                fountain.particles_ejection += EJECTION_CHANGE;
              break;
    case 'v': if (initial_vel -  VEL_CHANGE >= 0)
                initial_vel -= VEL_CHANGE;
              break;
    case 'V': if(initial_vel +  VEL_CHANGE <= MAX_VEL)
                initial_vel += VEL_CHANGE;
              break;
    case 'y': if (gravity - GRAV_CHANGE >= MIN_GRAV)
                 gravity -= GRAV_CHANGE;
              break;
    case 'Y': if(gravity +  GRAV_CHANGE <= MAX_GRAV)
                 gravity += GRAV_CHANGE;
              break;
    case 'w': wind_speed -=  WIND_CHANGE;
              break;
    case 'W': wind_speed += WIND_CHANGE;
              break;
  }
  glutPostRedisplay();
}

///////////////////////////////////////////////

void reshape(int width, int height)
{
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

void menu(int option){
  switch(option)
  {
    case 1: glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            initPaticleSystem();
            break;
    case 2: axisEnabled = 1 - axisEnabled;
            break;
    case 3: glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            initPaticleSystem();
            render_points = 1 - render_points;
            break;
    case 4: water_bouncing = 1 - water_bouncing;
            break;
    case 5: current_view = NORMAL_VIEW;
            break;
    case 6: current_view = TOP_VIEW;
            break;
    case 7: current_view = FLYING_VIEW;
            break;



  }
}

void menuCreation()
{
  glutCreateMenu(menu);
  glutAddMenuEntry ("Restart fountain", 1);
  glutAddMenuEntry ("On/off axis", 2);
  glutAddMenuEntry ("rendering with points/solid spheres", 3);
  glutAddMenuEntry ("Off/on water bouncing", 4);
  glutAddMenuEntry ("normal view", 5);
  glutAddMenuEntry ("top view", 6);
  glutAddMenuEntry ("flying view", 7);
  glutAttachMenu (GLUT_RIGHT_BUTTON);


}



///////////////////////////////////////////////

void makeAxes()
{
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
  glEndList();
}

void cursor_keys(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_LEFT:lon = lon + TURN_ANGLE;
                        break;
    case GLUT_KEY_RIGHT:lon = lon - TURN_ANGLE;
                        break;
    case GLUT_KEY_PAGE_UP:if(lat + TURN_ANGLE < 90)
                            lat = lat + TURN_ANGLE;
                        break;
    case GLUT_KEY_PAGE_DOWN:if(lat - TURN_ANGLE > -90)
                              lat = lat - TURN_ANGLE;
                        break;
    case GLUT_KEY_HOME:lat = 0;
                        break;
    case GLUT_KEY_UP:eyez = eyez + cos(lat*DEG_TO_RAD) * cos((lon)*DEG_TO_RAD);
                     eyey = eyey + sin(lat*DEG_TO_RAD);
                     eyex = eyex + cos(lat*DEG_TO_RAD) * sin((lon)*DEG_TO_RAD);
                        break;
    case GLUT_KEY_DOWN: eyez = eyez - cos((lon)*DEG_TO_RAD);
                        eyex = eyex - sin((lon)*DEG_TO_RAD);
                        break;
      /* To be completed */
  }
}

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  eyex = 2; /* Set eyepoint at eye height within the scene */
  eyey = 1.8;
  eyez = -10;

  upx = 0.0;   /* Set up direction to the +Y axis  */
  upy = 1.0;
  upz = 0.0;

  lat = 0.0;   /* Look horizontally ...  */
  lon = 0.0;
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(0, 0);
  current_view = NORMAL_VIEW;
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("COMP37111 Particles");
  glutDisplayFunc(display);
  //glutIdleFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(cursor_keys);
  menuCreation();
  glutReshapeFunc(reshape);
  makeAxes();
  glEnable(GL_POINT_SMOOTH);
  glPointSize(POINT_SIZE);
}

/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  srand(time(NULL));
  initPaticleSystem();
  initGraphics(argc, argv);
  glEnable(GL_POINT_SMOOTH);
  glutMainLoop();
}
