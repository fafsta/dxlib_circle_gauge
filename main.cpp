#include "DxLib.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

void circle_gauge_poly(double center_x, double center_y, double radius, double angle1, double angle2, COLOR_U8 color, std::vector<VERTEX2D>& vect);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	const int screen_width = 1368;
	const int screen_height = 912;

	ChangeWindowMode(TRUE);
	SetGraphMode(screen_width, screen_height, 32);
	if (DxLib_Init() < 0) return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	int texhandle = DX_NONE_GRAPH;
	
	double radius = 400;
	double angle = M_PI_4;
	double phase = 0;
	double speed_rad = M_PI / 180.;
	std::vector<VERTEX2D> vect;
	COLOR_U8 color = GetColorU8(255, 255, 255, 0);

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		
		circle_gauge_poly(screen_width / 2, screen_height / 2, radius, phase, phase + angle, color, vect);
		DrawPolygon2D(vect.data(), vect.size() / 3, texhandle, TRUE);

		phase += speed_rad;

		ScreenFlip();
	}
	// キー入力待ち
	WaitKey();

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

/**********************************************************************************************//**
 * @brief	扇形を形成するポリゴンを取得する
 *
 * @date	2018/09/17
 *
 * @param 		  	center_x	中心のX座標
 * @param 		  	center_y	中心のY座標
 * @param 		  	radius  	扇形の半径
 * @param 		  	angle1  	中心から円弧の端点までの絶対角度1
 * @param 		  	angle2  	中心から円弧の端点までの絶対角度2
 * @param 		  	color   	扇形の色
 * @param [in,out]	vect		作成したポリゴン群を格納する
 **************************************************************************************************/

void circle_gauge_poly(double center_x, double center_y, double radius, double angle1, double angle2, COLOR_U8 color, std::vector<VERTEX2D>& vect) {
	vect.clear();
	double angle_dif = angle2 - angle1;
	int N = static_cast<int>(abs(angle1 - angle2) * 180. / M_PI) / 2 + 1; // 約2度毎にポリゴンを作成

	// 扇形を構成する三角形ポリゴンを作成
	for (int i = 0; i < N; ++i) {
		
		double angle_s = angle1 + angle_dif * i / N;
		double angle_e = angle1 + angle_dif * (i+1) / N;
		double dx1 = radius * cos(angle_s);
		double dy1 = radius * sin(angle_s);
		double dx2 = radius * cos(angle_e);
		double dy2 = radius * sin(angle_e);
		vect.push_back(VERTEX2D());
		vect.push_back(VERTEX2D());
		vect.push_back(VERTEX2D());
		vect[i * 3].pos = VGet(static_cast<float>(center_x), static_cast<float>(center_y), 0.0f);
		vect[i * 3 + 1].pos = VGet(static_cast<float>(center_x + dx1), static_cast<float>(center_y + dy1), 0.0f);
		vect[i * 3 + 2].pos = VGet(static_cast<float>(center_x + dx2), static_cast<float>(center_y + dy2), 0.0f);

		vect[i * 3].dif = vect[i * 3 + 1].dif = vect[i * 3 + 2].dif = color;
		vect[i * 3].rhw = vect[i * 3 + 1].rhw = vect[i * 3 + 2].rhw = 1.0f;
	}
}