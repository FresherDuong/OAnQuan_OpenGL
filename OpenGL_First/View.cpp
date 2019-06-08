#include "View.h"

//static char P1[40] = "PLAYER 1: 0";
//static char P2[40] = "PLAYER 2: 0";
//
////STROKE - BUGS
//void writeStrokeString(float x, float y, void* font, char* string) {
//	char* c;
//	glPushMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(x, y, 0);
//	glScalef(1.0, 1.0, 1.0);
//	for (c = string; *c != '\0'; c++) { glutStrokeCharacter(font, *c); }
//	glPopMatrix();
//}
//
//void DisplayStroke() {
//	writeStrokeString(100, 40, GLUT_STROKE_ROMAN, P1);
//}
//
//// BITMAP - OK
//void writeBitmapString(float x, float y, void* font, char* string) {
//
//	char* c;
//	glRasterPos2f(x, y);
//	for (c = string; *c != '\0'; c++) { glutBitmapCharacter(font, *c); }
//}
//
//void DisplayBitMapP1() {
//	writeBitmapString(100, 40, GLUT_BITMAP_TIMES_ROMAN_24, P1); // Import Coordinate here !
//}
//
//void DisplayBitMapP2() {
//	writeBitmapString(100, 440, GLUT_BITMAP_TIMES_ROMAN_24, P2); // Import Coordinate here !
//}