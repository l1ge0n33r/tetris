//-----------------------------//  
//
//	Written together with Vadym Tolkachev
//
//-----------------------------//


#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>

using namespace sf;

const int M = 20; // height of field
const int N = 10; // width of field

int field[M][N] = { 0 }; // game filed

RenderWindow window(VideoMode(320, 480), "The Game!");
int dx = 0;
bool rotate = false;
int colorNum = 1;

const float DELAY = 1;

float timer = 0, delay = DELAY;

// array of tetramino
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // S
	3,5,4,6, // Z
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point
{
	int x, y;
} a[4], b[4];

bool IsCollided()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
		{
			return true;
		}
		else if (field[a[i].y][a[i].x])
		{
			return true;
		}
	}

	return false;
}

void HandleEvents()
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();

		if (event.type == Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case Keyboard::Left:
				dx = -1;
				break;

			case Keyboard::Right:
				dx = 1;
				break;

			case Keyboard::Up:
				rotate = true;
				break;

			case Keyboard::Down:
				delay = 0.1;
				break;
			}
		}

		if (event.type == Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case Keyboard::Down:
				delay = DELAY;
				break;
			}
		}
	}
}

void HandleTetraminoHorizontalMovements()
{
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].x += dx;
	}

	if (IsCollided())
	{
		for (int i = 0; i < 4; i++)
		{
			a[i] = b[i];
		}
	}
}

void HandleTetraminoRotations()
{
	if (rotate)
	{
		Point p = a[1];
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}

		if (IsCollided())
		{
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
		}
	}
}

void HandleTetraminoLanding()
{

	int count = 0;
	for (int i = M - 1; i >= 0; --i)
	{
		count = 0;
		for (int j = 0; j < N; ++j)
		{
			if (field[i][j] != 0)
			{
				++count;
			}
		}

		if (count == N)
		{
			for (int k = i; k >=0; --k)
			{
				for (int j = 0; j < N; ++j)
				{
					if(k!=M)
						field[k][j] = field[k-1][j];
				}
			}
			i = M;
		}
	}
}

void HandleTetraminoVerticalMovements(Sprite& sprite)
{
	if (timer > delay)
	{
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].y += 1;
		}

		if (IsCollided())
		{
			for (int i = 0; i < 4; i++)
			{
				field[b[i].y][b[i].x] = colorNum;
			}

			colorNum = rand() % 8;

			
			int n = rand() % 7;
			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
			}
		}
		timer = 0;
		HandleTetraminoLanding();

	}
}
void DrawFieldAndMovingTetramino(Sprite& sprite, Sprite& sprite_background, Sprite& sprite_frame)
{
	window.clear(Color::White);
	window.draw(sprite_background);


	for (int i = 0; i < 4; i++)
	{
		sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
		sprite.setPosition(a[i].x * 18, a[i].y * 18);
		sprite.move(28, 31);
		window.draw(sprite);
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (field[i][j] == 0)
				continue;


			sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
			sprite.setPosition(j * 18, i * 18);
			sprite.move(28, 31);
			window.draw(sprite);
		}
	}
	window.draw(sprite_frame);
	window.display();
}


int main()
{
	Texture texture, texture_background, texture_frame;
	texture.loadFromFile("tiles.jpg");
	texture_background.loadFromFile("background.jpg");
	texture_frame.loadFromFile("frame.png");

	Sprite sprite(texture), sprite_background(texture_background), sprite_frame(texture_frame);
	sprite.setTextureRect(IntRect(0, 0, 18, 18));
	Clock clock;

	

	int n = 3;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}

	int i = 0;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		HandleEvents();
		HandleTetraminoHorizontalMovements();
		HandleTetraminoRotations();
		HandleTetraminoVerticalMovements(sprite);

		dx = 0;
		rotate = 0;

		DrawFieldAndMovingTetramino(sprite, sprite_background, sprite_frame);
	}

	return 0;
}
