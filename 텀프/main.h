#ifndef __MAIN_H__
#define __MAIN_H__
#include <GL/freeglut.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Timerfunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int xp, int yp);
void Control_light();

GLubyte * LoadDIBitmap(const char *file_name, BITMAPINFO **info);
void Init_Texture(GLubyte *pBytes, BITMAPINFO *info, GLuint *textures);
void Draw_Background();
void Draw_Level();
void Sound_SetUp();
void Play_Sound();

struct mouse_
{
	double x = 0;
};

#endif // !__MAIN_H__