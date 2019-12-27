
#include <iostream>
#include <ctime>
#include <vector>
#include <glut.h>

using namespace std;
static GLuint texName[6];
bool t=0;
bool l=0;
class Vector
{
public:
	Vector(float X = 0.0, float Y = 0.0, float Z = 0.0)
		: x(X), y(Y), z(Z)
	{}

	static Vector Cross(const Vector& vec1, const Vector& vec2)
	{
		Vector vecCross;

		vecCross.x = vec1.y * vec2.z - vec1.z * vec2.y;
		vecCross.y = vec1.z * vec2.x - vec1.x * vec2.z;
		vecCross.z = vec1.x * vec2.y - vec1.y * vec2.x;

		vecCross.Normalize();
		return vecCross;
	}

	float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	void Normalize()
	{
		float Len = Length();
		x /= Len;
		y /= Len;
		z /= Len;
	}

	float x, y, z;
};
Vector vecEye(0, 0, 0);
Vector vecViewDirection(0, 0, -1);
Vector vecUp(0, 1, 0);
float angle = 0.0;
float eyeX = 0, eyeY = 1, eyeZ = 3;
float centerX = 0.0f, centerY = 1.0f, centerZ = 0.0f;

float width, height;

const float MOVEMENT_SPEED = 0.1;
float angleY = 0.0;
unsigned char* ppmRead(char* filename, int* width, int* height) {

	FILE* fp;
	int i, w, h, d;
	unsigned char* image;
	char head[70];		// max line <= 70 in PPM (per spec).

	fp = fopen(filename, "rb");
	if (!fp) {
		perror(filename);
		return NULL;
	}

	// Grab first two chars of the file and make sure that it has the
	// correct magic cookie for a raw PPM file.
	fgets(head, 70, fp);
	if (strncmp(head, "P6", 2)) {
		fprintf(stderr, "%s: Not a raw PPM file\n", filename);
		return NULL;
	}

	// Grab the three elements in the header (width, height, maxval).
	i = 0;
	while (i < 3) {
		fgets(head, 70, fp);
		if (head[0] == '#')		// skip comments.
			continue;
		if (i == 0)
			i += sscanf(head, "%d %d %d", &w, &h, &d);
		else if (i == 1)
			i += sscanf(head, "%d %d", &h, &d);
		else if (i == 2)
			i += sscanf(head, "%d", &d);
	}

	// Grab all the image data in one fell swoop.
	image = (unsigned char*)malloc(sizeof(unsigned char)* w * h * 3);
	fread(image, sizeof(unsigned char), w * h * 3, fp);
	fclose(fp);

	*width = w;
	*height = h;
	return image;
}

void renderBitmapString(float x, float y, float z, void* font, const char* string)
{
	const char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);
}

void draw_axes()
{
	glLineWidth(2.0);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0); // x-axis
		glVertex3f(0, 0, 0);
		glVertex3f(.15, 0, 0);

		glColor3f(0, 1, 0); // y-axis
		glVertex3f(0, 0, 0);
		glVertex3f(0, .15, 0);

		glColor3f(0, 0, 1);// z-axis
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, .15);
	glEnd();
	glLineWidth(1.0);
}
void draw_floor_tex()
{
	if(t)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	
	glPushMatrix();
	glScalef(1, 1, 1.05);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0, 1.55);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.91,0, 1.55);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.91, 0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}
void draw_floor()
{
	
	glColor4f(0.824, 0.706, 0.549, 1);
	glPushMatrix();
	
	glScalef(8, 0.01, 4);
	glBegin(GL_QUADS);
		glVertex3f(-0.5, 0, 1.55);
		glVertex3f(0.91, 0, 1.55);
		glVertex3f(0.91, 0, -0.5);
		glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();
	
	
}
void draw_wall_tex()
{
	
	if(t)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	glPushMatrix();
	
	glScalef(8, 0.01, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0, 1.55);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.91,0, 1.55);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.91, 0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void draw_roof()
{
	
	glColor4f(0.824, 0.706, 0.549, 1);
	glPushMatrix();
	
	glScalef(8, 0.01, 4);
	glBegin(GL_QUADS);
		glVertex3f(-0.5, 0, 1.55);
		glVertex3f(0.91, 0, 1.55);
		glVertex3f(0.91, 0, -0.5);
		glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();
	
}
void draw_wall()
{
	
	glColor4f(0.961, 0.961, 0.863, 1);
	glPushMatrix();
	glScalef(4, 0.01, 6.2);
	glutSolidCube(1.0);
	glPopMatrix();
	
}
void draw_stairs()
{
	glColor4f(0.545, 0.271, 0.075,1);
	glPushMatrix();
	glScalef(2.1, 0.5, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, -0.8);
	glScalef(2.1, 0.5, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, -1.6);
	glScalef(2.1, 0.5, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.5, -2.4);
	glScalef(2.1, 0.5, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 2, -3.2);
	glScalef(2.1, 0.5, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 2.5, -4);
	glScalef(2.1, 0.5, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 3, -5.1);
	glScalef(2.1, 0.5, 1.9);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, 3.3, -5.4);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(1.2, 0.4, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_window_tex()
{
	if(t)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	
	glPushMatrix();
	glScalef(2.3, 0.01, 0.7);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0, 1.55);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.91,0, 1.55);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.91, 0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}
void draw_window()
{
	glColor4f(0.678, 0.847, 0.902,1);
	glPushMatrix();
	glScalef(2.5, 1.5, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_pic()
{
	glColor4f(	0,0,0,0);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glScalef(6, 0.01, 5);
	if(t)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0, 1.55);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.91,0, 1.55);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.91, 0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void draw_carpet()
{
	glColor4f(	0.863, 0.863, 0.863,1);
	glPushMatrix();
	glScalef(6.5, 3.5, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();
	

}
void draw_TV()
{
	glColor4f(0, 0, 0, 0);
	glPushMatrix();
	glScalef(2, 1.5, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_tv_tex()
{
	if(t)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	
	glPushMatrix();
	glScalef(1.5, 0.01, 0.7);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0, 1.55);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.91,0, 1.55);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.91, 0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}
void draw_couch()
{
	glColor4f(0.412, 0.412, 0.412, 1);
	glPushMatrix();
	glScalef(4, 0.5, 1.5);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(3.5, 0.5, 1.5);
	glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.184, 0.310, 0.310, 1);
	glPushMatrix();
	glTranslatef(-0.7, 0.5, -0.8);
	glScalef(5.2, 1.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glTranslatef(1, -0.5, -3.2);
	glScalef(4, 1.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.1, 0.28, 0);
	glScalef(0.5, 1, 1.5);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, 0.28, 3);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(0.5, 1, 1.5);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_frame()
{
	glColor4f(0, 0, 0, 0);
	glPushMatrix();
	glScalef(1, 1.3, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_lamp()
{
	if(l)
	{
	glEnable(GL_LIGHTING); //Enable lighting
   
	   glEnable(GL_LIGHT0); //Enable light #0
	}
   else
	   glDisable(GL_LIGHT0);
	glColor4f(1,1,1,1);
	glPushMatrix();
	glScalef(0.08, 0.8, 0.08);
	glutSolidCube(1.0);
	glPopMatrix();

	
    glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	GLfloat diffuseLightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat specularLightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat lightPos0[] = {0.0f, -1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLightColor0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glutSolidSphere(.1,100,100);
	glPopMatrix();

	
	glColor4d(0.184, 0.310, 0.310,0);
	glPushMatrix();
	glRotatef(90,1,0,0);
	gluCylinder(gluNewQuadric(), .2, .5, .5, 30, 30);
	glPopMatrix();	
	
}
void draw_table()
{
	glColor4f(0,0,0,0);
	glPushMatrix();
	glScalef(4, 0.5, 1.8);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_table_tex()
{
	if(t)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	
	glPushMatrix();
	glScalef(2.8, 0.01, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0, 1.55);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.91,0, 1.55);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.91, 0, -0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, 0, -0.5);
	glEnd();
	glPopMatrix();

	
	glDisable(GL_TEXTURE_2D);

}
void draw_table_legs()
{
	glColor4f(0,0,0,0);
	glPushMatrix();
	glScalef(0.1, 0.7, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -1.7);
	glScalef(0.1, 0.7, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.75, 0, 0);
	glScalef(0.1, 0.7, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.75, 0, -1.7);
	glScalef(0.1, 0.7, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
}
void draw_cushion()
{
	
	glColor4f(0, 0, 0, 0);
	glPushMatrix();
	glRotatef(90, 1, 0, 1);
	glRotatef(90, 0, 1, 1);
	glScalef(0.5, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
}
void init ()
{
	
	glClearColor (0.0, 0.0, 0.0, 0.0);	// set display-window color to white
	
	glEnable(GL_LINE_SMOOTH);
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 80.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	  glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH); //Enable smooth shading
	int width, height;
	unsigned char* image[6];
	image[0]= ppmRead("floor.ppm", &width, &height);
	image[1] = ppmRead("wall.ppm", &width, &height);
	image[2] = ppmRead("pic.ppm", &width, &height);
	image[3] = ppmRead("window.ppm", &width, &height);
	image[4] = ppmRead("table.ppm", &width, &height);
	image[5] = ppmRead("tv.ppm", &width, &height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(6, texName);

	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image[i]);
	}
}
void reshape (int width, int height)
{
	::width = width;
	::height = height;

	glViewport (0, 0, width, height);

	glMatrixMode (GL_PROJECTION);		// set projection parameters
	glLoadIdentity ();
	gluPerspective(60, 1, 0.1, 20);

	glMatrixMode (GL_MODELVIEW);		// set projection parameters
}
void display ()
{
	
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
	gluLookAt(vecEye.x, vecEye.y, vecEye.z, vecEye.x + 
				vecViewDirection.x, vecEye.y + vecViewDirection.y, vecEye.z + vecViewDirection.z, 
				vecUp.x, vecUp.y, vecUp.z);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear display window
	
	if (width == 0.0 || height == 0.0)
		return;
	
	glTranslatef(0, -2, -5);
	glRotatef(angleY, 0, 1, 0);
	  glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    
    glEnable(GL_NORMALIZE); //Automatically normalize normals
	draw_axes();   // the axes are the local frame for the floor 

	glPushMatrix();
	glTranslatef(2, 3.5, 2);

	draw_lamp();
	glPopMatrix();
	
	//////////////////draw lamp
	
	if(t)
	{
   glBindTexture(GL_TEXTURE_2D, texName[0]);
	
	glPushMatrix();
	glTranslatef(0, 0.01, 0);
	glScalef(7.9, 0.01, 3.9);
	draw_floor_tex();
	glPopMatrix();
	}

   draw_floor();
	
	                // the floow is a 4x0.01x4 cube 
	

	// -------   draw wall
	

	glPushMatrix();
	glTranslatef(-4, 2, 1.1);
	glRotatef(90, 0, 0, 1);
	draw_wall();
	glPopMatrix();

		
	
	glPushMatrix();
	glTranslatef(7.2, 2, 1);
	glRotatef(90, 0, 0, 1);
	draw_wall();
	glPopMatrix();
	
	
		if(t)
		{
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	
	glPushMatrix();
	glTranslatef(-2, 1.4, -1.8);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(0.35, 0.01, 1.5);
	draw_wall_tex();
	glPopMatrix();
		}

	glPushMatrix();
	glTranslatef(-0.9, 2, -2);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_wall();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(4.1, 2, -2);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_wall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-6.9, 2, 4.2);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_wall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.2, 2, 3.13);
	glRotatef(90, 0, 0, 1);
	draw_wall();
	glPopMatrix();
		
	////////////////////////////////
	glPushMatrix();
	glTranslatef(-4, 0.2, 5.3);
	glRotatef(90, 0, 1, 0);
	draw_stairs();
	glPopMatrix();
	/////////////draw stairs
	glPushMatrix();
	glTranslatef(6.8, 2, 1.3);
	glRotatef(90, 0, 1, 0);
	draw_TV();
	glPopMatrix();

	if(t)
	{
	 glBindTexture(GL_TEXTURE_2D, texName[5]);
	glPushMatrix();
	glTranslatef(6.5, 2.4, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	draw_tv_tex();
	glPopMatrix();
	}
	////////////draw TV
	glPushMatrix();
	glTranslatef(1.5, 0.1, 1.1);
	glRotatef(90, 1, 0, 0);
	draw_carpet();
	glPopMatrix();
	/////////draw carpet
	if(t)
	{
	 glBindTexture(GL_TEXTURE_2D, texName[3]);
	 glPushMatrix();
	glTranslatef(-3.7, 2.45, 2.2);
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	
	draw_window_tex();
	glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(-3.8, 2.8, 1.3);
	glRotatef(90, 0, 1, 0);
	draw_window();
	glPopMatrix(); 
    ///////draw window
	glPushMatrix();
	glTranslatef(-0.6, 0.3, -1);
	draw_couch();
	glPopMatrix();
	///////////draw couch
	if(t)
	{
	 glBindTexture(GL_TEXTURE_2D, texName[2]);
	 glPushMatrix();
	glTranslatef(-5.1, 2.35, 4.35);
	glScalef(0.16, 0.11, 0.1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_pic();
	glPopMatrix();
	}
	
	glPushMatrix();
	glTranslatef(-5.3, 2.6, 4.3);
	glRotatef(90, 0, 0, 1);
	draw_frame();
	glPopMatrix();
	//////draw frame
	glPushMatrix();
	glTranslatef(1.5, 0.7, 1.6);
	draw_table();
	glPopMatrix();

	if(t)
	{
	 glBindTexture(GL_TEXTURE_2D, texName[4]);
	glPushMatrix();
	glTranslatef(0.91, 1, 1);
	draw_table_tex();
	glPopMatrix();
	}
	

	/////draw table 
	glPushMatrix();
	glTranslatef(3.35, 0.55, 2.45);
	glRotatef(0, 0, 1, 0);
	glRotatef(0, 1, 0, 0);
	draw_table_legs();
	glPopMatrix();
	///////draw table legs
		
	glPushMatrix();
	glTranslatef(.5, 1.1, -1.65);
	glRotatef(0, 0, 1, 0);
	draw_cushion();
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(-1, 1.1, -1.65);
	glRotatef(0, 0, 1, 0);
	draw_cushion();
	glPopMatrix(); 
	
	glPushMatrix();
	glTranslatef(-2.5, 1.1, -1.65);
	glRotatef(0, 0, 1, 0);
	draw_cushion();
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(-3.5, 1.1, -0.5);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_cushion();
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(-3.5, 1.1, 0.9);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_cushion();
	glPopMatrix(); 

	//////draw cushions
	glPushMatrix();
	glTranslatef(0, 4, 0);
	draw_roof();
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(-7.8, 4, -0.5);
	glScalef(0.55, 1, 0.8);
	draw_roof();
	glPopMatrix(); 
	//////////draw 2nd floor
	glutSwapBuffers();
	glFlush ();			// process all openGl routines as quickly as possible
}
void moveForward()
{
	vecEye.x += MOVEMENT_SPEED * vecViewDirection.x;
	vecEye.y += MOVEMENT_SPEED * vecViewDirection.y;
	vecEye.z += MOVEMENT_SPEED * vecViewDirection.z;
}
void moveBackward()
{
	vecEye.x += -MOVEMENT_SPEED * vecViewDirection.x;
	vecEye.y += -MOVEMENT_SPEED * vecViewDirection.y;
	vecEye.z += -MOVEMENT_SPEED * vecViewDirection.z;
}
void moveLeft()
{
	Vector vecCross = Vector::Cross(vecViewDirection, vecUp);
	vecEye.x += -MOVEMENT_SPEED * vecCross.x;
	vecEye.y += -MOVEMENT_SPEED * vecCross.y;
	vecEye.z += -MOVEMENT_SPEED * vecCross.z;
}
void moveRight()
{
	Vector vecCross = Vector::Cross(vecViewDirection, vecUp);
	vecEye.x += MOVEMENT_SPEED * vecCross.x;
	vecEye.y += MOVEMENT_SPEED * vecCross.y;
	vecEye.z += MOVEMENT_SPEED * vecCross.z;
}
void moveUp()
{
	vecEye.x += MOVEMENT_SPEED * vecUp.x;
	vecEye.y += MOVEMENT_SPEED * vecUp.y;
	vecEye.z += MOVEMENT_SPEED * vecUp.z;
}
void moveDown()
{
	vecEye.x += -MOVEMENT_SPEED * vecUp.x;
	vecEye.y += -MOVEMENT_SPEED * vecUp.y;
	vecEye.z += -MOVEMENT_SPEED * vecUp.z;
}
void MoveMeFlat(int i)
{
	eyeX = eyeX + i * (centerX)* 0.1;
	eyeZ = eyeZ + i * (centerY)* 0.1;

	glLoadIdentity();

	gluLookAt(eyeX, eyeY, eyeZ,
		centerX, centerY, centerZ,
		0.0f, 1.0f, 0.0f);
}
void processKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':case 'w':
			moveForward(); break;
		case 'Z':case 'z':
			moveBackward(); break;
		case 'A':case 'a':
			moveLeft(); break;
		case 'D':case 'd':
			moveRight(); break;
		case 'R':case 'r':
			moveUp(); break;
		case 'F':case 'f':
			moveDown(); break;
		case 'Y':
			angleY += 1; break;
		case 'y':
			angleY -= 1; break;
		case 'T':case 't':
			t=!t;
									
			case 'L':case 'l':
			l=!l;
	}
}
void OrientMe(float ang)
{
	centerX = sin(ang);
	centerZ = -cos(ang);

	glLoadIdentity();

	gluLookAt(eyeX, eyeY, eyeZ,
		centerX, centerY, centerZ,
		0.0f, 1.0f, 0.0f);
}
void processSpecialKeys(int key, int x, int y) {

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		angle -= 0.01f;
		OrientMe(angle); break;
	case GLUT_KEY_LEFT:
		angle += 0.01f;
		OrientMe(angle); break;
	case GLUT_KEY_UP:
		MoveMeFlat(1); break;
	case GLUT_KEY_DOWN:
		MoveMeFlat(-1); break;
	}
}
void main (int argc, char** argv)
{
	glutInit (&argc, argv);										// initialize GLUT
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	// set display mode
	glutInitWindowPosition (20, 20);							// set top left display window position
	glutInitWindowSize (600, 600);								// set display window width and height
	glutCreateWindow ("OpenGL");					// create display window

	init ();													// execute initialization function
	glutKeyboardFunc(processKeyboard);
	glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc (reshape);
	glutDisplayFunc (display);									// send graphics to display window
	glutIdleFunc (display);
	glutMainLoop ();											// dispaly everthing and wait
}