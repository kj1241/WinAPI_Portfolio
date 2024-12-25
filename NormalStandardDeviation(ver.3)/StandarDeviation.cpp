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

    //백터 비우기.
    data.clear();

    HDC hdc = GetDC(hwnd);

    // 1. 사용자 입력 y값을 기준으로 표준편차 계산 (yScaling을 나눠서 적용)
    //stddev = CalculateStandardDeviation(inputY, yScaling);  // 중앙값에서의 y값을 기준으로 표준편차 계산

    // 검정색 펜 선택
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, hPen);

    // 2. 그래프의 최대 y값을 계산
    double maxYValue = NormalDistribution(mean, mean, stddev) * yScaling;  // y축 스케일링 적용한 최대 y값

    // 3. 그래프의 최대 y값이 화면을 넘는지 확인하고, 넘을 경우에만 축소 비율 적용
    double yScalingFactor = 1.0;
    if (maxYValue * 400 > HEIGHT - 200) {  // 그래프 높이가 화면을 넘으면 축소
        yScalingFactor = (HEIGHT - 200) / (maxYValue * 400);  // 축소 비율 계산
    }

    // 4. 정규분포 그래프 그리기 (x축: 4시 30분 ~ 10시 30분)
    double stepSize = (double)(xMax - xMin) / step;
    bool firstPoint = true;

    // 첫 번째 그래프의 정규분포 곡선 그리기
    for (int i = 0; i <= step; ++i) {
        double x = xMin + i * stepSize;
        double y = NormalDistribution(x, mean, stddev) * yScaling;  // y축 스케일링 적용
        y *= yScalingFactor;  // 필요 시 y축 값 축소 적용

        int pixelX = 100 + static_cast<int>((x - xMin) * (WIDTH - 200) / (xMax - xMin)); // 첫 번째 그래프의 x 좌표
        int pixelY = HEIGHT - 100 - static_cast<int>(y * 400); // y값 변환 후 사용

        if (firstPoint) {
            MoveToEx(hdc, pixelX, pixelY, NULL);
            firstPoint = false;
        }
        else {
            LineTo(hdc, pixelX, pixelY); // 첫 번째 그래프의 곡선 그리기
        }
    }

    // X축에서 1시간 간격마다 점선 및 y값 표시
    HPEN hDashedPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0)); // 빨간색 점선 펜
    SelectObject(hdc, hDashedPen);

    for (double i = xMin; i <= xMax; i += xStep) {
        double originalYValue = NormalDistribution(i, mean, stddev) * yScaling;
        double yIntersection = originalYValue * yScalingFactor; // y축 스케일링 및 축소 적용

        int xPixel = 100 + static_cast<int>((i - xMin) * (WIDTH - 200) / (xMax - xMin));
        int yPixel = HEIGHT - 100 - static_cast<int>(yIntersection * 400); // y값 변환 후 사용

        // 점선 그리기 (x축에서 그래프까지)
        MoveToEx(hdc, xPixel, HEIGHT - 100, NULL);
        LineTo(hdc, xPixel, yPixel);

        // 교차점에 텍스트로 시간 및 y값 표시
        std::wstring timeLabel = TimeToWString(i);
        std::wstringstream yLabel;
        yLabel << L"y = " << originalYValue;
        TextOut(hdc, xPixel + 10, HEIGHT - 90, timeLabel.c_str(), timeLabel.length());
        TextOut(hdc, xPixel + 10, yPixel - 10, yLabel.str().c_str(), yLabel.str().length());  // y값 표시
    }

    // 5. 계산된 표준편차 및 y축 스케일링 값 출력
    if (isMaxProbabilityValue)
        probabilityValue = 1;
    else
    {
        double prob_x1 = NormalCDF(xMin, mean, stddev);
        double prob_x2 = NormalCDF(xMax, mean, stddev);
        probabilityValue = prob_x2 - prob_x1; // 구간 [x1, x2]의 면적 확률
    }


    std::wstringstream ss;
    ss << "Applied Probability Value: "<< probabilityValue << L", Calculated Std Dev: " << stddev << L", Y-Scaling: " << yScaling;
    TextOut(hdc, WIDTH - 500, 50, ss.str().c_str(), ss.str().length()); // 오른쪽 위에 표준편차 및 y축 스케일링 출력


    //6. 이제 파일로 출력해야 되는것을 정리(나중에 좀더 코드 최적화 필요)
    WhiteOutputFile(hdc);
    CloseOutputFile();

    // 리소스 해제
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
	return 1.0 / ((y / yScaling) * sqrt(2.0 * M_PI));  // yScaling을 적용하여 표준편차 계산
}

// 정규분포 누적분포 함수(CDF)를 계산하는 함수
double StandarDeviation::NormalCDF(double x, double mean, double stddev) {
    return 0.5 * (1 + erf((x - mean) / (stddev * sqrt(2.0))));
}

std::wstring StandarDeviation::TimeToWString(double time)
{
    int hour = static_cast<int>(time);
    int minute = static_cast<int>((time - hour) * 60);
    std::wstringstream ss;
    ss << hour << L"시 " << minute << L"분";
    return ss.str();
}

std::string StandarDeviation::TimeToString(double time)
{
    int hour = static_cast<int>(time); // 시간 계산
    int minute = static_cast<int>((time - hour) * 60); // 분 계산
    double fractionalMinute = (time - hour) * 60 - minute; // 분의 소수점 부분 계산
    int second = static_cast<int>(fractionalMinute * 60); // 초 계산

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

    //od 파일을 읽어 trip 맵에 저장.
    while (od >> i >> j >> o2) {
        trip[i][j] = o2;
        maxPeople += o2;
    }

    // xy 파일을 읽어 x_coordinate와 y_coordinate 맵에 저장.
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
    } while (FileExists(name));  // 파일이 존재하는지 확인
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
    // trip 배열을 순회하며 XML 생성
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 6; ++j) {
            if (trip[i][j] > 0) {
                int maxPerson = 0;

                for (int k = 0; k <= step / 2; ++k)
                {
                    if (k == 0) //가운데 지점임
                    {
                        double stepSize = (double)(minutes / 60);
                        double x = mean;
                        //double y = NormalDistribution(x, mean, stddev) * yScaling;  // y축 스케일링 적용
                        double prob_x1 = NormalCDF(x - stepSize / 2, mean, stddev);
                        double prob_x2 = NormalCDF(x + stepSize / 2, mean, stddev);
                        double y = prob_x2 - prob_x1; // 구간 [x1, x2]의 면적 확률

                        int person = floor(y * trip[i][j] / probabilityValue);

                        if (person == 0 && trip[i][j] > maxPerson) // 사람은 배정이 안됬는데 현재 전체수보다 계산수가 적으면
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
                    else { //양옆은 동일하다.


                        if (trip[i][j] <= maxPerson) //더 이상하면 안된다.
                            break;

                        double stepSize = (double)(minutes / 60);
                        double x1 = mean + k * stepSize + floatingIPoint;
                        double x2 = mean - k * stepSize + floatingIPoint;
                        //double y = NormalDistribution(x1, mean, stddev) * yScaling;

                        double prob_x1 = NormalCDF(x1 - stepSize / 2, mean, stddev);
                        double prob_x2 = NormalCDF(x1 + stepSize / 2, mean, stddev);
                        double y = prob_x2 - prob_x1; // 구간 [x1, x2]의 면적 확률
           
                        int person = floor(y * trip[i][j] / probabilityValue);

                        if (person == 0 && trip[i][j] > maxPerson) // 사람은 배정이 안됬는데 현재 전체수보다 계산수가 적으면
                            person = 1;

                        std::string time1 = TimeToString(x1);
                        std::string time2 = TimeToString(x2);

                        DrawBarGraph(hdc, x1, y, RGB(0, 255, 0));
                        DrawBarGraph(hdc, x2, y, RGB(0, 255, 255));
                        for (int l = 0; l < person; ++l)
                        {
                            if (trip[i][j] <= maxPerson) //더 이상하면 안된다.
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
                                    MakeTxtFile(i, j, totalPeople, time1); //늦는 사람이 우선
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

// 그래프 그리기 함수 추가
void StandarDeviation::DrawBarGraph(HDC hdc, double x, double y, COLORREF color) {

    HBRUSH hBrush = CreateSolidBrush(color); // 파란색

    //막대 그리기
    int barHeight = y * 200; // y값을 스케일링해서 막대 높이 설정
    int barWidth = 20;  // 막대 너비 설정

    int pixelX = 100 + static_cast<int>((x - xMin) * (WIDTH - 200) / (xMax - xMin)); // 막대 x 위치
    int pixelY = HEIGHT - 100 - barHeight; // 막대 y 위치

    // 막대 그리기 (pixelX, pixelY를 기준으로)
    Rectangle(hdc, pixelX, pixelY, pixelX + barWidth, HEIGHT - 100); // 막대 높이만큼 그리기

    // 사각형 영역 지정
    RECT rect;
    rect.left = pixelX;
    rect.right = pixelX + barWidth;
    rect.top = pixelY;
    rect.bottom = HEIGHT - 100;

    // 파란색으로 막대 채우기
    FillRect(hdc, &rect, hBrush); // 파란색 브러시로 채워진 막대 그리기
    DeleteObject(hBrush);
}

bool StandarDeviation::FileExists(const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}