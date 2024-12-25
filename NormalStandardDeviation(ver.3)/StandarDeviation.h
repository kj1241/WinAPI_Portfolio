#pragma once


class StandarDeviation
{
public:
	StandarDeviation();
	~StandarDeviation();


	double GetMean() { return mean; };
	void SetMean(double m) { mean = m; };

	int GetStep() { return step; };
	void SetStep(int s) { step = s; };

	double GetXMin() { return xMin; };
	void SetXMin(double min) { xMin = min; };

	double GetXMax() { return xMax; };
	void SetXMax(double max) { xMax = max; };

	double GetXStep() { return xStep; };
	void SetXStep(double step) { xStep = step; };

	double GetMinutes() { return minutes; };
	void SetMinutes(double i) { minutes = i; };

	double GetYScaling() { return yScaling; };
	void SetYScaling(double y) { yScaling = y; };

	bool GetIsMaxProbabilityValue() { return isMaxProbabilityValue; };
	void SetIsMaxProbabilityValue(bool y) { isMaxProbabilityValue = y; };

	double GetStddev() { return stddev; }
	void SetStddev(double s) { stddev = s; }

	void Calc(HWND hwnd);

	bool InitStandarDeviation();
	void CloseStandarDeviation();
	void MakeExcle();

private:
	const int WIDTH = 1000;
	const int HEIGHT = 600;
	double mean = 7;    // 중앙값 (7시 30분을 의미)
	double stddev = 0.25;        // 계산된 표준편차가 들어갈 변수
	int step = 400;       // 그래프 부드럽게 그리기 위한 세분화
	double xMin = 6.5;    // 첫 번째 그래프의 x축 범위 (4시 30분)
	double xMax = 7.5;   // 첫 번째 그래프의 x축 범위 (10시 30분)
	double xStep = 1.0;   // 첫 번째 그래프의 x축 간격 (1시간)
	double minutes = 1;       // 사용자 입력 y값 (중앙값의 y값)
	double yScaling = 1;      // y축 스케일링 값 (2.3)
	int maxPeople = 0;		//파일에서 출력되는 사람의 수
	float floatingIPoint = 0.000001; //부동소수점 보강
	float probabilityValue = 1; //적용된 확률
	bool isMaxProbabilityValue = false;

	// 정규분포 함수
	double NormalDistribution(double x, double mean, double stddev);

	// 표준편차 계산 함수 (입력된 중앙값의 y값에 대해 σ를 계산, yScaling 고려)
	double CalculateStandardDeviation(double y, double yScaling);

	// 정규분포 누적분포 함수(CDF)를 계산하는 함수
	double NormalCDF(double x, double mean, double stddev);
	
	// 시간을 문자열로 변환하는 함수 (소수점을 시간, 분으로 변환)
	std::wstring TimeToWString(double time);
	std::string TimeToString(double time);

	//ReadFiles rf;

	//이제부턴 파일로 사용하는 곳
	std::ifstream od;
	std::ifstream xy;
	std::ofstream out;

	double trip[6 + 1][6 + 1];
	double x_coordinate[6 + 1];
	double y_coordinate[6 + 1];

	long   pop_id = 0;

	bool InitReadFile();
	void MakeOutputFile();
	void MakeTxtFile(int i, int j, int number, std::string time);
	void CloseOutputFile();
	void WhiteOutputFile(HDC hdc);
	void CloseFiles();
	void DrawBarGraph(HDC hdc, double x, double y, COLORREF color);
	bool FileExists(const std::string& name);

	//엑셀을 위한 코드
	std::vector<std::tuple<int, std::string , int, int>> data;
};