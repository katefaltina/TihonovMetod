// TihonovMetodDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "TihonovMetod.h"
#include "TihonovMetodDlg.h"
#include "afxdialogex.h"
#include <complex>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

// Диалоговое окно CTihonovMetodDlg

CTihonovMetodDlg::CTihonovMetodDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIHONOVMETOD_DIALOG, pParent)
	, _A1(1)
	, _A2(0.5)
	, _A3(1)
	, _m1(0.4)
	, _m2(0.8)
	, _m3(1.6)
	, _sigma1(0.1)
	, _sigma2(0.1)
	, _sigma3(0.15)
	, _Ah(1)
	, _sigmah(0.1)
	, _N(256)
	, _fd(125)
	, _noise(1)
	, alfa(0.4)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTihonovMetodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_A1, _A1);
	DDX_Text(pDX, IDC_A2, _A2);
	DDX_Text(pDX, IDC_A3, _A3);
	DDX_Text(pDX, IDC_M1, _m1);
	DDX_Text(pDX, IDC_M2, _m2);
	DDX_Text(pDX, IDC_M3, _m3);
	DDX_Text(pDX, IDC_SIGMA1, _sigma1);
	DDX_Text(pDX, IDC_SIGMA2, _sigma2);
	DDX_Text(pDX, IDC_SIGMA3, _sigma3);
	DDX_Text(pDX, IDC_AH, _Ah);
	DDX_Text(pDX, IDC_SIGMAH, _sigmah);
	DDX_Text(pDX, IDC_N, _N);
	DDX_Text(pDX, IDC_FD, _fd);
	/*DDX_Control(pDX, IDC_RADIO1, before);
	DDX_Control(pDX, IDC_RADIO2, after);*/
	DDX_Text(pDX, IDC_NOISE, _noise);
	DDX_Text(pDX, IDC_ALFA, alfa);
}

BEGIN_MESSAGE_MAP(CTihonovMetodDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTihonovMetodDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DECONV, &CTihonovMetodDlg::OnBnClickedDeconv)
	ON_BN_CLICKED(IDC_ERR_FROM_ENOISE, &CTihonovMetodDlg::OnBnClickedErrFromEnoise)
END_MESSAGE_MAP()


// Обработчики сообщений CTihonovMetodDlg

BOOL CTihonovMetodDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	Signal.Create(GetDlgItem(IDC_SIGNAL)->GetSafeHwnd());
	//Impulse.Create(GetDlgItem(IDC_IMPULSE)->GetSafeHwnd());
	Convolution.Create(GetDlgItem(IDC_SVERTKA)->GetSafeHwnd());
	Signal_vosst.Create(GetDlgItem(IDC_VOSST)->GetSafeHwnd());
	ErrEnoise.Create(GetDlgItem(IDC_OTKLON_E)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTihonovMetodDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTihonovMetodDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// функция гауссовых куполов
double CTihonovMetodDlg::x(int n)
{
	double dt = 1 / _fd;
	double s1 = _A1 * exp(-(pow((n * dt) - _m1, 2) / (2 * _sigma1 * _sigma1)));
	double s2 = _A2 * exp(-(pow((n * dt) - _m2, 2) / (2 * _sigma2 * _sigma2)));
	double s3 = _A3 * exp(-(pow((n * dt) - _m3, 2) / (2 * _sigma3 * _sigma3)));
	return s1 + s2 + s3;
}

// функция импульсной характеристики
double  CTihonovMetodDlg::h(int n)
{
	double dt = 1.0 / _fd;
	if (n < _N / 2)
	{
		return _Ah * exp(-(n * dt * n * dt) / (2 * _sigmah * _sigmah));
	}
	else
	{
		return _Ah * exp(-((n - _N) * dt * (n - _N) * dt) / (2 * _sigmah * _sigmah));
	}
}

// шум (накладываем на выходной сигнал, полученный после свёртки)
void CTihonovMetodDlg::Noise()
{
	double* g = new double[_N];
	for (int i = 0; i < _N; i++)
	{
		g[i] = 0;
		for (int k = 0; k < 12; k++) g[i] += rand();
	}
	double betta = 0;
	double res_noize = 0; double res_g = 0;
	for (int i = 0; i < _N; i++)
	{
		res_noize += convolution[i] * convolution[i];
		res_g += g[i] * g[i];
	}

	betta = sqrt(_noise * res_noize / res_g/100);
	noise_max = 0;
	for (int i = 0; i < _N; i++)
	{
		convolution[i] = convolution[i] + betta * g[i];
		if (convolution[i] > noise_max) noise_max = convolution[i];
	}
}

// сигнал и фурье от него
void CTihonovMetodDlg::make_signal()
{
	N_signal.clear(); signal.clear();
	signal_max = 0;
	for (int n = 0; n < _N; n++)
	{
		N_signal.push_back(n /_fd);
		signal.push_back(x(n));
		if (signal[n] > signal_max) signal_max = signal[n];
	}
	FFT(to_complex(signal), signal_spectre, -1);
}

// импульсная характеристика и фурье от нее
void CTihonovMetodDlg::make_impulse()
{
	N_impulse.clear(); impulse.clear();
	for (int n = 0; n < _N; n++)
	{
		impulse.push_back(h(n));
		N_impulse.push_back(n / _fd);
	}
	FFT(to_complex(impulse), impulse_spectre, -1);
}

// вычисление свертки, наложение шума и фурье от свертки с шумом
void CTihonovMetodDlg::make_convolution()
{
	convolution.clear(); N_convolution.clear();
	conv_max = 0;
	double res;
	for (int i = 0; i < _N; i++)
	{
		res = 0;
		for (int k = 0; k < _N; k++)
		{
			res += signal[k] * impulse[abs(k - i)];
		}
		convolution.push_back(res);
		if (convolution[i] > conv_max) conv_max = convolution[i];
		N_convolution.push_back(i / _fd);
	}
	Noise(); // наложение шума
	conv_max = 0;
	for (int i = 0; i < _N; i++)
	{
		if (convolution[i] > conv_max) conv_max = convolution[i];
	}
	FFT(to_complex(convolution), convolution_spectre, -1);
}

vector<complex<double>> CTihonovMetodDlg::to_complex(vector <double>& data)
{
	vector<complex<double>> res;
	for (auto & sample : data)
	{
		res.push_back({sample, 0});
	}
	return res;
}

// построить исходный график u, импульсную хар-ку h и свертку g
void CTihonovMetodDlg::OnBnClickedOk()
{
	UpdateData(true);

	// сигнал
	make_signal();
	Signal.Draw(3, signal, N_signal, _N, signal_max, N_signal[_N - 1]);
	Signal.DrawRis(3, 1, signal, N_signal, _N, signal_max, N_signal[_N - 1]);
	
	// импульсная характеристика
	make_impulse();
	Signal.DrawRis(3, 2, impulse, N_impulse, _N, _Ah, N_impulse[_N - 1]);

	// свёртка
	make_convolution();
	Convolution.Draw(3, convolution, N_convolution, _N, conv_max, N_convolution[_N - 1]);
	Convolution.DrawRis(3, 1, convolution, N_convolution, _N, conv_max, N_convolution[_N - 1]);
}

// фурье
void CTihonovMetodDlg::FFT(vector<complex<double>>& in, vector<complex<double>>& out, int direction)
{
	out = in;
	unsigned int pts = 2;
	while (out.size() > pts)
	{
		pts *= 2;
	}

	int pts_to_add = pts - out.size();

	for (int ttt = 0; ttt < pts_to_add; ttt++)
	{
		out.push_back(complex<double>(0, 0));
	}
	int n = out.size();

	int i, j, istep;
	int m, mmax;
	double r, r1, theta, w_r, w_i, temp_r, temp_i;

	r = M_PI * direction;
	j = 0;

	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = out[j].real();
			temp_i = out[j].imag();
			out[j] = out[i];
			out[i] = complex<double>(temp_r, temp_i);
		}
		m = n >> 1;
		while (j >= m)
		{
			j -= m;
			m = (m + 1) / 2;
		}
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (double)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (double)cos((double)theta);
			w_i = (double)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * out[j].real() - w_i * out[j].imag();
				temp_i = w_r * out[j].imag() + w_i * out[j].real();
				out[j] = complex<double>((out[i].real() - temp_r), (out[i].imag() - temp_i));
				out[i] += complex<double>(temp_r, temp_i);
			}
		}
		mmax = istep;
	}
	if (direction > 0)
	{
		double sqn = sqrt(n);
		for (i = 0; i < n; i++)
		{
			out[i] /= sqn;
		}
	}
}

// функция Q
double CTihonovMetodDlg::reg(int n)
{
	if (n < _N / 2) return pow((n * _fd / _N), 2);
	else return pow(_fd - (n * _fd / _N), 2);
}

// получение реальной части
vector<double> CTihonovMetodDlg::get_real_part(vector <complex<double>>& data)
{
	vector<double> res;
	for (auto& sample : data)
	{
		res.push_back(sample.real());
	}
	return res;
}

// метод Тихонова восстановление сигнала
void CTihonovMetodDlg::MetodTihonova()
{
	// X = U / (H + alfa* R)
	tihonov_signal_spectre.clear();
	for (int i = 0; i < _N; i++)
	{
		//tihonov_signal_spectre.push_back(convolution_spectre[i] / (impulse_spectre[i] + 1.0));
		tihonov_signal_spectre.push_back(convolution_spectre[i] / (impulse_spectre[i] + alfa * reg(i)));
	}
	FFT(tihonov_signal_spectre, tihonov_signal, 1);

	tihonov_signal_real = get_real_part(tihonov_signal);

	max = 0; otclon = 0;
	N_tihonov_signal_real.clear();
	for (int i = 0; i < _N; i++)
	{
		N_tihonov_signal_real.push_back(i/_fd);
		if (tihonov_signal_real[i] > max) max = tihonov_signal_real[i];
		// невязка
		otclon += pow(tihonov_signal_real[i] - signal[i], 2);
	}
	otclon = otclon / _N;
}

// построить восстановленный сигнал u* 
void CTihonovMetodDlg::OnBnClickedDeconv()
{
	UpdateData(true);

	MetodTihonova();
	Signal_vosst.Draw(2, tihonov_signal_real, N_tihonov_signal_real, _N, max, N_tihonov_signal_real[_N-1]);
	Signal_vosst.DrawRis(2, 2, tihonov_signal_real, N_tihonov_signal_real, _N, max, N_tihonov_signal_real[_N - 1]);
	Signal_vosst.DrawRis(2, 1, signal, N_signal, signal.size(), 1, N_signal[_N - 1]);
}

// исследование устойчивости к шуму (зависимость невязки от шума)
void CTihonovMetodDlg::Ust()
{
	for (double iNoi = 0; iNoi <= _noise; iNoi += _noise * 0.1)
	{
		NOISE = iNoi;

		convolution.clear(); N_convolution.clear();
		double res;
		for (int i = 0; i < _N; i++) {
			res = 0;
			for (int k = 0; k < _N; k++)
			{
				res += signal[k] * impulse[abs(k - i)];
			}
			convolution.push_back(res);
			N_convolution.push_back(i / _fd);
		}

		double* g = new double[_N];
		for (int i = 0; i < _N; i++) {
			g[i] = 0;
			for (int k = 0; k < 12; k++) g[i] += rand();
		}
		double betta = 0, E_s = 0, E_g = 0;
		for (int i = 0; i < _N; i++) {
			E_s += convolution[i] * convolution[i];
			E_g += g[i] * g[i];
		}
		betta = sqrt(NOISE * E_s / E_g/100);
		for (int i = 0; i < _N; i++) {
			convolution[i] = convolution[i] + betta * g[i];
		}
		FFT(to_complex(convolution), convolution_spectre, -1);

		MetodTihonova();

		// отклонение от шума
		if (err_max < otclon)  err_max = otclon;
		err_from_Enoise.push_back(otclon);

		if (NoiMax < iNoi)  NoiMax = iNoi;
		N_Enoise.push_back(iNoi*100);

		ErrEnoise.Draw(err_from_Enoise, 0, 50., NoiMax*100, 0.5 + err_max, N_Enoise);
	}
}

// построить график зависимости
void CTihonovMetodDlg::OnBnClickedErrFromEnoise()
{
	err_from_Enoise.clear(); N_Enoise.clear();

	UpdateData(true);

	Ust();
}