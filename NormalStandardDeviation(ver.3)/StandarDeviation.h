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
	double mean = 7;    // �߾Ӱ� (7�� 30���� �ǹ�)
	double stddev = 0.25;        // ���� ǥ�������� �� ����
	int step = 400;       // �׷��� �ε巴�� �׸��� ���� ����ȭ
	double xMin = 6.5;    // ù ��° �׷����� x�� ���� (4�� 30��)
	double xMax = 7.5;   // ù ��° �׷����� x�� ���� (10�� 30��)
	double xStep = 1.0;   // ù ��° �׷����� x�� ���� (1�ð�)
	double minutes = 1;       // ����� �Է� y�� (�߾Ӱ��� y��)
	double yScaling = 1;      // y�� �����ϸ� �� (2.3)
	int maxPeople = 0;		//���Ͽ��� ��µǴ� ����� ��
	float floatingIPoint = 0.000001; //�ε��Ҽ��� ����
	float probabilityValue = 1; //����� Ȯ��
	bool isMaxProbabilityValue = false;

	// ���Ժ��� �Լ�
	double NormalDistribution(double x, double mean, double stddev);

	// ǥ������ ��� �Լ� (�Էµ� �߾Ӱ��� y���� ���� �� ���, yScaling ���)
	double CalculateStandardDeviation(double y, double yScaling);

	// ���Ժ��� �������� �Լ�(CDF)�� ����ϴ� �Լ�
	double NormalCDF(double x, double mean, double stddev);
	
	// �ð��� ���ڿ��� ��ȯ�ϴ� �Լ� (�Ҽ����� �ð�, ������ ��ȯ)
	std::wstring TimeToWString(double time);
	std::string TimeToString(double time);

	//ReadFiles rf;

	//�������� ���Ϸ� ����ϴ� ��
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

	//������ ���� �ڵ�
	std::vector<std::tuple<int, std::string , int, int>> data;
};