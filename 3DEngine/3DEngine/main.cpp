#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cmath>

using namespace std;

float hight = 1200;
float width = 1200;

sf::RenderWindow window(sf::VideoMode(width, hight), "3D Engine");

float fnear = 0.1f;
float ffar = 1000.0f;
float fFov = 90.0f;
float fAspectRatio = hight / width;
float fFovRad = 1.0f / tan(fFov * 0.5f); // в радианах
float fTheta = 0.0f;

struct vec3d // структура координат
{
	float x, y, z;
};

struct triangle // структура представл€юща€ треугольльники
{
	vec3d p[3];
};

struct mesh // структура представл€юща€ объект из всех треугольников
{
	vector<triangle> tris;
};

struct mat4x4
{
	float mat[4][4] = { 0 };
};

void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
{
	o.x = i.x * m.mat[0][0] + i.y * m.mat[1][0] + i.z * m.mat[2][0] + m.mat[3][0];
	o.y = i.x * m.mat[0][1] + i.y * m.mat[1][1] + i.z * m.mat[2][1] + m.mat[3][1];
	o.z = i.x * m.mat[0][2] + i.y * m.mat[1][2] + i.z * m.mat[2][2] + m.mat[3][2];
	float w = i.x * m.mat[0][3] + i.y * m.mat[1][3] + i.z * m.mat[2][3] + m.mat[3][3];

	if (w != 0.0f)
	{
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}
}

void Draw_traingles(float x1, float y1, float x2, float y2, float x3, float y3)
{
	sf::Vertex Lineone[] =
	{
	sf::Vertex(sf::Vector2f(x1, y1)),
	sf::Vertex(sf::Vector2f(x2, y2))
	};
	window.draw(Lineone, 2, sf::Lines);

	sf::Vertex Linetwo[] =
	{
	sf::Vertex(sf::Vector2f(x2, y2)),
	sf::Vertex(sf::Vector2f(x3, y3))
	};
	window.draw(Linetwo, 2, sf::Lines);

	sf::Vertex Linethree[] =
	{
	sf::Vertex(sf::Vector2f(x3, y3)),
	sf::Vertex(sf::Vector2f(x1, y1))
	};
	window.draw(Linethree, 2, sf::Lines);
}

void draw_figure(mesh& triangles, mat4x4& matProj, mat4x4& matRotZ, mat4x4& matRotX)
{
	for (auto tri : triangles.tris)
	{

		//Rotation Z Matrix
		matRotZ.mat[0][0] = cos(fTheta);
		matRotZ.mat[0][1] = sinf(fTheta);
		matRotZ.mat[1][0] = -sinf(fTheta);
		matRotZ.mat[1][1] = cos(fTheta);
		matRotZ.mat[2][2] = 1;
		matRotZ.mat[3][3] = 1;

		//Rotation ZX Matrix
		matRotX.mat[0][0] = 1;
		matRotX.mat[1][1] = cos(fTheta * 0.5f);
		matRotX.mat[1][2] = sinf(fTheta * 0.5f);
		matRotX.mat[2][1] = -sinf(fTheta * 0.5f);
		matRotX.mat[2][2] = cos(fTheta * 0.5f);
		matRotX.mat[3][3] = 1;

		triangle triProj, triTranslated, triRotatedZ, triRotatedZX;

		MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
		MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
		MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

		MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
		MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
		MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

		triTranslated = triRotatedZX; // отодвинул треугольник
		triTranslated.p[0].z = triRotatedZX.p[0].z + 2.5f;
		triTranslated.p[1].z = triRotatedZX.p[1].z + 2.5f;
		triTranslated.p[2].z = triRotatedZX.p[2].z + 2.5f;

		//Normales - unfinished
		/*vec3d normal, line1, line2;
		line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
		line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
		line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

		line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
		line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
		line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

		float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x /= l; normal.y /= l; normal.z /= l;*/


		//if(normal.z < 0)
		//{
		MultiplyMatrixVector(triTranslated.p[0], triProj.p[0], matProj);
		MultiplyMatrixVector(triTranslated.p[1], triProj.p[1], matProj);
		MultiplyMatrixVector(triTranslated.p[2], triProj.p[2], matProj);


		//scale into view
		triProj.p[0].x += 1.0f; triProj.p[0].y += 1.0f;
		triProj.p[1].x += 1.0f; triProj.p[1].y += 1.0f;
		triProj.p[2].x += 1.0f; triProj.p[2].y += 1.0f;

		triProj.p[0].x *= 0.5 * width; triProj.p[0].y *= 0.5 * hight;
		triProj.p[1].x *= 0.5 * width; triProj.p[1].y *= 0.5 * hight;
		triProj.p[2].x *= 0.5 * width; triProj.p[2].y *= 0.5 * hight;

		Draw_traingles(triProj.p[0].x, triProj.p[0].y,
					   triProj.p[1].x, triProj.p[1].y,
			           triProj.p[2].x, triProj.p[2].y);
		//}

		cout << fTheta << endl;
	}
}


int main()
{
	window.setFramerateLimit(165);
	window.setVerticalSyncEnabled(true);

	mesh meshCube;
	mesh pyramid;
	mat4x4 matProj;
	mat4x4 matRotZ, matRotX;

	//Projection Matrix
	matProj.mat[0][0] = fAspectRatio * fFovRad;
	matProj.mat[1][1] = fFovRad;
	matProj.mat[2][2] = ffar / (ffar - fnear);
	matProj.mat[3][2] = (-ffar * fnear) / (ffar - fnear);
	matProj.mat[2][3] = 1.0f;
	matProj.mat[3][3] = 0.0f;

	meshCube.tris = {
		//South
		{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, //инициализирует треугольник
		{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.0f },

		//East
		{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },

		//North
		{ 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },

		//West
		{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },

		//Top
		{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },

		//Bottom
		{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		draw_figure(meshCube, matProj, matRotZ, matRotX);
		fTheta += 0.010f;
		window.display();
	}

	return 0;
}