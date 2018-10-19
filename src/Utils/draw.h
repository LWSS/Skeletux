#pragma once

#include "../interfaces.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

namespace Draw
{
	void Circle(Vector2D position, float points, float radius, Color color);
	void OutlinedCircle(int x0, int y0, int points, int radius, Color col);
	void FilledCircle(Vector2D position, float points, float radius, Color color);
	void Circle3D(Vector position, float points, float radius, Color color);
	void FilledRectangle(int x0, int y0, int x1, int y1, Color col);
	void FilledRectangle(Vector2D start_pos, Vector2D end_pos, Color col);
	void Rectangle(int x0, int y0, int x1, int y1, Color col);
	void Rectangle(Vector2D start_pos, Vector2D end_pos, Color col);
	void Line(int x0, int y0, int x1, int y1, Color col);
	void Line(Vector2D start_pos, Vector2D end_pos, Color col);
	void PolyLine(int* px, int* py, int num_points, Color col);
	void PolyLine(Vertex_t* vertice, int num_points, Color col);
	void TexturedPolygon(int n, Vertex_t* vertice, Color col);
	void TextW(int x, int y, const wchar_t* text, HFont font, Color col);
	void TextW(Vector2D pos, const wchar_t* text, HFont font, Color col);
	void Text(int x, int y, const char* text, HFont font, Color col);
	void Text(Vector2D pos, const char* text, HFont font, Color col);
	void GetTextWSize(const wchar_t* text, HFont font, int& wide, int& tall);
	void GetTextSize(const char* text, HFont font, int& wide, int& tall);
	Vector2D GetTextWSize(const wchar_t* text, HFont font);
	Vector2D GetTextSize(const char* text, HFont font);
	HFont CreateFont(const char* fontName, int size, int flag = (int)FontFlags::FONTFLAG_DROPSHADOW);

}