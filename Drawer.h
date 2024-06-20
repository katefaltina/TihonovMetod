#pragma once

#include <afxwin.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
using namespace std;

class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, области рисования.
	CWnd* wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC* dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;
	// Флаг для отслеживания состояния инициализации класса.
	bool init;

public:


	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd)
	{
		// Получаем указатель на окно.
		wnd = CWnd::FromHandle(hWnd);
		// Получаем прямоугольник окна.
		wnd->GetClientRect(frame);
		// Получаем контекст для рисования в этом окне.
		dc = wnd->GetDC();

		// Создаем буфер-контекст.
		memDC.CreateCompatibleDC(dc);
		// Создаем растр для контекста рисования.
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		// Выбираем растр для использования буфер-контекстом.
		memDC.SelectObject(&bmp);
		init = true;
	}


	//функция для отрисовки сетки

	void Draw(int typefunc, vector<double> y_first_graph, vector<double> x_first_graph, int N, double A, double x_max2)
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Курсив
			FALSE,				// Подчеркнутый
			FALSE,				// Перечеркнутый
			DEFAULT_CHARSET,	// Набор символов
			OUT_OUTLINE_PRECIS,	// Точность соответствия.	
			CLIP_DEFAULT_PRECIS,//  
			100,	// Качество
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
			);

		double padding = 20;
		double left_keys_padding = 20;
		double bottom_keys_padding = 10;

		double actual_width = frame.Width() - 2 * padding - left_keys_padding;
		double actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		double actual_top = padding;
		double actual_bottom = actual_top + actual_height;
		double actual_left = padding + left_keys_padding;
		double actual_right = actual_left + actual_width;

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		double grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		//при вызове метода отрисовки, мы указываем "порядковый номер" графика
		//который позволяет выбрать нужную область

		double data_y_min(0), data_y_max(0);
		double data_x_min(0), data_x_max(0);

		switch (typefunc)
		{
		case 1: //дискретизация
			data_y_min = -A;
			data_y_max = A;
			data_x_min = 0;
			data_x_max = x_max2;
			break;
		case 2: //дискретизация
			data_y_min = 0;
			data_y_max = A;
			data_x_min = 0;
			data_x_max = x_max2;
			break;
		case 3: //дискретизация
			data_y_min = 0;
			data_y_max = A;
			data_x_min = 0;
			data_x_max = x_max2;
			break;
		}

		vector<double> y = convert_range(y_first_graph, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(x_first_graph, actual_right, actual_left, data_x_max, data_x_min);

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));


		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.2f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}


		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void DrawRis(int typefunc, int pen_number, vector<double>& y_first_graph, vector<double>& x_first_graph, int N, double A, double x_max2)
	{
		if (!init) return;

		if (y_first_graph.empty()) return;

		if (x_first_graph.size() != y_first_graph.size())
		{
			x_first_graph.resize(y_first_graph.size());
			for (int i = 0; i < x_first_graph.size(); i++)
			{
				x_first_graph[i] = i;
			}
		}


		CPen data_pen1(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_pen2(PS_SOLID, 2, RGB(0, 0, 0));
		CPen data_pen3(PS_SOLID, 2, RGB(0, 0, 255));

		double padding = 20;
		double left_keys_padding = 20;
		double bottom_keys_padding = 10;

		double actual_width = frame.Width() - 2 * padding - left_keys_padding;
		double actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		double actual_top = padding;
		double actual_bottom = actual_top + actual_height;
		double actual_left = padding + left_keys_padding;
		double actual_right = actual_left + actual_width;

		double data_y_min(0), data_y_max(0);
		double data_x_min(0), data_x_max(0);

		switch (typefunc)
		{
		case 1: //дискретизация
			data_y_min = -A;
			data_y_max = A;
			data_x_min = 0;
			data_x_max = x_max2;
			break;
		case 2: //дискретизация
			data_y_min = 0;
			data_y_max = A;
			data_x_min = 0;
			data_x_max = x_max2;
			break;
		case 3: //дискретизация
			data_y_min = 0;
			data_y_max = A;
			data_x_min = 0;
			data_x_max = x_max2;
			break;

		}
		switch (pen_number) {
		case 1:
			memDC.SelectObject(&data_pen1);
			break;
		case 2:
			memDC.SelectObject(&data_pen2);
			break;
		case 3:
			memDC.SelectObject(&data_pen3);
		}


		vector<double> y = convert_range(y_first_graph, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(x_first_graph, actual_right, actual_left, data_x_max, data_x_min);

		for (int i = 0; i < N - 1; i++)
		{
			memDC.MoveTo(x[i], y[i]);
			memDC.LineTo(x[i + 1], y[i + 1]);
		}


		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);


	}

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
	{
		vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
};

class Drawer2
{
	CRect frame;
	CWnd* wnd;
	CDC* dc;
	CDC memDC;
	CBitmap bmp;
	bool init;

	CPen subgrid_pen;
	CPen grid_pen;
	CPen data_pen;
	CPen data_pen2;
	CPen data_pen3;
	CPen data_pen4;
	CFont font;

	int actual_top;
	int actual_bottom;
	int actual_left;
	int actual_right;

	vector<double> y;
	vector<double> x;
public:
	void Create(HWND hWnd)
	{
		wnd = CWnd::FromHandle(hWnd);
		wnd->GetClientRect(frame);
		dc = wnd->GetDC();
		memDC.CreateCompatibleDC(dc);
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		memDC.SelectObject(&bmp);

		subgrid_pen.CreatePen(PS_DOT, 1, RGB(200, 200, 200));
		grid_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		data_pen.CreatePen(PS_SOLID, 2.9, RGB(0, 0, 255));
		data_pen2.CreatePen(PS_SOLID, 2.9, RGB(255, 0, 0));
		data_pen3.CreatePen(PS_SOLID, 3, RGB(57, 255, 20));
		data_pen4.CreatePen(PS_SOLID, 2.5, RGB(127, 72, 41));

		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Курсив
			FALSE,				// Подчеркнутый
			FALSE,				// Перечеркнутый
			DEFAULT_CHARSET,	// Набор символов
			OUT_OUTLINE_PRECIS,	// Точность соответствия.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// Качество
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
			);

		init = true;
	}

	void Draw(vector<double>& data, double xMin, double yMin, double xMax, double yMax, vector<double>& keys = vector<double>())
	{
		if (!init) return;

		int padding = 20;
		int left_keys_padding = 23;
		int bottom_keys_padding = 10;
		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// Белый фон  /////////////////////////////////////////////
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку ////////////////////////////////////////
		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		// подсетка
		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}
		memDC.SelectObject(&grid_pen);

		// сетка 
		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}
		if (data.empty()) return;
		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}
		memDC.SelectObject(&data_pen2);
		double data_y_max(yMax), data_y_min(yMin);
		double data_x_max(xMax), data_x_min(xMin);
		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);
		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}

		// подписываем численные значения ///////////////////////////////////////
		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));

		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.2f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}


	void DrawGrid(vector<double>& data, double xMin, double yMin, double xMax, double yMax, vector<double>& keys = vector<double>()) {
		if (!init) return;

		int padding = 20;
		int left_keys_padding = 23;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		actual_top = padding;
		actual_bottom = actual_top + actual_height;
		actual_left = padding + left_keys_padding;
		actual_right = actual_left + actual_width;

		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}


		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		if (data.empty()) return;

		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}
		double data_y_max(yMax), data_y_min(yMin);
		double data_x_max(xMax), data_x_min(xMin);

		y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);


		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));

		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;

			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));

			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.2f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void DrawGraphsAnimation(int number, int n, bool ball = false) {
		if (!init) return;

		memDC.SelectObject(&data_pen);
		memDC.MoveTo(x[number], y[number]);
		memDC.LineTo(x[number + 1], y[number + 1]);
		CPen data_pen_drowDots(PS_SOLID, 2, RGB(0, 0, 0));
		int drowDots = 9;
		memDC.SelectObject(&data_pen_drowDots);
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		if (ball) dc->Ellipse(x[number + 1] + drowDots, y[number + 1] + drowDots, x[number + 1] - drowDots, y[number + 1] - drowDots);
		if (number >= n - 1) {
			memDC.FillSolidRect(frame, RGB(255, 255, 255));
			dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		}
	}

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
	{
		vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
};