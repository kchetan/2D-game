#include<stdio.h>
#include <iostream>
#include <cmath>
#include <string.h>
#include <sstream>
#include <fstream>
#include<stdlib.h>

#include <GL/glut.h>

using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

struct spiders {
	float x;
	float y;
	float speed;
	char colour;
	int flag;
}spider[100000];

struct lasers{
	float x;
	float y;
	float speed;
	float angle;
	int flag;
}laser[100000];

// Function Declarations
void drawScene();
void func1();
void update(int value);
void drawrectangle(float len);
void drawBox(float len);
//void drawBall(float rad);
void drawcannon();
void drawgameover();
void drawspider();
void createspider();
void createlaser();
void drawlaser();
void drawcontrol();
void drawbasket_green();
void drawbasket_red();
//void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleMousedrag(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void drawgamepause();
void drawscore();

// Global Variables
float box_len = 4.0f;
float cannon_x = 0.0f;
float cannon_y = -box_len/2.3f;
float cannon_angle = 0.0f;
float basket_green_x= box_len/2.5f;
float basket_green_y= -box_len/2.3f;
float basket_red_x= -box_len/2.5f;
float basket_red_y= -box_len/2.3f;
float cannon_rad=box_len/30;
float maxspeed=box_len/1500;
float minspeed=box_len/3000;
int score=0;
float tt=0;
char state='b';
char rem;
int count_spiders=0;
int count_lasers=0;
int active_spiders=0;
int max_spiders=4;
int t=0;
int t_l=1000;
int windowWidth;
int windowHeight;
int down=-1;
char state_m='n';

void input()
{
	char arr[100];
	FILE *file;
	file=fopen ("input.txt","r");
	if(file<=0)
	{
		printf("no input.txt file in current directory\n");
		exit(0);
	}
	fgets(arr,100,file);
	float val=0,flag=0,k=1,sign=1;
	int i;
	for(i=0;i<int(strlen(arr)-1);i++)
	{
		if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	box_len=val;
	fgets(arr,100,file);
	if(strcmp(arr,"MEDIUM\n")==0)
	{
		minspeed=box_len/1000;
		maxspeed=box_len/1500;
		max_spiders=5;
	}
	else if(strcmp(arr,"HARD\n")==0)
	{
		minspeed=box_len/500;
		maxspeed=box_len/1000;
		max_spiders=6;
	}
	fgets(arr,100,file);
	val=0;k=0;
	for(i=0;i<int(strlen(arr)-1) && arr[i]!=' ';i++)
	{
		if(arr[i]=='-')
			sign=-1;
		else if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	cannon_x=sign*val;
	sign=1;val=0;k=0;
	for(i++;i<int(strlen(arr)-1);i++)
	{
		if(arr[i]=='-')
			sign=-1;
		else if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	cannon_y=sign*val;
	val=0;k=0;
	sign=1;
	

	fgets(arr,100,file);
	val=0;k=0;
	for(i=0;i<int(strlen(arr)-1) && arr[i]!=' ';i++)
	{
		if(arr[i]=='-')
			sign=-1;
		else if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	basket_red_x=sign*val;
	sign=1;val=0;k=0;
	for(i++;i<int(strlen(arr)-1);i++)
	{
		if(arr[i]=='-')
			sign=-1;
		else if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	basket_red_y=sign*val;
	val=0;k=0;
	sign=1;
	
	fgets(arr,100,file);
	val=0;k=0;
	for(i=0;i<int(strlen(arr)-1) && arr[i]!=' ';i++)
	{
		if(arr[i]=='-')
			sign=-1;
		else if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	basket_green_x=sign*val;
	sign=1;val=0;k=0;
	for(i++;i<int(strlen(arr)-1);i++)
	{
		if(arr[i]=='-')
			sign=-1;
		else if(arr[i]=='.')
			flag=1;
		else if(flag==0)
			val=val*10+arr[i]-'0';
		else
			val=val+(arr[i]-'0')*1.0/pow(10,(k++));
	}
	basket_green_y=sign*val;
	fclose(file);

}

int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	windowWidth = w * 2 / 4;
	windowHeight = h * 2 / 3;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
	glutCreateWindow("Game");  // Setup the window
	initRendering();
	
	input();

	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutMotionFunc(handleMousedrag);
	glutReshapeFunc(handleResize);
	glutTimerFunc(10, update, 0);
	glutMainLoop();
	return 0;
}

// Function to draw objects on the screen
void drawScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	// Draw Box
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawBox(box_len);

	//draw rectangle below
	glPushMatrix();
	drawrectangle(box_len);
	glPopMatrix();

	//draw baskets
	glPushMatrix();
	glTranslatef(basket_green_x,basket_green_y,0.0f);
	drawbasket_green();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(basket_red_x,basket_red_y,0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawbasket_red();
	glPopMatrix();

	//draw cannon
	glPushMatrix();
	glTranslatef(cannon_x,cannon_y,0.0f);
	drawcannon();
	glPopMatrix();

	//draw spider
	glPushMatrix();
	if(state!='p' && state!='G')
	{
		//printf("%d lol active_spider=%d\n",t,active_spiders);
		if (active_spiders<=max_spiders && t >=800)
		{
			t=0;
			createspider();
			active_spiders++;
		}
	}
	drawspider();
	glPopMatrix();

	//draw laser
	glPushMatrix();
	drawlaser();
	glPopMatrix();

	if(state=='p')
	{
		//game paused
		glPushMatrix();
		gluLookAt(0.0, 30.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glRotatef(-20, 1.0, 0.0, 0.0);
		glScalef(0.03, 0.03, 0.03);
		drawgamepause();
		glPopMatrix();
	}

	if(state=='G')
	{
		//game paused
		glPushMatrix();
		gluLookAt(0.0, 30.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glRotatef(-20, 1.0, 0.0, 0.0);
		glScalef(0.03, 0.03, 0.03);
		drawgameover();
		glPopMatrix();
	}
	glPushMatrix();
	gluLookAt(0.0, 30.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef(0.023, 0.023, 0.023);
	glRotatef(-20, 1.0, 0.0, 0.0);
	drawscore();
	glPopMatrix();

	glPushMatrix();
	gluLookAt(0.0, 30.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef(0.023, 0.023, 0.023);
	glRotatef(-20, 1.0, 0.0, 0.0);
	drawcontrol();
	glPopMatrix();
	
	glPopMatrix();
	glutSwapBuffers();
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value)
{   
	//printf("score= %d \n",score);
	int i;

	if(state!='p' && state!='G')
	{
		tt=box_len/100000;
		if(tt >=10000)
		{
			tt=0;
			minspeed+=tt;
			maxspeed+=tt;
		}
		t +=10;
		t_l+=10;
		if(cannon_rad<box_len/30)
			cannon_rad+=box_len/1500;
		for(i=0;i<count_spiders;i++)
		{
			for(int j=0;j<count_lasers;j++)
			{
				if(spider[i].flag==1 && laser[j].flag==1 && sqrt(pow(spider[i].x - (laser[j].x + (box_len/20)*sin(DEG2RAD(-laser[i].angle)) ),2)+pow(spider[i].y - (laser[j].y + (box_len/20)*cos(DEG2RAD(laser[i].angle))),2))<=1.2*box_len/25)
				{
					system("cvlc smb_stomp.wav &");
					active_spiders--;
					spider[i].flag=0;
					laser[j].flag=0;
					if(spider[i].colour=='r')
						score=score-1;
					if(spider[i].colour=='b')
						score=score+1;
					if(spider[i].colour=='g')
						score=score-1;
				}
			}
			if(spider[i].y<=-box_len/2.5 + box_len/20  && spider[i].flag==1)
			{
				if(spider[i].x  > basket_green_x - box_len/20 && spider[i].x < basket_green_x + box_len/20 && spider[i].colour=='g')
				{
					active_spiders--;
					spider[i].flag=0;
					score=score+1;
					system("cvlc smb_coin.wav &");
				}
				else if(spider[i].x  > basket_green_x - box_len/20 && spider[i].x < basket_green_x + box_len/20  && (spider[i].colour=='r' || spider[i].colour=='b' ))
				{ 
					active_spiders--;
					spider[i].flag=0;
					score=score-1;
				}
				else if(spider[i].x  > basket_red_x - box_len/20 && spider[i].x < basket_red_x + box_len/20 && spider[i].colour=='r')
				{
					active_spiders--;
					spider[i].flag=0;
					score=score+1;
					system("cvlc smb_coin.wav &");
				}
				else if(spider[i].x  > basket_red_x - box_len/20 && spider[i].x < basket_red_x + box_len/20  && ( spider[i].colour=='g' || spider[i].colour=='b'))
				{
					active_spiders--;
					spider[i].flag=0;
					score=score-1;
				}
				else if(spider[i].x>cannon_x-box_len/16 && spider[i].x<cannon_x+box_len/16)
				{
					active_spiders--;
					state='G';
					system("cvlc smb_gameover.wav &");
				}
				else if(spider[i].y<= -box_len/2.4)
				{
					active_spiders--;
					score=score-5;
					spider[i].flag=-1;
				}
			}
			if(spider[i].flag==1)
				spider[i].y -=spider[i].speed;
		}
		for(i=0;i<count_lasers;i++)
		{
			if(laser[i].y>=box_len/2+box_len/10)
			{
				laser[i].flag=0;
			}
			if(laser[i].x-laser[i].speed*sin(DEG2RAD(laser[i].angle))>=box_len/2 || laser[i].x-laser[i].speed*sin(DEG2RAD(laser[i].angle))<=-box_len/2)
			{
				laser[i].angle *=-1;
			}
			if(laser[i].y>=box_len/2 || laser[i].y<=-box_len/2)
				laser[i].flag=0;
			laser[i].y +=laser[i].speed*cos(DEG2RAD(laser[i].angle));
			laser[i].x -=laser[i].speed*sin(DEG2RAD(laser[i].angle));
		}
	}
	glutTimerFunc(10, update, 0);
}

void drawgameover()
{
	glPushMatrix();
	//	float val=box_len/40;
	glLineWidth(2);
	glColor3f(0,0,0);
	char a[2][30];
	strcpy(a[0],"Game Over \nPress r to restart ");
	glPushMatrix();
	glTranslatef(-1*((int)strlen(a[0]))*37,0, 0.0f);
	for(int i=0;i<(int)strlen(a[0]);i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN,a[0][i]);
	}
	glPopMatrix();
	glLineWidth(1);
	glPopMatrix();

}
void drawscore()
{
	char a[2][10];
	int j;
	glLineWidth(2);
	strcpy(a[0],"Score: ");
	sprintf(a[1], "%d", score);
	strcat(a[0],a[1]);
	glPushMatrix();
	glColor3f(0.0f,0,0);
	glTranslatef(box_len*250,box_len*430, 0.0f);
	for(j=0;j<(int)strlen(a[0]);j++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN,a[0][j]);
	}
	glPopMatrix();
	glLineWidth(1);

}
void drawcontrol()
{
	char a[2][50];
	int j;
	glLineWidth(2);
	strcpy(a[0],"Controller: ");
	if(state=='r')
		strcat(a[0],"red-basket");
	else if(state=='g')
		strcat(a[0],"green-basket");
	else if(state=='b')
		strcat(a[0],"cannon");
	else
		a[0][0]='\0';
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(-box_len*450,box_len*430, 0.0f);
	for(j=0;j<(int)strlen(a[0]);j++)
	{
		if(a[0][j]==' ' && a[0][j+1]=='r')
			glColor3f(1,0,0);
		if(a[0][j]==' ' && a[0][j+1]=='c')
			glColor3f(0,0,1);
		if(a[0][j]==' ' && a[0][j+1]=='g')
			glColor3f(0,0.3,0);
		glutStrokeCharacter(GLUT_STROKE_ROMAN,a[0][j]);
	}
	glPopMatrix();
	glLineWidth(1);

}
void drawgamepause()
{
	glPushMatrix();
	//	float val=box_len/40;
	glLineWidth(2);
	glColor3f(0,0,0);
	char a[2][30];
	strcpy(a[0],"Game Paused \nPress P to continue! ");
	glPushMatrix();
	glTranslatef(-1*((int)strlen(a[0]))*37,0, 0.0f);
	for(int i=0;i<(int)strlen(a[0]);i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN,a[0][i]);
	}
	glPopMatrix();
	glLineWidth(1);
	glPopMatrix();

}

void createlaser()
{
	system("cvlc laser_human.mp3 &");
	laser[count_lasers].x=cannon_x;
	laser[count_lasers].y=cannon_y;
	laser[count_lasers].speed=box_len/50;
	laser[count_lasers].flag=1;
	laser[count_lasers++].angle=cannon_angle;

}

void drawlaser()
{
	int i,angle;
	for (i=0;i<count_lasers;i++)
	{
		angle=-1*laser[i].angle;
		if(laser[i].flag==1)
		{
			if(laser[i].x + (box_len/10 ) * sin(DEG2RAD(angle))<=box_len/2 && laser[i].x + (box_len/10)*sin(DEG2RAD(angle)) >= -box_len/2)
			{
				glPushMatrix();
				glTranslatef(laser[i].x,laser[i].y,0.0f);
				glRotatef(laser[i].angle,0,0,1);
				glColor3f(1, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f(box_len/400,box_len/10);
				glVertex2f(-box_len/400,box_len/10);	
				glVertex2f(-box_len/400,0);
				glVertex2f(box_len/400,0);
				glEnd();
				glPopMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glLineWidth(0.5);
				glPushMatrix();
				glTranslatef(laser[i].x,laser[i].y,0.0f);
				glRotatef(laser[i].angle,0,0,1);
				glColor3f(0, 0, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f(box_len/400,2*box_len/20);
				glVertex2f(-box_len/400,2*box_len/20);	
				glVertex2f(-box_len/400,0);
				glVertex2f(box_len/400,0);
				glEnd();
				glPopMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glLineWidth(1);
			}
			else
			{
				float x1,y1,x2,y2,l=box_len/10;
				glPushMatrix();
				glTranslatef(laser[i].x,laser[i].y,0.0f);
				if (laser[i].x + (box_len/10 ) * sin(DEG2RAD(angle)) > box_len/2)
					x1=box_len/2 - laser[i].x;
				else
					x1=-box_len/2 - laser[i].x;
				y1= x1 / tan(DEG2RAD(angle));
				glColor3f(1.0, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f((box_len/400)*sin(DEG2RAD(angle)),-1*(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(-1*(box_len/400)*sin(DEG2RAD(angle)),(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x1 - 1*(box_len/400)*sin(DEG2RAD(angle)),y1+(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x1+(box_len/400)*sin(DEG2RAD(angle)),y1 - 1*(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glEnd();
				glPopMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPushMatrix();
				glTranslatef(laser[i].x,laser[i].y,0.0f);
				glColor3f(0, 0, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f((box_len/400)*sin(DEG2RAD(angle)),-1*(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(-1*(box_len/400)*sin(DEG2RAD(angle)),(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x1 - 1*(box_len/400)*sin(DEG2RAD(angle)),y1+(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x1+(box_len/400)*sin(DEG2RAD(angle)),y1 - 1*(box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glEnd();
				glPopMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glLineWidth(0.5);
				y2=box_len/10* cos(DEG2RAD(angle));
				x2= x1 - ( l*sin(DEG2RAD(angle)) - x1 );
				glPushMatrix();
				glTranslatef(laser[i].x,laser[i].y,0.0f);
				glColor3f(1.0, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f(x1 - (box_len/400)*sin(DEG2RAD(angle)),y1 - (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x1 + (box_len/400)*sin(DEG2RAD(angle)),y1 + (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x2 + (box_len/400)*sin(DEG2RAD(angle)),y2 + (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x2 - (box_len/400)*sin(DEG2RAD(angle)),y2 - (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glEnd();
				glPopMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPushMatrix();
				glTranslatef(laser[i].x,laser[i].y,0.0f);
				glColor3f(0, 0, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f(x1 - (box_len/400)*sin(DEG2RAD(angle)),y1 - (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x1 + (box_len/400)*sin(DEG2RAD(angle)),y1 + (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x2 + (box_len/400)*sin(DEG2RAD(angle)),y2 + (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glVertex2f(x2 - (box_len/400)*sin(DEG2RAD(angle)),y2 - (box_len/400)*cos(DEG2RAD(laser[i].angle)));
				glEnd();
				glPopMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glLineWidth(1);
			}
		}
	}
}

void drawspider()
{
	int i;
	for (i=0;i<count_spiders;i++)
	{
		if(spider[i].flag!=0)
		{
			glPushMatrix();
			//glScalef(0.8f,0.8f,0.8f);
			glTranslatef(spider[i].x,spider[i].y,0.0f);
			if(spider[i].colour=='r')
				glColor3f(1.0f, 0.0f, 0.0f);
			if(spider[i].colour=='b')
				glColor3f(0.0f, 0.0f, 0.0f);
			if(spider[i].colour=='g')
				glColor3f(0.0f, 1.0f, 0.0f);
			float rad=box_len/30;
			glBegin(GL_TRIANGLE_FAN);
			for(int j=0 ; j<360 ; j++)
				glVertex2f(rad * cos(DEG2RAD(j)), rad * sin(DEG2RAD(j)));
			glEnd();
			glTranslatef(0,-3*rad/2+rad/30,0.0f);
			glBegin(GL_TRIANGLE_FAN);
			for(int j=0 ; j<360 ; j++)
				glVertex2f(rad * cos(DEG2RAD(j))/2, rad * sin(DEG2RAD(j))/2);
			glEnd();

			glTranslatef(0,3*rad/2-rad/30,0.0f);

			glBegin(GL_LINES);

			glVertex2f(-1.25*rad,1.25*rad);
			glVertex2f(0,0);

			glVertex2f(-1.25*rad,1.25*rad);
			glVertex2f(-2*rad,0);

			glVertex2f(0,0);
			glVertex2f(1.25*rad,1.25*rad);

			glVertex2f(1.25*rad,1.25*rad);
			glVertex2f(2*rad,0);

			glVertex2f(-1.25*rad,-0.75*rad);
			glVertex2f(0,-1*rad);

			glVertex2f(-1.25*rad,-0.75*rad);
			glVertex2f(-1.65*rad,-1.25*rad);

			glVertex2f(1.25*rad,-0.75*rad);
			glVertex2f(0,-1*rad);

			glVertex2f(1.25*rad,-0.75*rad);
			glVertex2f(1.65*rad,-1.25*rad);

			glVertex2f(0,-1.5*rad);
			glVertex2f(-0.85*rad,-1.75*rad);

			glVertex2f(-0.6*rad,-2.2*rad);
			glVertex2f(-0.85*rad,-1.75*rad);


			glVertex2f(0,-1.5*rad);
			glVertex2f(0.85*rad,-1.75*rad);

			glVertex2f(0.85*rad,-1.75*rad);	
			glVertex2f(0.6*rad,-2.2*rad);

			glEnd();

			glPopMatrix();
		}
	}

}

void createspider()
{
	float r=(rand()%((int)(100*(box_len-box_len/10))));
	r=r/100 - (box_len-box_len/10)/2;
	spider[count_spiders].x=r;
	spider[count_spiders].y=box_len/2;
	spider[count_spiders].flag=1;
	spider[count_spiders++].speed=minspeed + (maxspeed-minspeed)*((rand()%((int)(1000)))*1.0/1000);
	int j=rand()%3;
	if(j==0)
		spider[count_spiders-1].colour='r';
	if(j==1)
		spider[count_spiders-1].colour='b';
	if(j==2)
		spider[count_spiders-1].colour='g';

}

void drawbasket_green()
{
	float len=box_len/10,rad=box_len/20;
	glColor3f(0, 1, 0);
	if(state=='g')
		glColor3f(0,0.5,0);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glColor3f(0.3,1, 0.3);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();



	glPushMatrix();
	glTranslatef(0,len/2,0.0f);
	glColor3f(0.7f,1,0.7f);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex3f(rad * cos(DEG2RAD(i)),box_len/60* sin(DEG2RAD(i)),0);
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,-len/2,0.0f);
	glColor3f(0, 0.8,0);
	if(state=='g')
		glColor3f(0,0.7,0);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex3f(rad * cos(DEG2RAD(i)),box_len/60* sin(DEG2RAD(i)),0);
	}
	glEnd();
	glPopMatrix();
}

void drawbasket_red()
{
	float len=box_len/10,rad=box_len/20;
	glColor3f(1, 0, 0);
	if(state=='r')
		glColor3f(0.8,0,0);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glColor3f(1, 0.3, 0.3);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();

	glPushMatrix();
	glTranslatef(0,len/2,0.0f);
	glColor3f(1, 0.7f,0.7f);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex3f(rad * cos(DEG2RAD(i)),box_len/60* sin(DEG2RAD(i)),0);
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,-len/2,0.0f);
	glColor3f(0.9, 0,0);
	if(state=='r')
		glColor3f(0.6,0,0);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex3f(rad * cos(DEG2RAD(i)),box_len/60* sin(DEG2RAD(i)),0);
	}
	glEnd();
	glPopMatrix();

}
void drawcannon()
{
	float len=box_len/10;
	glScalef(0.7f, 0.7f, 0.7f);
		glColor3f(0.0f, 0.3f, 0);
		if(state=='b')
		glColor3f(0.0f, 0.2f, 0);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2.2, -len / 2.2);
	glVertex2f(len / 2.2, -len / 2.2);
	glColor3f(0.3f, 1, 0.3);
	glVertex2f(len / 2.2, len / 2.2);
	glVertex2f(-len / 2.2, len / 2.2);

	glVertex2f(-1.2*len/2.2  , -0.15*len);
	glVertex2f(-1*len/2.2 , -0.15*len );
	glVertex2f(-1*len/2.2 , 0.15*len );
	glVertex2f(-1.2*len/2.2 , 0.15*len );

	glVertex2f(1.2*len/2.2  , -0.15*len);
	glVertex2f(1*len/2.2 , -0.15*len );
	glVertex2f(1*len/2.2 , 0.15*len );
	glVertex2f(1.2*len/2.2 , 0.15*len );
	
	glColor3f(0,0,0);
	glVertex2f(-1.2*len/2.2  , -1.30*len/2.2);
	glVertex2f(-1.6*len/2.2 , -1.30*len/2.2 );
	glColor3f(0.6,0.6,0.6);
	glVertex2f(-1.6*len/2.2 , 1.30*len/2.2 );
	glVertex2f(-1.2*len/2.2 , 1.30*len/2.2 );
	
	glColor3f(0,0,0);
	glVertex2f(1.2*len/2.2  , -1.30*len/2.2);
	glVertex2f(1.6*len/2.2 , -1.30*len/2.2 );
	glColor3f(0.6,0.6,0.6);
	glVertex2f(1.6*len/2.2 , 1.30*len/2.2 );
	glVertex2f(1.2*len/2.2 , 1.30*len/2.2 );
	
	glEnd();

	glRotatef(cannon_angle, 0.0f, 0.0f, 1.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_QUADS); 
	glVertex2f(0.2*len/2.2 , len/2.2);
	glVertex2f(-0.2*len/2.2 , len/2.2);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex2f(-0.2*len/2.2  , 2.3*len/2.2);
	glVertex2f(0.2*len/2.2 , 2.3*len/2.2 );
	glEnd();
	glRotatef(-cannon_angle, 0.0f, 0.0f, 1.0f);

	//glColor3f(1, 0.1f, 0.1f);
	glColor3f(0.5, 0.5, 0.5f);
	if(state=='b')
		glColor3f(0.1, 0.1, 0.1f);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex3f(cannon_rad * cos(DEG2RAD(i)),cannon_rad * sin(DEG2RAD(i)),0);
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);

		glColor3f(0,0,0);
		glColor3f(0,0,0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2.2, -len / 2.2);
	glVertex2f(len / 2.2, -len / 2.2);
	glVertex2f(len / 2.2, len / 2.2);
	glVertex2f(-len / 2.2, len / 2.2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-1.2*len/2.2  , -0.10*len);
	glVertex2f(-1*len/2.2 , -0.10*len );
	glVertex2f(-1*len/2.2 , 0.10*len );
	glVertex2f(-1.2*len/2.2 , 0.10*len );
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-1.2*len/2.2  , -1.30*len/2.2);
	glVertex2f(-1.6*len/2.2 , -1.30*len/2.2 );
	glVertex2f(-1.6*len/2.2 , 1.30*len/2.2 );
	glVertex2f(-1.2*len/2.2 , 1.30*len/2.2 );
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(1.2*len/2.2  , -1.30*len/2.2);
	glVertex2f(1.6*len/2.2 , -1.30*len/2.2 );
	glVertex2f(1.6*len/2.2 , 1.30*len/2.2 );
	glVertex2f(1.2*len/2.2 , 1.30*len/2.2 );
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRotatef(cannon_angle, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS); 
	glVertex2f(-0.2*len/2.2  , 2.3*len/2.2);
	glVertex2f(-0.2*len/2.2 , len/2.2 );
	glVertex2f(0.2*len/2.2 , len/2.2 );
	glVertex2f(0.2*len/2.2 , 2.3*len/2.2 );
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRotatef(cannon_angle, 0.0f, 0.0f, 1.0f);
	glLineWidth(1);

}

void drawrectangle(float len)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(181*1.0/255,185*1.0/255,185*1.0/255);
	glBegin(GL_QUADS);
	glVertex2f(-box_len/2,-box_len/2);
	glVertex2f(box_len/2,-box_len/2);
	glColor3f(1,1,1);
	glVertex2f(box_len/2,-box_len/2.5f);
	glVertex2f(-box_len/2,-box_len/2.5f);
	glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void drawBox(float len) 
{   
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glColor3f(198*1.0/255,215*1.0/255,213*1.0/255);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) 
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	windowWidth=w;
	windowHeight=h;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) 
{
	if (key == 27) 
		exit(0);   // escape key is pressed
	if (key == 'r' && state!='p')
	{
		if (state=='G')
		{
			for(int i=0;i<10000;i++)
			{
				laser[i].flag=0;
				spider[i].flag=0;
			}
			cannon_x =0;
			cannon_y = -box_len/2.3f;
			cannon_angle = 0.0f;
			basket_green_x= box_len/2.5f;
			basket_green_y= -box_len/2.3f;
			basket_red_x= -box_len/2.5f;
			basket_red_y= -box_len/2.3f;
			score=0;
			state='b';
			count_spiders=0;
			count_lasers=0;
			active_spiders=0;
			max_spiders=5;
			t=0;
			t_l=1000;

		}
		else
			state='r';
	}
	if (key == 'b'  && state!='p'&& state!='G')
		state='b';
	if(key=='p' && state!='G')
	{
		if(state!='p')
		{
			system("cvlc smb_pause.wav &");
			rem=state;
			state='p';
		}
		else
			state=rem;
	}
	if (key == 'g' && state!='p' && state!='G') 
		state='g';
	if (key== ' ' && state=='b' && state!='G')	
	{
		if(t_l>=500)
		{
			cannon_rad=0;
			t_l=0;
			createlaser();
		}
	} 
}

void handleKeypress2(int key, int x, int y) 
{
	int flag=0,i;
	float val=box_len/100;
	if (key == GLUT_KEY_LEFT)
	{
		if(state=='b'&& cannon_x>-1*box_len/2+box_len/20)
		{
			for(i=0;i<count_spiders;i++)
			{
				if(cannon_x - box_len/15 >= spider[i].x && (cannon_x - val - box_len/15) <=spider[i].x && spider[i].flag==-1 )
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				cannon_x -= box_len/100;
		}
		else if(state=='g'&& basket_green_x > -1*box_len/2+box_len/20)
		{
			for(i=0;i<count_spiders;i++)
			{
				if(basket_green_x - box_len/15 >= spider[i].x && (basket_green_x - val - box_len/15) <=spider[i].x && spider[i].flag==-1 )
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				basket_green_x -= val;
		}
		else if(state=='r'&& basket_red_x>-1*box_len/2+box_len/20)
		{
			for(i=0;i<count_spiders;i++)
			{
				if(basket_red_x - box_len/15 >= spider[i].x && basket_red_x - val -box_len/15 <=spider[i].x && spider[i].flag==-1 )
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				basket_red_x -= val;
		}
	}
	if (key == GLUT_KEY_RIGHT )
	{
		if(state=='b'  && cannon_x<box_len/2-box_len/20)
		{
			for(i=0;i<count_spiders;i++)
			{
				if(cannon_x + box_len/15 <= spider[i].x && (cannon_x + val + box_len/15) >=spider[i].x && spider[i].flag==-1 )
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				cannon_x += val;
		}
		if(state=='g'&& basket_green_x<box_len/2-box_len/20)
		{
			for(i=0;i<count_spiders;i++)
			{
				if(basket_green_x + box_len/15 <= spider[i].x && (basket_green_x + val + box_len/15) >=spider[i].x && spider[i].flag==-1 )
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				basket_green_x += val;
		}
		if(state=='r'&& basket_red_x<box_len/2-box_len/20)
		{
			for(i=0;i<count_spiders;i++)
			{
				if(basket_red_x + box_len/15 <= spider[i].x && basket_red_x + val + box_len/15 >=spider[i].x && spider[i].flag==-1 )
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
				basket_red_x += val;
		}
	}
	if (key == GLUT_KEY_UP && cannon_angle<80)
	{
		if(state=='b')
			cannon_angle += 5;
	}
	if (key == GLUT_KEY_DOWN && cannon_angle>-80)
	{
		if(state=='b')
			cannon_angle -= 5;
	}
}

void handleMousedrag(int x,int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	posX*=50;
	posY*=50;
	if(down==1 && posX>-1*box_len/2+box_len/20 && posX<box_len/2-box_len/20)
	{
		int fl=0;
		if(state_m=='r')
		{
			for(int temp=0;temp<count_spiders;temp++)
			{
				if((basket_red_x - box_len/15 )>= spider[temp].x && (posX - box_len/15) <= spider[temp].x && spider[temp].flag==-1 )
				{
					fl=1;
					break;
				}
			}
			for(int temp=0;temp<count_spiders;temp++)
			{
				if(basket_red_x + box_len/15 <= spider[temp].x && (posX + box_len/15) >=spider[temp].x && spider[temp].flag==     -1  )
				{
					fl=1;
					break;
				}
			}
				if(fl==0)
					basket_red_x=posX;
		}
		else if(state_m=='g')
		{
			for(int temp=0;temp<count_spiders;temp++)
			{
				if(basket_green_x - box_len/15 >= spider[temp].x && (posX - box_len/15) <= spider[temp].x && spider[temp].flag==-1 )
				{
					fl=1;
					break;
				}
			}
			for(int temp=0;temp<count_spiders;temp++)
			{
				if(basket_green_x + box_len/15 <= spider[temp].x && (posX + box_len/15) >=spider[temp].x && spider[temp].flag==     -1  )
				{
					fl=1;
					break;
				}
			}
				if(fl==0)
					basket_green_x=posX;
		}
		else if(state_m=='b')
		{
			for(int temp=0;temp<count_spiders;temp++)
			{
				if(cannon_x - box_len/15 >= spider[temp].x && (posX - box_len/15) <= spider[temp].x && spider[temp].flag==-1 )
				{
					fl=1;
					break;
				}
			}
			for(int temp=0;temp<count_spiders;temp++)
			{
				if(cannon_x + box_len/15 <= spider[temp].x && (posX + box_len/15) >=spider[temp].x && spider[temp].flag==     -1  )
				{
					fl=1;
					break;
				}
			}
				if(fl==0)
					cannon_x=posX;
		}
	}
	else if(down==0)
	{
		if(state_m=='b')
		{
			float angle=atan((posY-cannon_y)*1.0/(posX-cannon_x)) * 180 / PI;
			if(angle>0)
				cannon_angle=angle-90;
			else if(angle<0)
				cannon_angle=angle+90;
		}

	}
	//cout << posX << posY << endl;
}

void handleMouseclick(int button, int stat, int x, int y) 
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	posX*=50;
	posY*=50;

	if (stat == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON && state!='G' && state!='p')
		{
			if(posX>cannon_x-box_len/16 && posX<cannon_x+box_len/16 && posY>cannon_y-box_len/16 && posY<cannon_y+box_len/16)
			{
				state_m='b';
			}
			else if(posX>basket_red_x-box_len/20 && posX<basket_red_x+box_len/20 && posY>basket_red_y-box_len/10 && posY<basket_red_y+box_len/10)
			{
				state_m='r';
			}
			else if(posX>basket_green_x-box_len/20 && posX<basket_green_x+box_len/20 && posY>basket_green_y-box_len/10 && posY<basket_green_y+box_len/10)
			{
				state_m='g';
			}
			else
				state_m='n';
			down=1;
			//cannon_angle += 1;
		}
		else if (button == GLUT_RIGHT_BUTTON && state!='G' && state!='p')
		{
			if(posX>cannon_x-box_len/16 && posX<cannon_x+box_len/16 && posY>cannon_y-box_len/16 && posY<cannon_y+box_len/16)
			{
				state_m='b';
			}
			else
				state_m='n';
			down=0;
			//cannon_angle -= 10;
		}
	}
}
