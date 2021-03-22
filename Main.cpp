#include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "state.hpp"
#include "tic_tac_toe.hpp"

inline tic_tac_toe::FieldState get_enemy(tic_tac_toe::FieldState state);

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));

	const Font font(30);
	int fontX = 20, fontY = 5;
	Color color = Palette::Black;

	Window::Resize(Size(800, 700));

	const Texture cat(Emoji(U"🍉"));
	const Texture dog(Emoji(U"🍔"));

	std::vector<Vec2> catPos;
	std::vector<Vec2> dogPos;

	std::vector<std::vector<tic_tac_toe::FieldState>> field(tic_tac_toe::SIZE, std::vector<tic_tac_toe::FieldState>(tic_tac_toe::SIZE));

	const int play_out_num = 100000;

	double r = 0.0;
	double max, min;
	int max_idx, min_idx;

	int pos[tic_tac_toe::SIZE] = {157, 272, 387, 502, 617};

	srand((unsigned)time(NULL));
	// AIの順番を決める
	tic_tac_toe::FieldState AIside = tic_tac_toe::FieldState::NONE;
	// 人の順番を決める
	tic_tac_toe::FieldState PERSONside = tic_tac_toe::FieldState::NONE;

	tic_tac_toe::FieldState turn = tic_tac_toe::FieldState::NONE;

	int finish_flag = 3;

	while (System::Update())
	{
		if (AIside != tic_tac_toe::FieldState::NONE)
		{
			font(U"先行：", ((AIside == tic_tac_toe::FieldState::FIRST) ? U"AI" : U"プレイヤー")).draw(fontX, fontY, color);
		}
		// 盤面の表示
		// 横
		Rect(95, 210, 575, 10).draw(Palette::Black);
		Rect(95, 325, 575, 10).draw(Palette::Black);
		Rect(95, 440, 575, 10).draw(Palette::Black);
		Rect(95, 555, 575, 10).draw(Palette::Black);
		// 縦
		Rect(210, 95, 10, 575).draw(Palette::Black);
		Rect(325, 95, 10, 575).draw(Palette::Black);
		Rect(440, 95, 10, 575).draw(Palette::Black);
		Rect(555, 95, 10, 575).draw(Palette::Black);

		// コマの表示
		for (int i = 0; i < catPos.size(); ++i)
		{
			cat.resized(100).drawAt(catPos[i]);
		}

		for (int i = 0; i < dogPos.size(); ++i)
		{
			dog.resized(100).drawAt(dogPos[i]);
		}


		if (finish_flag == -2)
		{
			// AI のターン
			r += Scene::DeltaTime();
			if (turn == AIside && r >= 0.5)
			{
				r = 0.0;
				State game(field);

				// play out
				for (int i = 0; i < play_out_num; i++)
				{
					game.play_out();
				}

				std::vector<double> result = game.get_result();

				max = -1;
				max_idx = -1;

				min = 100000;
				min_idx = -1;

				int idx = 0;
				int i = 0;
				for (int y = 0; y < tic_tac_toe::SIZE; ++y)
				{
					for (int x = 0; x < tic_tac_toe::SIZE; ++x, ++i)
					{
						if (field[y][x] == tic_tac_toe::FieldState::NONE)
						{
							if (AIside == tic_tac_toe::FieldState::FIRST && max < result[idx])
							{
								max = result[idx];
								max_idx = i;
							}
							else if (AIside == tic_tac_toe::FieldState::SECOND && min > result[idx])
							{
								min = result[idx];
								min_idx = i;
							}

							++idx;
						}
					}
				}

				if (AIside == tic_tac_toe::FieldState::FIRST)
				{
					field[max_idx / tic_tac_toe::SIZE][max_idx % tic_tac_toe::SIZE] = AIside;
					dogPos.push_back(Vec2(pos[max_idx % tic_tac_toe::SIZE], pos[max_idx / tic_tac_toe::SIZE]));
				}
				else
				{
					field[min_idx / tic_tac_toe::SIZE][min_idx % tic_tac_toe::SIZE] = AIside;
					dogPos.push_back(Vec2(pos[min_idx % tic_tac_toe::SIZE], pos[min_idx / tic_tac_toe::SIZE]));
				}

				turn = PERSONside;

				finish_flag = tic_tac_toe::is_sinish(field);

			}
			// 人のターンで
			// クリックされたら
			if (turn == PERSONside && MouseL.up())
			{
				// 盤面の中なら
				if (100 <= Cursor::PosF().x && Cursor::PosF().x <= 675 && 100 <= Cursor::PosF().y && Cursor::PosF().y <= 675)
				{
					int x;
					if (Cursor::PosF().x <= 215)
					{
						x = 0;
					}
					else if (Cursor::PosF().x <= 330)
					{
						x = 1;
					}
					else if (Cursor::PosF().x <= 445)
					{
						x = 2;
					}
					else if (Cursor::PosF().x <= 560)
					{
						x = 3;
					}
					else
					{
						x = 4;
					}

					int y;
					if (Cursor::PosF().y <= 215)
					{
						y = 0;
					}
					else if (Cursor::PosF().y <= 330)
					{
						y = 1;
					}
					else if (Cursor::PosF().y <= 445)
					{
						y = 2;
					}
					else if (Cursor::PosF().y <= 560)
					{
						y = 3;
					}
					else
					{
						y = 4;
					}

					// 何も置いていないなら
					if (field[y][x] == tic_tac_toe::FieldState::NONE)
					{
						catPos.push_back(Vec2(pos[x], pos[y]));
						field[y][x] = PERSONside;

						turn = AIside;
						r = 0.0;

						finish_flag = tic_tac_toe::is_sinish(field);
					}
				}
			}
		}
		else
		{
			if (finish_flag == 0)
			{
				font(U"引き分け！！").draw(fontX, fontY + 30, color);
			}
			if (finish_flag == 1)
			{
				font(((AIside == tic_tac_toe::FieldState::FIRST) ? U"AI" : U"プレイヤー"), U"の勝利！！").draw(fontX, fontY + 30, color);
			}
			if (finish_flag == -1)
			{
				font(((AIside == tic_tac_toe::FieldState::SECOND) ? U"AI" : U"プレイヤー"), U"の勝利！！").draw(fontX, fontY + 30, color);
			}
		}

		// ボタンが押されたら
		if (SimpleGUI::Button(U"Start Game", Vec2(600, 20)))
		{
			ClearPrint();

			// 盤面の初期化
			for (int y = 0; y < tic_tac_toe::SIZE; ++y)
			{
				for (int x = 0; x < tic_tac_toe::SIZE; ++x)
				{
					field[y][x] = tic_tac_toe::FieldState::NONE;
				}
			}

			// コマの初期化
			catPos.resize(0);
			dogPos.resize(0);

			finish_flag = -2;

			turn = tic_tac_toe::FieldState::FIRST;
			AIside = (tic_tac_toe::FieldState) (rand() % 2 + 1);
			PERSONside = get_enemy(AIside);
			r = 0.0;
		}
	}
}

inline tic_tac_toe::FieldState get_enemy(tic_tac_toe::FieldState state)
{
	return (state == tic_tac_toe::FieldState::FIRST) ? tic_tac_toe::FieldState::SECOND : tic_tac_toe::FieldState::FIRST;
}