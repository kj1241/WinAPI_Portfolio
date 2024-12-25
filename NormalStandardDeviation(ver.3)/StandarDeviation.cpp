#include "stdafx.h"
#include "StandarDeviation.h"


StandarDeviation::StandarDeviation()
{
}

StandarDeviation::~StandarDeviation()
{
}

void StandarDeviation::Calc(HWND hwnd)
{
    MakeOutputFile();

    //���� ����.
    data.clear();

    HDC hdc = GetDC(hwnd);

    // 1. ����� �Է� y���� �������� ǥ������ ��� (yScaling�� ������ ����)
    //stddev = CalculateStandardDeviation(inputY, yScaling);  // �߾Ӱ������� y���� �������� ǥ������ ���

    // ������ �� ����
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, hPen);

    // 2. �׷����� �ִ� y���� ���
    double maxYValue = NormalDistribution(mean, mean, stddev) * yScaling;  // y�� �����ϸ� ������ �ִ� y��

    // 3. �׷����� �ִ� y���� ȭ���� �Ѵ��� Ȯ���ϰ�, ���� ��쿡�� ��� ���� ����
    double yScalingFactor = 1.0;
    if (maxYValue * 400 > HEIGHT - 200) {  // �׷��� ���̰� ȭ���� ������ ���
        yScalingFactor = (HEIGHT - 200) / (maxYValue * 400);  // ��� ���� ���
    }

    // 4. ���Ժ��� �׷��� �׸��� (x��: 4�� 30�� ~ 10�� 30��)
    double stepSize = (double)(xMax - xMin) / step;
    bool firstPoint = true;

    // ù ��° �׷����� ���Ժ��� � �׸���
    for (int i = 0; i <= step; ++i) {
        double x = xMin + i * stepSize;
        double y = NormalDistribution(x, mean, stddev) * yScaling;  // y�� �����ϸ� ����
        y *= yScalingFactor;  // �ʿ� �� y�� �� ��� ����

        int pixelX = 100 + static_cast<int>((x - xMin) * (WIDTH - 200) / (xMax - xMin)); // ù ��° �׷����� x ��ǥ
        int pixelY = HEIGHT - 100 - static_cast<int>(y * 400); // y�� ��ȯ �� ���

        if (firstPoint) {
            MoveToEx(hdc, pixelX, pixelY, NULL);
            firstPoint = false;
        }
        else {
            LineTo(hdc, pixelX, pixelY); // ù ��° �׷����� � �׸���
        }
    }

    // X�࿡�� 1�ð� ���ݸ��� ���� �� y�� ǥ��
    HPEN hDashedPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0)); // ������ ���� ��
    SelectObject(hdc, hDashedPen);

    for (double i = xMin; i <= xMax; i += xStep) {
        double originalYValue = NormalDistribution(i, mean, stddev) * yScaling;
        double yIntersection = originalYValue * yScalingFactor; // y�� �����ϸ� �� ��� ����

        int xPixel = 100 + static_cast<int>((i - xMin) * (WIDTH - 200) / (xMax - xMin));
        int yPixel = HEIGHT - 100 - static_cast<int>(yIntersection * 400); // y�� ��ȯ �� ���

        // ���� �׸��� (x�࿡�� �׷�������)
        MoveToEx(hdc, xPixel, HEIGHT - 100, NULL);
        LineTo(hdc, xPixel, yPixel);

        // �������� �ؽ�Ʈ�� �ð� �� y�� ǥ��
        std::wstring timeLabel = TimeToWString(i);
        std::wstringstream yLabel;
        yLabel << L"y = " << originalYValue;
        TextOut(hdc, xPixel + 10, HEIGHT - 90, timeLabel.c_str(), timeLabel.length());
        TextOut(hdc, xPixel + 10, yPixel - 10, yLabel.str().c_str(), yLabel.str().length());  // y�� ǥ��
    }

    // 5. ���� ǥ������ �� y�� �����ϸ� �� ���
    if (isMaxProbabilityValue)
        probabilityValue = 1;
    else
    {
        double prob_x1 = NormalCDF(xMin, mean, stddev);
        double prob_x2 = NormalCDF(xMax, mean, stddev);
        probabilityValue = prob_x2 - prob_x1; // ���� [x1, x2]�� ���� Ȯ��
    }


    std::wstringstream ss;
    ss << "Applied Probability Value: "<< probabilityValue << L", Calculated Std Dev: " << stddev << L", Y-Scaling: " << yScaling;
    TextOut(hdc, WIDTH - 500, 50, ss.str().c_str(), ss.str().length()); // ������ ���� ǥ������ �� y�� �����ϸ� ���


    //6. ���� ���Ϸ� ����ؾ� �Ǵ°��� ����(���߿� ���� �ڵ� ����ȭ �ʿ�)
    WhiteOutputFile(hdc);
    CloseOutputFile();

    // ���ҽ� ����
    DeleteObject(hPen);
    DeleteObject(hDashedPen);
}

bool StandarDeviation::InitStandarDeviation()
{
    return InitReadFile();
}

void StandarDeviation::CloseStandarDeviation()
{
    CloseFiles();
}

void StandarDeviation::MakeExcle()
{
    std::string name = "pop" + std::to_string(pop_id-1) + ".csv";
    std::ofstream file(name);

    if (file.is_open()) {
        file << "number, time, i, j\n";

        std::for_each(data.begin(), data.end(), [&](const auto& t)
            {
                file << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << ", " << std::get<3>(t) << std::endl;
            });
    }
    file.close();
}
double StandarDeviation::NormalDistribution(double x, double mean, double stddev)
{
	return (1.0 / (stddev * sqrt(2.0 * M_PI))) * exp(-0.5 * pow((x - mean) / stddev, 2));
}

double StandarDeviation::CalculateStandardDeviation(double y, double yScaling)
{
	return 1.0 / ((y / yScaling) * sqrt(2.0 * M_PI));  // yScaling�� �����Ͽ� ǥ������ ���
}

// ���Ժ��� �������� �Լ�(CDF)�� ����ϴ� �Լ�
double StandarDeviation::NormalCDF(double x, double mean, double stddev) {
    return 0.5 * (1 + erf((x - mean) / (stddev * sqrt(2.0))));
}

std::wstring StandarDeviation::TimeToWString(double time)
{
    int hour = static_cast<int>(time);
    int minute = static_cast<int>((time - hour) * 60);
    std::wstringstream ss;
    ss << hour << L"�� " << minute << L"��";
    return ss.str();
}

std::string StandarDeviation::TimeToString(double time)
{
    int hour = static_cast<int>(time); // �ð� ���
    int minute = static_cast<int>((time - hour) * 60); // �� ���
    double fractionalMinute = (time - hour) * 60 - minute; // ���� �Ҽ��� �κ� ���
    int second = static_cast<int>(fractionalMinute * 60); // �� ���

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << minute << ":"
        << std::setw(2) << std::setfill('0') << second;

    return ss.str();
}

bool StandarDeviation::InitReadFile()
{
    od.open("od_input.txt", std::ios::in);
    xy.open("xy_input.txt", std::ios::in);

    long i, j;
    double o2, x, y;

    if (!od.is_open()) {
        return false;
    }
    if (!xy.is_open()) {
        return false;
    }

    //od ������ �о� trip �ʿ� ����.
    while (od >> i >> j >> o2) {
        trip[i][j] = o2;
        maxPeople += o2;
    }

    // xy ������ �о� x_coordinate�� y_coordinate �ʿ� ����.
    while (xy >> i >> x >> y) {
        x_coordinate[i] = x;
        y_coordinate[i] = y;
    }
    return true;
}

void StandarDeviation::MakeOutputFile()
{
    std::string name;
    do {
        name = "pop" + std::to_string(pop_id) + ".out";
        pop_id++;
    } while (FileExists(name));  // ������ �����ϴ��� Ȯ��
    out.open(name, std::ios::out);
}

void StandarDeviation::MakeTxtFile(int i, int j, int number, std::string time)
{
    out << std::fixed << std::setprecision(6);
    out << "<!-- ====================================================================== -->\n";
    out << "    <person id=\"" << number << "\" sex=\"m\" age=\"41\" car_avail=\"always\" employed=\"yes\">\n";
    out << "        <plan selected=\"yes\">\n";
    out << "            <act type=\"home\" x=\"" << x_coordinate[i] << "\" y=\"" << y_coordinate[i] << "\" end_time=\"" << time << "\"/>\n";
    out << "            <leg mode=\"pt\">\n";
    out << "            </leg>\n";
    out << "            <act type=\"work\" x=\"" << x_coordinate[j] << "\" y=\"" << y_coordinate[j] << "\"  start_time=\"09:00:00\" end_time=\"18:00:00\"/>\n";
    out << "            <leg mode=\"pt\">\n";
    out << "            </leg>\n";
    out << "            <act type=\"home\" x=\"" << x_coordinate[i] << "\" y=\"" << y_coordinate[i] << "\"/>\n";
    out << "        </plan>\n";
    out << "    </person>\n";
    out << std::defaultfloat << std::setprecision(6);
}

void StandarDeviation::CloseOutputFile()
{
    if (out.is_open())
    {
        out.close();
    }
}

void StandarDeviation::WhiteOutputFile(HDC hdc)
{
    int totalPeople = 0;
    // trip �迭�� ��ȸ�ϸ� XML ����
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 6; ++j) {
            if (trip[i][j] > 0) {
                int maxPerson = 0;

                for (int k = 0; k <= step / 2; ++k)
                {
                    if (k == 0) //��� ������
                    {
                        double stepSize = (double)(minutes / 60);
                        double x = mean;
                        //double y = NormalDistribution(x, mean, stddev) * yScaling;  // y�� �����ϸ� ����
                        double prob_x1 = NormalCDF(x - stepSize / 2, mean, stddev);
                        double prob_x2 = NormalCDF(x + stepSize / 2, mean, stddev);
                        double y = prob_x2 - prob_x1; // ���� [x1, x2]�� ���� Ȯ��

                        int person = floor(y * trip[i][j] / probabilityValue);

                        if (person == 0 && trip[i][j] > maxPerson) // ����� ������ �ȉ�µ� ���� ��ü������ ������ ������
                            person = 1;
                        std::string time = TimeToString(x);


                        DrawBarGraph(hdc, x, y, RGB(0, 0, 255));

                        for (int l = 0; l < person; ++l)
                        {
                            maxPerson++;
                            totalPeople++;
                            MakeTxtFile(i, j, totalPeople, time);
                            data.push_back(std::make_tuple(totalPeople, time, i, j));
                        }
                    }
                    else { //�翷�� �����ϴ�.


                        if (trip[i][j] <= maxPerson) //�� �̻��ϸ� �ȵȴ�.
                            break;

                        double stepSize = (double)(minutes / 60);
                        double x1 = mean + k * stepSize + floatingIPoint;
                        double x2 = mean - k * stepSize + floatingIPoint;
                        //double y = NormalDistribution(x1, mean, stddev) * yScaling;

                        double prob_x1 = NormalCDF(x1 - stepSize / 2, mean, stddev);
                        double prob_x2 = NormalCDF(x1 + stepSize / 2, mean, stddev);
                        double y = prob_x2 - prob_x1; // ���� [x1, x2]�� ���� Ȯ��
           
                        int person = floor(y * trip[i][j] / probabilityValue);

                        if (person == 0 && trip[i][j] > maxPerson) // ����� ������ �ȉ�µ� ���� ��ü������ ������ ������
                            person = 1;

                        std::string time1 = TimeToString(x1);
                        std::string time2 = TimeToString(x2);

                        DrawBarGraph(hdc, x1, y, RGB(0, 255, 0));
                        DrawBarGraph(hdc, x2, y, RGB(0, 255, 255));
                        for (int l = 0; l < person; ++l)
                        {
                            if (trip[i][j] <= maxPerson) //�� �̻��ϸ� �ȵȴ�.
                                break;

                            if (trip[i][j] - 1 > maxPerson)
                            {
                                if (isMaxProbabilityValue)
                                {
                                    maxPerson += 2;
                                    totalPeople++;
                                    MakeTxtFile(i, j, totalPeople, time1);
                                    data.push_back(std::make_tuple(totalPeople, time1, i, j));


                                    totalPeople++;
                                    MakeTxtFile(i, j, totalPeople, time2);
                                    data.push_back(std::make_tuple(totalPeople, time2, i, j));
                                }
                                else
                                {
                                    if (xMax >= x1)
                                    {
                                        maxPerson++;
                                        totalPeople++;
                                        MakeTxtFile(i, j, totalPeople, time1);
                                        data.push_back(std::make_tuple(totalPeople, time1, i, j));
                                    }
                                    if (xMin <= x2)
                                    {
                                        maxPerson++;
                                        totalPeople++;
                                        MakeTxtFile(i, j, totalPeople, time2);
                                        data.push_back(std::make_tuple(totalPeople, time2, i, j));
                                    }
                                }

                            }
                            else if (trip[i][j] > maxPerson)
                            {
                                if (isMaxProbabilityValue)
                                {
                                    maxPerson++;
                                    totalPeople++;
                                    MakeTxtFile(i, j, totalPeople, time1); //�ʴ� ����� �켱
                                    data.push_back(std::make_tuple(totalPeople, time1, i, j));
                                }
                                else
                                {
                                    if (xMax >= x1)
                                    {
                                        maxPerson++;
                                        totalPeople++;
                                        MakeTxtFile(i, j, totalPeople, time1);
                                        data.push_back(std::make_tuple(totalPeople, time1, i, j));
                                    }
                                    else if (xMin <= x2)
                                    {
                                        maxPerson++;
                                        totalPeople++;
                                        MakeTxtFile(i, j, totalPeople, time2);
                                        data.push_back(std::make_tuple(totalPeople, time2, i, j));
                                    }
                                }
                            }
                            else
                                break;
                        }
                    }
                }
            }
        }
    }
}

void StandarDeviation::CloseFiles()
{
    if (od.is_open())
    {
        od.close();
    }
    if (xy.is_open())
    {
        xy.close();
    }

}

// �׷��� �׸��� �Լ� �߰�
void StandarDeviation::DrawBarGraph(HDC hdc, double x, double y, COLORREF color) {

    HBRUSH hBrush = CreateSolidBrush(color); // �Ķ���

    //���� �׸���
    int barHeight = y * 200; // y���� �����ϸ��ؼ� ���� ���� ����
    int barWidth = 20;  // ���� �ʺ� ����

    int pixelX = 100 + static_cast<int>((x - xMin) * (WIDTH - 200) / (xMax - xMin)); // ���� x ��ġ
    int pixelY = HEIGHT - 100 - barHeight; // ���� y ��ġ

    // ���� �׸��� (pixelX, pixelY�� ��������)
    Rectangle(hdc, pixelX, pixelY, pixelX + barWidth, HEIGHT - 100); // ���� ���̸�ŭ �׸���

    // �簢�� ���� ����
    RECT rect;
    rect.left = pixelX;
    rect.right = pixelX + barWidth;
    rect.top = pixelY;
    rect.bottom = HEIGHT - 100;

    // �Ķ������� ���� ä���
    FillRect(hdc, &rect, hBrush); // �Ķ��� �귯�÷� ä���� ���� �׸���
    DeleteObject(hBrush);
}

bool StandarDeviation::FileExists(const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}