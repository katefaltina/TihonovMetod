
// TihonovMetodDlg.h: файл заголовка
//

#pragma once

#include "Drawer.h"
#include "Drawer2.h"
#include <complex>

// Диалоговое окно CTihonovMetodDlg
class CTihonovMetodDlg : public CDialogEx
{
// Создание
public:
	CTihonovMetodDlg(CWnd* pParent = nullptr);	// стандартный конструктор

	struct cmplx
	{
		float real;
		float image;
	};
	cmplx* CmplxH;
	cmplx* CmplxG;
	cmplx* CmplxU;
	//void fourea(struct cmplx* data, int n, int is);
	//void SpectrCalc(vector <double>);

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIHONOVMETOD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk(); // построить исходный график u, импульсную хар-ку h и свертку g
	afx_msg void OnBnClickedDeconv(); // построить восстановленный сигнал u* 
	afx_msg void OnBnClickedErrFromEnoise(); // построить график зависимости невязки от энергии шума

	// параметры сигнала u
	double _A1;
	double _A2;
	double _A3;
	double _m1;
	double _m2;
	double _m3;
	double _sigma1;
	double _sigma2;
	double _sigma3;

	int _N; // количество отсчетов
	double _fd; // частота

	Drawer Signal;
	double x(int); // функция исходного сигнала
	double signal_max; // max амплитуда сигнала

	// импульсная характеристика h
	double _Ah;
	double _sigmah;

	Drawer Impulse;
	double h(int); // функция импульсной характеристики

	// свёртка g
	Drawer Convolution;
	double conv_max; // max амплитуда свёртки

	void Noise(); // функция шума
	double noise_max; // max амплитуда сигнала с шумом
	double _noise; // параметр шума (задаю)

	Drawer Signal_vosst;
	void MetodTihonova();
	double alfa; // параметр (задаю)
	double max; // max амплитуда восстановленного сигнала

	//vector<double> R; 
	double reg(int);

	void FFT(vector<complex<double>>& in, vector<complex<double>>& out, int direction);
	void make_signal();
	void make_impulse();
	void make_convolution();

	vector <double> signal; 
	vector <double> N_signal;

	vector <double> impulse;
	vector <double> N_impulse;

	vector <double> convolution;
	vector <double> N_convolution;

	// спектры u, h, g
	vector<complex<double>> signal_spectre;
	vector<complex<double>> impulse_spectre;
	vector<complex<double>> convolution_spectre;

	vector<complex<double>> tihonov_signal_spectre; // спектр u*
	vector<complex<double>> tihonov_signal;

	vector<double> tihonov_signal_real; // массив восстановленного сигнала u*
	vector<double> N_tihonov_signal_real; // точки 

	vector<complex<double>> to_complex(vector <double>& data);
	vector<double> get_real_part(vector <complex<double>>& data);

	// зависимость невязки от шума
	Drawer2 ErrEnoise;
	void Ust();
	double otclon;
	vector<double> err_from_Enoise;
	vector<double> N_Enoise;
	double NOISE;
	double err_max = 0, NoiMax = 0;
};
