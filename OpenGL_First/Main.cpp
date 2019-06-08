#include <iostream>
#include <conio.h>
#include <glut.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <irrKlang.h>

#define PI 3.1415926535897932384626433832795
#define MAXX 800
#define MAXY 550
#define Y 250

using namespace std;
using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();

class OAnQuan;

// Control Key Events
static bool UP = false;
static bool DOWN = false;
static bool ENTER = false;
static bool LEFT = false;
static bool RIGHT = false;


// Control Game's State
static int HienTai = 1; //Giống như biến soO để xác định ô hiện tại là ô nào
static int Player = 0;
static int MousePlayer = 0;
static int tmp;
static int mx; //Biên xác định chiều dải 1 hoặc -1
static int DaiDan = 0;
static int Score[2] = { 0,0 };
static bool DONE = false;
static bool WIN_CONFIRMATION = false;
static int he = 0;
static int check = 0;

// Buffer dung de ghep text hien thi diem
char buffer1[100];
char buffer2[100];
char buffer3[100];

//Result Score Text
static char P1[100] = "PLAYER 1 : ";
static char P2[100] = "PLAYER 2 : ";
char GAME_NAME[50] = "O AN QUAN : MODE PvP";

// Duong dan toi file am thanh .wav
#pragma region Duong_Dan_File_Am_Thanh
char UP_DOWN_SOUND_PATH[300] = "C:/Users/Duong Dinh/Desktop/DoHoa/OpenGL_First/OpenGL_First/Audio/ChonOSound.wav";
char ENTER_SOUND_PATH[300] = "C:/Users/Duong Dinh/Desktop/DoHoa/OpenGL_First/OpenGL_First/Audio/EnterSound.wav";
char LEFT_RIGHT_SOUND_PATH[300] = "C:/Users/Duong Dinh/Desktop/DoHoa/OpenGL_First/OpenGL_First/Audio/DaiOSound2.wav";
char BACK_GROUND_SOUND_PATH[300] = "Still Missing";
char ESC_SOUND_PATH[300] = "C:/Users/Duong Dinh/Desktop/DoHoa/OpenGL_First/OpenGL_First/Audio/incorrect_sfx.wav";
char WINNING_SOUND_PATH[300] = "C:/Users/Duong Dinh/Desktop/DoHoa/OpenGL_First/OpenGL_First/Audio/victory_confetti.wav";
#pragma endregion

struct ToaDoDan {
	int x;
	int y;
};

//Định nghĩa vị trí cứng xuất hiện cho tọa độ của dân
ToaDoDan tdd[] = { {0,0},{0,0},{-25,-25},{25,-25},{-25,25},{25,25},{0,-25},{-25,0},{25,0},{0,25},{-12,-12},{12,-12},
				 {-12,12},{12,12},{-12,-25},{12,-25},{-12,25},{12,25},{-25,-12},{25,-12},{-25,12},{25,12} };

//Định nghĩa vị trí cứng xuất hiện cho tọa độ của quan
ToaDoDan tddq[] = { {0,0},{35,-75},{35,-60},{35,-45},{35,-30},{35,-15},{35,0},{35,15},{35,30},{35,45},{35,60},{35,75},
					{20,-75},{20,-60},{20,-45},{20,-30},{20,-15},{20,0},{20,15},{20,30},{20,45},{20,60},{20,75} };
ToaDoDan tddtq[] = { {0,0} };
int Abs(int x) {
	return x > 0 ? x : -x;
}

// Ve cac hinh co ban
void drawPoint(GLint x1, GLint y1) {
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex2i(x1, y1);
	glEnd();
}

void drawRectangle(GLint x1, GLint y1, GLint x2, GLint y2) {
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
}

void drawSolidRectangle(GLint x1, GLint y1, GLint x2, GLint y2) {
	glBegin(GL_POLYGON);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
}

void drawLine(GLint x1, GLint y1, GLint x2, GLint y2) {
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void drawArc(GLint x_center, GLint y_center, GLint start_angle, GLint end_angle, GLint R) {
	glBegin(GL_LINE_STRIP);
	for (int i = start_angle; i <= end_angle; i++)
		glVertex3f(R*cos(2 * PI*i / 360) + x_center, R*sin(2 * PI*i / 360) + y_center, 0.0);
	glEnd();
}

void drawPieslice(GLint x_center, GLint y_center, GLint start_angle, GLint end_angle, GLint R) {
	glBegin(GL_POLYGON);
	for (int i = start_angle; i <= end_angle; i++)
		glVertex3f(R*cos(2 * PI*i / 360) + x_center, R*sin(2 * PI*i / 360) + y_center, 0.0);
	glEnd();
}

void drawCircle(GLint x_center, GLint y_center, GLint R) {
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++)
		glVertex3f(R*cos(2 * PI*i / 360) + x_center, R*sin(2 * PI*i / 360) + y_center, 0.0);
	glEnd();
}

void drawBar(GLint x1, GLint y1, GLint x2, GLint y2) {
	glBegin(GL_POLYGON);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
}

#pragma region DisplayText

void writeBitmapString(float x, float y, void* font, char* string) {

	char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) { glutBitmapCharacter(font, *c); }
}

void DisplayBitMapName(char* stringP1) {
	glColor3f(1.0, 1.0, 1.0);
	writeBitmapString(270, 30, GLUT_BITMAP_TIMES_ROMAN_24, stringP1); // Import Coordinate here !
}
void DisplayBitMapP1(char* stringP1) {
	writeBitmapString(100, 85, GLUT_BITMAP_TIMES_ROMAN_24, stringP1); // Import Coordinate here !
}
void DisplayBitMapP2(char* stringP2) {
	writeBitmapString(100, 430, GLUT_BITMAP_TIMES_ROMAN_24, stringP2); // Import Coordinate here !
}

//Hien thi diem khi chien thang

void DisplayBitMapP1_WinScore(char* stringP1) {
	writeBitmapString(340, 150, GLUT_BITMAP_TIMES_ROMAN_24, stringP1); // Import Coordinate here !
}

void DisplayBitMapP2_WinScore(char* stringP1) {
	writeBitmapString(340, 250, GLUT_BITMAP_TIMES_ROMAN_24, stringP1); // Import Coordinate here !
}

void DisplayWinResultText(char* stringP1) {
	writeBitmapString(220, 350, GLUT_BITMAP_TIMES_ROMAN_24, stringP1); // Import Coordinate here !
}

#pragma endregion

// O AN QUAN
class OAnQuan {
public:
	int conQuan; //Còn quan hav không để quết định dải quân tiếp, hết quan thì thông báo người win
	int soDan;
	OAnQuan(int a = 1, int b = 5)
	{
		conQuan = a;
		soDan = b;
	}

	void veDan();
};

void OAnQuan::veDan() {
	glColor3f(1.0, 1.0, 1.0);
	int n;
	if (HienTai % 6 == 0) { //Phần vẽ dân ở ô Quan
		n = (HienTai + 1) * 100;
		drawCircle(n + (1 - HienTai / 3)*tddq[this->soDan].x, 250 + tddq[this->soDan].y, 5);
	}
	else { //Phần vẽ dân ở ô dân bình thường
		int n = (7 - Abs(6 - HienTai)) * 100;
		int m = (HienTai / 6 + 2) * 100;
		drawCircle(n + tdd[this->soDan].x, m + tdd[this->soDan].y, 5);
	}
	cout << "veDan " << HienTai << "\n";
}

static OAnQuan oaq[12];

// Ve doi tuong cho O AN QUAN

void VeDan(int soO, int soDan) { //sô 1-11, soDan 1-5 //ok
	glColor3f(1.0, 1.0, 1.0);
	int n = (7 - Abs(6 - soO)) * 100;
	int m = (soO / 6 + 2) * 100;
	drawCircle(n + tdd[soDan].x, m + tdd[soDan].y, 5);
}

void VeDan2(int soO, int soDan) {
	glColor3f(1.0, 1.0, 1.0);
	//int n = (7 - Abs(6 - soO)) * 100;
	int n = (soO + 1) * 100;
	int m = (soO / 6 + 2) * 100;
	//drawCircle(n + tdd[soDan].x, m + tdd[soDan].y, 5);
	drawCircle(n + (1 - soO / 3)*tddq[soDan].x, 250 + tddq[soDan].y, 5);

	//int n = (i + 1) * 100;
		//drawCircle(n + (1 - HienTai / 3)*tddq[this->soDan].x, 250 + tddq[this->soDan].y, 5);

	//n = (HienTai + 1) * 100;
		//drawCircle(n + (1 - HienTai / 3)*tddq[this->soDan].x, 250 + tddq[this->soDan].y, 5);
}

void VeQuan(int soO) { //ok
	glColor3f(0.91, 0.76, 0.65);
	int n = (soO + 1) * 100;
	drawSolidRectangle(n - 10, 230, n + 10, 270);
}

void drawFull() { // Ô 0,6 là các ô quan
	for (int i = 0; i < 12; i++) {
		if (i % 6) {
			for (int j = 1; j <= 5; j++) // Chỉnh số quân muốn chơi ở đâv 
				VeDan(i, j);
		}
		else VeQuan(i);
	}
}

void drawBan() {
	glColor3f(1.0, 1.0, 1.0);
	drawRectangle(150, 150, 650, 350);
	for (int i = 1; i <= 4; i++) drawLine(150 + 100 * i, 150, 150 + 100 * i, 350);
	drawLine(150, 250, 650, 250);
	drawArc(150, 250, 90, 270, 100);
	drawArc(650, 250, -90, 90, 100);

	drawFull();
}

void drawBanEmty() {
	glColor3f(1.0, 1.0, 1.0);
	drawRectangle(150, 150, 650, 350);
	for (int i = 1; i <= 4; i++) drawLine(150 + 100 * i, 150, 150 + 100 * i, 350);
	drawLine(150, 250, 650, 250);
	drawArc(150, 250, 90, 270, 100);
	drawArc(650, 250, -90, 90, 100);
}

void XoaO() {
	glColor3f(0.25, 0.25, 0.25);
	int n = (7 - Abs(6 - HienTai)) * 100;
	int m = (HienTai / 6 + 2) * 100;
	drawBar(n - 48, m - 48, n + 48, m + 48);
	cout << "XOA O " << HienTai << "\n";
}

void XoaOQuan() {
	glColor3f(0.25, 0.25, 0.25);
	if (HienTai == 0) {
		glColor3f(0.25, 0.25, 0.25);
		drawPieslice(147, 250, 90, 270, 95);
	}
	else {
		glColor3f(0.25, 0.25, 0.25);
		drawPieslice(653, 250, -90, 90, 95);
	}
}

void VeMuiTen() {
	glColor3f(0.0, 1.0, 0.0);
	int m, n = (7 - Abs(6 - HienTai)) * 100;
	int k = (HienTai / 6) * 2 - 1;
	m = Y + k * 110;
	drawLine(n, m, n, m + 30 * k);
	drawLine(n, m, n + 10 * k, m + 10 * k);
	drawLine(n, m, n - 10 * k, m + 10 * k);
}

void XoaMuiTen() {
	glColor3f(0.25, 0.25, 0.25);
	int m, n = (7 - Abs(6 - HienTai)) * 100;
	int k = (HienTai / 6) * 2 - 1;
	m = Y + k * 110;
	drawLine(n, m, n, m + 30 * k);
	drawLine(n, m, n + 10 * k, m + 10 * k);
	drawLine(n, m, n - 10 * k, m + 10 * k);
}

void ChonO() {
	glColor3f(0.0, 1.0, 0.0);
	int n = (7 - Abs(6 - HienTai)) * 100;
	int m = (HienTai / 6 + 2) * 100;
	drawRectangle(n - 48, m - 48, n + 48, m + 48);
	drawRectangle(n - 46, m - 46, n + 50, m + 50);
}

void BoChonO() {
	glColor3f(0.0, 0.0, 0.0);
	int n = (7 - Abs(6 - HienTai)) * 100;
	int m = (HienTai / 6 + 2) * 100;
	drawRectangle(n - 48, m - 48, n + 48, m + 48);
	drawRectangle(n - 46, m - 46, n + 50, m + 50);
}

void moveMuiTen()
{
	VeMuiTen();

	if (UP == true) {
		XoaMuiTen();
		do {
			HienTai -= (1 - 2 * Player);
			if (HienTai == Player * 12) HienTai = Player * 2 + 5;
		} while (oaq[HienTai].soDan == 0);
		VeMuiTen();
		cout << "UP_STATE : " << UP << endl;
	}

	if (DOWN == true) {
		XoaMuiTen();
		do {
			HienTai += (1 - 2 * Player);
			if (HienTai == 6) HienTai = Player * 10 + 1;
		} while (oaq[HienTai].soDan == 0);
		VeMuiTen();
		cout << "DOWN_STATE : " << RIGHT << endl;
	}

	UP = false;
	DOWN = false;
}

void daiDanLienTiep() {
	cout << "WHILE BEGIN" << endl;
	// Dai O
	DaiDan = oaq[HienTai].soDan; // Lấy số dân của ô muốn dải
	oaq[HienTai].soDan = 0; // Lấy hết rồi lên còn lại 0
	while (true) {

		if (DaiDan == 0) {
			HienTai += mx; //Điều hướng ô hiện tại sang vị trí mới trái hoặc phải
			if (HienTai == 12) HienTai = 0;
			else if (HienTai == -1) HienTai = 11;
			if (HienTai % 6) {
				XoaO();
				DaiDan = oaq[HienTai].soDan;
				if (DaiDan == 0) {
					do {
						HienTai += mx;
						if (HienTai == 12) HienTai = 0;
						else if (HienTai == -1) HienTai = 11;
						if (HienTai % 6 == 0) {
							if (oaq[HienTai].conQuan) {
								if (oaq[HienTai].soDan >= 5) {
									Score[Player] += (oaq[HienTai].soDan + 10);

									oaq[HienTai].soDan = 0;
									oaq[HienTai].conQuan = 0;
									XoaOQuan();

									// Điều kiện chiến thắng
									if ((oaq[6 - HienTai].soDan == oaq[6 - HienTai].conQuan) && (oaq[6 - HienTai].conQuan == 0))
									{
										for (int i = 1; i < 6; i++) Score[0] += oaq[i].soDan;
										for (int i = 7; i < 12; i++) Score[1] += oaq[i].soDan;

										WIN_CONFIRMATION = true;
										break;

									}
								}
								else break;
							}
							else if (oaq[HienTai].soDan > 0) {
								Score[Player] += oaq[HienTai].soDan;
								oaq[HienTai].soDan = 0;
								XoaOQuan();
								// Điều kiện chiến thắng
								if ((oaq[6 - HienTai].soDan == oaq[6 - HienTai].conQuan) && (oaq[6 - HienTai].conQuan == 0)) {
									for (int i = 1; i < 6; i++) Score[0] += oaq[i].soDan;
									for (int i = 7; i < 12; i++) Score[1] += oaq[i].soDan;

									WIN_CONFIRMATION = true;
									break;
								}
							}
							else break;
						}
						else if (oaq[HienTai].soDan > 0) {
							Score[Player] += oaq[HienTai].soDan;
							oaq[HienTai].soDan = 0;
							XoaO(); //XoaO - HienTai
						}
						else break;
						HienTai += mx;
						if (HienTai == 12) HienTai = 0;
						else if (HienTai == -1) HienTai = 11;
						DaiDan = oaq[HienTai].soDan;
						//delay(400);
					} while (DaiDan == 0);
					break;
				}
				oaq[HienTai].soDan = 0;
				//delay(400);
			}
			else break;
		}
		DaiDan--;
		HienTai += mx;
		if (HienTai == 12) HienTai = 0;
		else if (HienTai == -1) HienTai = 11;
		oaq[HienTai].soDan++;
		oaq[HienTai].veDan(); //veDan - HienTai + Class
		cout << "END WHILE\n";
	} // while end

	Player = 1 - Player; // Đổi sang người chơi khác

	//Sử ly khi còn quan mà ở 1 bên người chơi hết dân ở tất cả các ô
	tmp = 1;
	do {
		HienTai = 6 * Player + tmp;
		tmp++;
	} while (oaq[HienTai].soDan == 0 && tmp <= 5); //

	if (oaq[HienTai].soDan == 0) {
		Score[Player] -= 5;
		for (int i = 1 + 6 * Player; i < 6 + 6 * Player; i++) {
			VeDan(i, 1);
			oaq[i].soDan = 1;
		}
	}

	cout << "FINAL Hientai : " << HienTai << endl;
}

void defautScore() {
	glColor3f(1.0, 0.498039, 0.0);
	char TextDefaut1[] = "PLAYER 1: ";
	char TextDefaut2[] = "PLAYER 2: ";
	DisplayBitMapP1(TextDefaut1);
	DisplayBitMapP2(TextDefaut2);
}

void loadScore() {
	glColor3f(1.0, 0.498039, 0.0);

	char P1_1[40] = "PLAYER 1: ";
	char P2_2[40] = "PLAYER 2: ";

	sprintf_s(buffer1, "%d  ", Score[0]);
	strcat_s(P1_1, buffer1);
	DisplayBitMapP1(P1_1);

	sprintf_s(buffer2, "%d  ", Score[1]);
	strcat_s(P2_2, buffer2);
	DisplayBitMapP2(P2_2);
}

void run() {
	moveMuiTen();
	VeMuiTen();

	if (ENTER == true) {
		ChonO();
	}

	if (LEFT == true) {
		mx = Player * 2 - 1;
		XoaMuiTen();
		XoaO();
		daiDanLienTiep();
		//loadScore();
		LEFT = false;
	}

	if (RIGHT == true) {
		mx = 1 - Player * 2;
		XoaMuiTen();
		XoaO();
		daiDanLienTiep();
		//loadScore();
		RIGHT = false;
	}

	VeMuiTen();
	ENTER = false;
}

void initWindows() {

	glClearColor(0.25, 0.25, 0.25, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, MAXX + 9, MAXY + 13, 0.0, 1, -1);
	SoundEngine->play2D(BACK_GROUND_SOUND_PATH, GL_TRUE);
}

// BITMAP - OK
void drawPreviousTable()
{
	drawBanEmty();
	for (int i = 0; i < 12; i++)
	{
		if (i % 6 != 0)
		{
			for (int j = 1; j <= oaq[i].soDan; j++)
				VeDan(i, j);
		}
		else
		{
			if (oaq[i].conQuan != 0)
				VeQuan(i);
			for (int j = 1; j <= oaq[i].soDan; j++)
			{
				//oaq[i].soDan++;
				VeDan2(i, j);
			}
		}
	}
}

void confirmWinnerShow() {
	SoundEngine->play2D(WINNING_SOUND_PATH, GL_TRUE);
	glColor3f(1.0, 0.498039, 0.0);

	char P1_1_Win[40] = "PLAYER 1: ";
	char P2_2_Win[40] = "PLAYER 2: ";
	char Winner[100] = "O AN QUAN WINNER IS PLAYER : ";

	sprintf_s(buffer1, "%d  ", Score[0]);
	strcat_s(P1_1_Win, buffer1);
	DisplayBitMapP1_WinScore(P1_1_Win);

	sprintf_s(buffer2, "%d  ", Score[1]);
	strcat_s(P2_2_Win, buffer2);
	DisplayBitMapP2_WinScore(P2_2_Win);

	sprintf_s(buffer3, "%d  ", Score[0] > Score[1] ? 1 : 2);
	strcat_s(Winner, buffer3);
	DisplayWinResultText(Winner);
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cout << "\nStart Rendering\n";

	if (WIN_CONFIRMATION == false) {
		defautScore();
		DisplayBitMapName(GAME_NAME);

		if (check == 0)
		{
			drawBan();
			check++;
		}
		if (check != 0)
		{
			drawPreviousTable();
		}
		run();
		loadScore();
		cout << "\nHienTai = " << HienTai << endl;
	}

	else {
		confirmWinnerShow();
	}
	glutSwapBuffers();
}

//Mouse Pressing Events
void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		//store the x,y value where the click happened
		cout << x << " " << y << endl;
		ENTER = true;
		SoundEngine->play2D(ENTER_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//store the x,y value where the click happened
		cout << x << " " << y << endl;
		LEFT = true;
		MousePlayer = 1 - MousePlayer;
		SoundEngine->play2D(LEFT_RIGHT_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//store the x,y value where the click happened
		cout << x << " " << y << endl;
		RIGHT = true;
		MousePlayer = 1 - MousePlayer;
		SoundEngine->play2D(LEFT_RIGHT_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
	}
}

void PassiveMotionMouse(int x, int y) {
	cout << x << " " << y << endl;

	switch (MousePlayer) {
	case 0:
		if (x > 150 && x < 250 && y > 150 && y < 250) {
			if (oaq[1].soDan != 0) {
				HienTai = 1;
			}
			glutPostRedisplay();
		}

		if (x > 250 && x < 350 && y > 150 && y < 250) {
			if (oaq[2].soDan != 0) {
				HienTai = 2;
			}
			glutPostRedisplay();
		}

		if (x > 350 && x < 450 && y > 150 && y < 250) {
			if (oaq[3].soDan != 0) {
				HienTai = 3;
			}
			glutPostRedisplay();
		}

		if (x > 450 && x < 550 && y > 150 && y < 250) {
			if (oaq[4].soDan != 0) {
				HienTai = 4;
			}
			glutPostRedisplay();
		}

		if (x > 550 && x < 650 && y > 150 && y < 250) {
			if (oaq[5].soDan != 0) {
				HienTai = 5;
			}
			glutPostRedisplay();
		}
		break;
	case 1:
		if (x > 150 && x < 250 && y > 250 && y < 350) {
			if (oaq[11].soDan != 0) {
				HienTai = 11;
			}
			glutPostRedisplay();
		}

		if (x > 250 && x < 350 && y > 250 && y < 350) {
			if (oaq[10].soDan != 0) {
				HienTai = 10;
			}
			glutPostRedisplay();
		}

		if (x > 350 && x < 450 && y > 250 && y < 350) {
			if (oaq[9].soDan != 0) {
				HienTai = 9;
			}
			glutPostRedisplay();
		}

		if (x > 450 && x < 550 && y > 250 && y < 350) {
			if (oaq[8].soDan != 0) {
				HienTai = 8;
			}
			glutPostRedisplay();
		}

		if (x > 550 && x < 650 && y > 250 && y < 350) {
			if (oaq[7].soDan != 0) {
				HienTai = 7;
			}
			glutPostRedisplay();
		}
		break;
	}

	//Sound
	if (x == 150 || x == 250 || x == 350 || x == 450 || x == 550 || x == 650) {
		SoundEngine->play2D(UP_DOWN_SOUND_PATH, GL_FALSE);
	}
}

// Key Pressing Events
void processNormalKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // GLUT_KEY_ESC
		BoChonO();
		SoundEngine->play2D(ESC_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
		break;
	case 13: // GLUT_KEY_ENTER
		cout << "Pressed E" << endl;
		ENTER = true;
		SoundEngine->play2D(ENTER_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
		break;
	}
}

void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		cout << "Pressed U" << endl;
		UP = true;
		SoundEngine->play2D(UP_DOWN_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		cout << "Pressed D" << endl;
		DOWN = true;
		SoundEngine->play2D(UP_DOWN_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		cout << "Press L" << endl;
		LEFT = true;
		SoundEngine->play2D(LEFT_RIGHT_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		cout << "Press R" << endl;
		RIGHT = true;
		SoundEngine->play2D(LEFT_RIGHT_SOUND_PATH, GL_FALSE);
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char **argv) {
	for (int i = 0; i < 12; i++) if (i % 6 == 0) oaq[i].soDan = 0;
	cout << "Main_Start\n";

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(MAXX + 9, MAXY + 13);
	glutCreateWindow("GAME O AN QUAN : DINH DUONG & MINH HOANG COPYRIGHT ©");

	// register callbacks
	initWindows();
	glutDisplayFunc(renderScene);

	//Key Pressing
	glutKeyboardFunc(processNormalKeys); //ESC
	glutSpecialFunc(processSpecialKeys);

	//Mouse Pressing
	glutPassiveMotionFunc(PassiveMotionMouse);
	glutMouseFunc(OnMouseClick);

	// enter GLUT event processing loop
	glutMainLoop();

	return 0;
}