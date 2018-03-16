/*
==========================================================================
File:        ex2.c (skeleton)
Version:     5, 19/12/2017
Author:     Toby Howard
==========================================================================
*/

/* The following ratios are not to scale: */
/* Moon orbit : planet orbit */
/* Orbit radius : body radius */
/* Sun radius : planet radius */

#ifdef MACOS
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BODIES 25
#define TOP_VIEW 1
#define ECLIPTIC_VIEW 2
#define SHIP_VIEW 3
#define EARTH_VIEW 4
#define PI 3.141593
#define DEG_TO_RAD 0.01745329
#define ORBIT_POLY_SIDES 50
#define TIME_STEP 0.5   /* days per frame */
#define RUN_SPEED  1000000.0
#define TURN_ANGLE 1.0

// Initialised required rotation variables.
float rotX, rotY, rotZ;
float x_Points[1000], y_Points[1000], z_Points[1000];
GLfloat eyex, eyey, eyez;
GLfloat lat, lon;              /* View angles (degrees)    */
GLfloat  centerx, centery, centerz; /* Look point               */
GLfloat  upx,     upy,     upz;     /* View up vector           */
GLint width= 1000, height= 1200;      /* size of window           */

typedef enum { False, True } bool;
bool axisEnabled = False;
bool earthMode = False;

typedef struct {
  char    name[25];       /* name */
  GLfloat r,g,b;          /* colour */
  GLfloat orbital_radius; /* distance to parent body (km) */
  GLfloat orbital_tilt;   /* angle of orbit wrt ecliptic (deg) */
  GLfloat orbital_period; /* time taken to orbit (days) */
  GLfloat radius;         /* radius of body (km) */
  GLfloat axis_tilt;      /* tilt of axis wrt body's orbital plane (deg) */
  GLfloat rot_period;     /* body's period of rotation (days) */
  GLint   orbits_body;    /* identifier of parent body */
  GLfloat spin;           /* current spin value (deg) */
  GLfloat orbit;          /* current orbit value (deg) */
 } body;

body  bodies[MAX_BODIES];
int   numBodies, current_view, draw_orbits, draw_labels, draw_starfield, draw_string;
float  date;

/*****************************/

float myRand (void)
{
  /* return a random float in the range [0,1] */

  return (float) rand() / RAND_MAX;
}

/*****************************/
// Function to plot x number of points randomly.
void generatePoints (int size)
{
  // Scale to 600 000 000 km
  float scaleFactor = 600000000;
  float randomX, randomY, randomZ;

  int i;
  for (i = 0; i < size; i++)
  {
    randomX = (rand()/(float)(RAND_MAX)) * 2 - 1;
    randomY = (rand()/(float)(RAND_MAX)) * 2 - 1;
    randomZ = (rand()/(float)(RAND_MAX)) * 2 - 1;

    x_Points[i] = scaleFactor * myRand() * randomX;
    y_Points[i] = scaleFactor * myRand() * randomY;
    z_Points[i] = scaleFactor * myRand() * randomZ;
  }

}

void drawStarfield (void)
{
  /* This is for you to complete. */
  int i;

  // Make the stars color white.
  glColor3f(255.0,255.0,255.0);

  glBegin(GL_POINTS);
  for (i = 0; i < 1000; i++)
  {
    glVertex3f((GLfloat) x_Points[i],(GLfloat) y_Points[i],(GLfloat) z_Points[i]);
  }
  glEnd();

}
/*****************************/

void readSystem(void)
{
  /* reads in the description of the solar system */

  FILE *f;
  int i;

  f= fopen("sys", "r");
  if (f == NULL) {
     printf("ex2.c: Couldn't open 'sys'\n");
     printf("To get this file, use the following command:\n");
     printf("  cp /opt/info/courses/COMP27112/ex2/sys .\n");
     exit(0);
  }
  fscanf(f, "%d", &numBodies);
  for (i= 0; i < numBodies; i++)
  {
    fscanf(f, "%s %f %f %f %f %f %f %f %f %f %d",
      bodies[i].name,
      &bodies[i].r, &bodies[i].g, &bodies[i].b,
      &bodies[i].orbital_radius,
      &bodies[i].orbital_tilt,
      &bodies[i].orbital_period,
      &bodies[i].radius,
      &bodies[i].axis_tilt,
      &bodies[i].rot_period,
      &bodies[i].orbits_body);

    /* Initialise the body's state */
    bodies[i].spin= 0.0;
    bodies[i].orbit= myRand() * 360.0; /* Start each body's orbit at a
                                          random angle */
    bodies[i].radius*= 1000.0; /* Magnify the radii to make them visible */
  }
  fclose(f);
}

/*****************************/

void drawString (void *font, float x, float y, float z, char *str)
{ /* Displays the string "str" at (x,y,0), using font "font" */

  /* This is for you to complete. */
  char *ch;
  glRasterPos3f(x, y, z);
  for (ch =str; *ch; ch++)
    glutBitmapCharacter(font, (int)*ch);
}

/*****************************/

void calculate_lookpoint(void) 
{
  // Converting lat and lon to radians.
  GLfloat lat_RAD = lat * DEG_TO_RAD;
  GLfloat lon_RAD = lon * DEG_TO_RAD;

  GLfloat dir_x = cos(lat_RAD) * sin(lon_RAD);
  GLfloat dir_y = sin(lat_RAD);
  GLfloat dir_z = cos(lat_RAD) * cos(lon_RAD);

  centerx = (GLfloat) eyex + dir_x * 600000000;
  centery = (GLfloat) eyey + dir_y * 600000000;
  centerz = (GLfloat) eyez + dir_z * 600000000;

} // calculate_lookpoint()


void setView (void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  switch (current_view) {
  // Look 90 angle from the top.
  case TOP_VIEW:
    gluLookAt(0.0, 600000000.0, 1.0,  // camera position
	            0.0, 0.0, 0.0,    // point to look at
              0.0, 1.0, 0.0 );  // "upright" vector
    break;

  // Look from the same height, horizontal view
  case ECLIPTIC_VIEW:
    gluLookAt(270000000.0, 0, 270000000.0,  // camera position
	            0.0, 0.0, 0.0,    // point to look at
              0.0, 1.0, 0.0 );  // "upright" vector
    break;
  // Look from inside a ship
  case SHIP_VIEW:
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
    break;

  // Look to the sun always from earth
  case EARTH_VIEW:
    earthMode = !earthMode;

    // Get new x and y positions of the planet.
    GLfloat xPostion = bodies[3].orbital_radius * cos(bodies[3].orbit * DEG_TO_RAD);
    GLfloat yPostion = bodies[3].orbital_radius * sin(bodies[3].orbit * DEG_TO_RAD);

    gluLookAt(xPostion, bodies[3].radius * 1.05, yPostion,  // camera position
    	        0.0, 0, 0.0,    // point to look at
              0.0, 1.0, 0.0 );  // "upright" vector
    break;
  }
}

/*****************************/

void menu (int menuentry) {
  switch (menuentry) {
  case 1: current_view= TOP_VIEW;
          break;
  case 2: current_view= ECLIPTIC_VIEW;
          break;
  case 3: current_view= SHIP_VIEW;
          // Set initial values for gluLookAt.
          eyex = 85000000.0; eyey = 69000000.0; eyez = -400000000.0;    
          centerx = 85000000.0; centery =  69000000.0; centerz =  201000000.0;
          upx= 0.0; upy= 1.0; upz= 0.0;
          lat= 0.0; lon= 0.0;
          break;
  case 4: current_view= EARTH_VIEW;
          break;
  case 5: draw_labels= !draw_labels;
          break;
  case 6: draw_orbits= !draw_orbits;
          break;
  case 7: draw_starfield= !draw_starfield;
          break;
  case 8: exit(0);
          break;
  // 2 new cases for displaying axis and labels.
  case 9: axisEnabled =! axisEnabled;
          break;
  case 10: draw_string =! draw_string;
          break;
  }
}

/*****************************/
void init(void)
{
  generatePoints(1000);
  /* Define background colour */
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glutCreateMenu (menu);
  glutAddMenuEntry ("Top view", 1);
  glutAddMenuEntry ("Ecliptic view", 2);
  glutAddMenuEntry ("Spaceship view", 3);
  glutAddMenuEntry ("Earth view", 4);
  glutAddMenuEntry ("Display axis", 9);
  glutAddMenuEntry ("Toggle labels", 5);
  glutAddMenuEntry ("Toggle orbits", 6);
  glutAddMenuEntry ("Toggle starfield", 7);
  glutAddMenuEntry ("Toggle welcome", 10);
  glutAddMenuEntry ("Quit", 8);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

  current_view= TOP_VIEW;
  draw_labels= 1;
  draw_orbits= 1;
  draw_starfield= 1;
}

/*****************************/

void animate(void)
{
  int i;

    date+= TIME_STEP;

    for (i= 0; i < numBodies; i++)  {
      bodies[i].spin += 360.0 * TIME_STEP / bodies[i].rot_period;
      bodies[i].orbit += 360.0 * TIME_STEP / bodies[i].orbital_period;
      glutPostRedisplay();
    }
}

/*****************************/

void drawOrbit (int n)
{
  float angleSegment = 2 * PI / ORBIT_POLY_SIDES;
  float i;

  // Start polygon mode.
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_POLYGON);

  if (bodies[n].orbits_body == 0)
  {
    // Make color of orbit line same as planet color.
    glColor3f(bodies[n].r,bodies[n].g,bodies[n].b);
    GLfloat yPostion = (bodies[n].orbital_radius * 
                        sin(bodies[n].orbital_tilt * DEG_TO_RAD));

    for (i = 0; i < 2 * PI; i+=angleSegment)
    {
      glVertex3f(bodies[n].orbital_radius * sin(i), yPostion * cos(i),
                 bodies[n].orbital_radius * cos(i));
    }
  }
  else
  {
    // Get index of the parent location.
    int parentIndex =  bodies[n].orbits_body;

    // Get the current position of the parent body.
    GLfloat xPostion = (bodies[parentIndex].orbital_radius * 
                                   cos(bodies[parentIndex].orbit * DEG_TO_RAD));
    GLfloat yPostion = (bodies[parentIndex].orbital_radius * 
                            sin(bodies[parentIndex].orbital_tilt * DEG_TO_RAD));
    yPostion *= sin(DEG_TO_RAD * bodies[parentIndex].orbit);
    GLfloat zPostion = (bodies[parentIndex].orbital_radius * 
                        sin(bodies[parentIndex].orbit * DEG_TO_RAD));

    // Make color of orbit line same as planet color.
    glColor3f(bodies[n].r,bodies[n].g,bodies[n].b);

    // Draw the orbit path by adding offset to parent body
    for (i = 0; i < 2 * PI; i+=angleSegment)
    {
      // Calculating offset.
      GLfloat y_Offset = bodies[n].orbital_radius * sin(bodies[n].orbital_tilt * DEG_TO_RAD);
      y_Offset *= cos(i);

      glVertex3f(xPostion + (bodies[n].orbital_radius * sin(i)), 
                 yPostion + y_Offset,
                 zPostion + (bodies[n].orbital_radius * cos(i)));
    }
  }
  glEnd();
}

/*****************************/

void drawLabel(int n)
{
  GLfloat length = bodies[n].radius * 1.4;
  glColor3f(bodies[n].r, bodies[n].g, bodies[n].b);

  if (bodies[n].orbits_body == 0)
  {
    // Calculate the current planet location.
    GLfloat yPosition = (bodies[n].orbital_radius * sin(bodies[n].orbital_tilt 
                         * DEG_TO_RAD));
    yPosition *= sin(DEG_TO_RAD * bodies[n].orbit);

    GLfloat xPosition = bodies[n].orbital_radius * cos(bodies[n].orbit * DEG_TO_RAD);
    GLfloat zPosition = bodies[n].orbital_radius * sin(bodies[n].orbit * DEG_TO_RAD);

    drawString(GLUT_BITMAP_HELVETICA_18, xPosition + length, yPosition + length, 
               zPosition + length, bodies[n].name);
  }
  else
  {
    // Get index of the parent location.
    int parentIndex =  bodies[n].orbits_body;

    // Get the current position of the parent body.
    GLfloat xPosition = (bodies[parentIndex].orbital_radius * 
                         cos(bodies[parentIndex].orbit * DEG_TO_RAD));
    GLfloat yPosition = (bodies[parentIndex].orbital_radius * 
                            sin(bodies[parentIndex].orbital_tilt * DEG_TO_RAD));
    yPosition *= sin(DEG_TO_RAD * bodies[parentIndex].orbit);

    GLfloat zPosition = (bodies[parentIndex].orbital_radius * 
                                  sin(bodies[parentIndex].orbit * DEG_TO_RAD));

    GLfloat y_Offset = (bodies[n].orbital_radius * 
                                      sin(bodies[n].orbital_tilt * DEG_TO_RAD));
    y_Offset *= sin(DEG_TO_RAD * bodies[n].orbit);

    // Get new x and y positions of the planet by adding it to the offset.
    xPosition += bodies[n].orbital_radius * cos(bodies[n].orbit * DEG_TO_RAD);
    yPosition += y_Offset;
    zPosition += bodies[n].orbital_radius * sin(bodies[n].orbit * DEG_TO_RAD);

    drawString(GLUT_BITMAP_HELVETICA_18, xPosition + length, yPosition + length, 
               zPosition + length, bodies[n].name);
  }
}

/*****************************/

void drawBody(int n)
{
  // To draw the planet
  if (bodies[n].orbits_body == 0)
  {
    GLfloat yPosition = (bodies[n].orbital_radius * 
                         sin(bodies[n].orbital_tilt * DEG_TO_RAD));
    yPosition *= sin(DEG_TO_RAD * bodies[n].orbit);

    // Get new x and z positions of the planet.
    GLfloat xPosition = (bodies[n].orbital_radius * 
                         cos(bodies[n].orbit * DEG_TO_RAD));

    GLfloat zPosition = (bodies[n].orbital_radius * 
                         sin(bodies[n].orbit * DEG_TO_RAD));

    glTranslatef(xPosition, yPosition, zPosition);

    // Set the self rotation for the planet.
    glRotatef(90,1,0,0);
    glRotatef(bodies[n].spin, 0, 0, 1);
    glRotatef(bodies[n].axis_tilt,0,1,0);


    glRotatef(270.0,1,0,0);
    glColor3f(bodies[n].r, bodies[n].g, bodies[n].b);
      glBegin(GL_LINES);
        glVertex3f(0, -2.5 * bodies[n].radius, 0);
        glVertex3f(0, 2.5 * bodies[n].radius, 0);
      glEnd();

    // Display the planet as a sphere.
    glutWireSphere(bodies[n].radius, 50, 50);
  }

  // To draw the moon.
  else
  {
    // Get index of the parent location.
    int parentIndex =  bodies[n].orbits_body;

    // Get the current position of the parent body.
    GLfloat xPostion = (bodies[parentIndex].orbital_radius * 
                        cos(bodies[parentIndex].orbit * DEG_TO_RAD));
    GLfloat yPostion = (bodies[parentIndex].orbital_radius * 
                        sin(bodies[parentIndex].orbital_tilt * DEG_TO_RAD));

    yPostion *= sin(DEG_TO_RAD * bodies[parentIndex].orbit);

    GLfloat zPostion = (bodies[parentIndex].orbital_radius * 
                        sin(bodies[parentIndex].orbit * DEG_TO_RAD));

    // Calculate the offset.
    GLfloat y_Offset = (bodies[n].orbital_radius * 
                        sin(bodies[n].orbital_tilt * DEG_TO_RAD));
    y_Offset *= sin(DEG_TO_RAD * bodies[n].orbit);

    // Get new x, y and z positions of the planet by adding it to the offset.
    xPostion += bodies[n].orbital_radius * cos(bodies[n].orbit * DEG_TO_RAD);
    yPostion += y_Offset;
    zPostion += bodies[n].orbital_radius * sin(bodies[n].orbit * DEG_TO_RAD);
    glTranslatef(xPostion, yPostion, zPostion);

    glRotatef(270.0,1,0,0);
    glColor3f(bodies[n].r, bodies[n].g, bodies[n].b);
      glBegin(GL_LINES);
        glVertex3f(0, -2.5 * bodies[n].radius, 0);
        glVertex3f(0, 2.5 * bodies[n].radius, 0);
      glEnd();

    glutWireSphere(bodies[n].radius, 50, 50);
  }
}

/*****************************/
// Function to draw 3 axis. x, y and z
void drawAxes(void) 
{
  float axis_length = 800000000.0;

  glLineWidth(10.0);

  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0); // red, x-axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(axis_length, 0.0, 0.0);

  glColor3f(0.0,1.0,0.0); // green, y-axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, axis_length, 0.0);

  glColor3f(0.0,0.0,1.0); // blue, z-axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, axis_length);
  glEnd();

   glLineWidth(1.0);
}

void earthEye(void)
{
  // Get new x and y positions of the planet.
  GLfloat xPostion = bodies[3].orbital_radius * cos(bodies[3].orbit * DEG_TO_RAD);
  GLfloat yPostion = bodies[3].orbital_radius * sin(bodies[3].orbit * DEG_TO_RAD);

  // Z is zero since earth is horizontal.
  gluLookAt(xPostion, 0, yPostion,  // camera position
  	        0.0, 0, 0.0,    // point to look at
            0.0, 1.0, 0.0 );  // "upright" vector
}

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (axisEnabled) drawAxes ();
  /* set the camera */
  setView();

  if (draw_starfield) drawStarfield();

  // To check for displaying welcome string
  if (draw_string)
  {
    GLfloat length = bodies[0].radius * 1.4;
    drawString(GLUT_BITMAP_HELVETICA_18, -length, -length, -length, 
               "Welcome to Khoi's solar system!");
  }

  for (i= 0; i < numBodies; i++)
  {
    glPushMatrix();

    if (draw_orbits) drawOrbit(i);
    if (draw_labels) drawLabel(i);
    drawBody(i);
    glPopMatrix();
  }
  glutSwapBuffers();
}

/*****************************/

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (48.0, (GLfloat)w/(GLfloat)h,  10000.0, 800000000.0);
}

/*****************************/

void keyboard(unsigned char key, int x, int y)
{
  double lon_RAD = (lon) * DEG_TO_RAD;

  switch(key)
  {
    case 27:  /* Escape key */
      exit(0);
      break;
    case 'a':
    case 'A':
      axisEnabled = !axisEnabled;
      break;
    case '4':  /* Left Key */
      lon += TURN_ANGLE;
      break;
    case '6':  /* Right key */
      lon -= TURN_ANGLE;
      break;
    case '-':  /* Page Up key */
      lat += TURN_ANGLE;
      break;
    case '+':  /* Page Down key */
      lat -= TURN_ANGLE;
      break;
    case '5':  /* Down key */
      eyex -= RUN_SPEED * sin(lon_RAD);
      eyez -= RUN_SPEED * cos(lon_RAD);
      break;
    case '8':  /* Up key */
      eyex += RUN_SPEED * sin(lon_RAD);
      eyez += RUN_SPEED * cos(lon_RAD);
      break;
    case '0':  /* Reset */
      lat = 0;
      break;
  }
  calculate_lookpoint();
}

/*****************************/

int main(int argc, char** argv)
{
  rotX= rotY= rotZ= 0.0; // no rotation
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow ("COMP27112 Exercise 2");
  //glutFullScreen();
  glutInitWindowSize(width, height);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(animate);
  readSystem();
  glutMainLoop();
  return 0;
}
/* end of ex2.c */
