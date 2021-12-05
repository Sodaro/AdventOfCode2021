#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#define PI 3.14159265
const int SIDE_LENGTH = 1000;

void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}

struct Point
{
    int x = 0;
    int y = 0;
};
struct Line
{
    Line(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }
    Point p1;
    Point p2;
};

Point parsePoint(std::string part)
{
    std::string numberStr;
    Point point;
    for (int i = 0; i < part.length(); i++)
    {
        if (part[i] == ',')
        {
            point.x = std::stoi(numberStr);
            numberStr = "";
            continue;
        }
        numberStr += part[i];

        if (i == part.length()-1)
        {
            point.y = std::stoi(numberStr);
            numberStr = "";
        }
        
    }
    return point;
}

void mapVerticalAndHorizontalLines(std::vector<Line> &lines, std::vector<int> &grid)
{
        for (int i = 0; i < lines.size(); i++)
        {
            Line line = lines[i];
            if (line.p1.y == line.p2.y)
            {
                int diff = line.p2.x - line.p1.x;
                if (diff > 0)
                {
                    for (int x = line.p1.x; x <= line.p2.x; x++)
                        grid[line.p1.y * SIDE_LENGTH + x]++;
                }
                else
                {
                    for (int x = line.p1.x; x >= line.p2.x; x--)
                        grid[line.p1.y * SIDE_LENGTH + x]++;
                }
            }
            else
            {
                int diff = line.p2.y - line.p1.y;
                if (diff > 0)
                {
                    for (int y = line.p1.y; y <= line.p2.y; y++)
                        grid[line.p1.x + SIDE_LENGTH * y]++;
                }
                else
                {
                    for (int y = line.p1.y; y >= line.p2.y; y--)
                        grid[line.p1.x + SIDE_LENGTH * y]++;
                }
            }
        }
}

void mapDiagonalLines(std::vector<Line>& diagonals, std::vector<int>& grid)
{
    for (Line& line : diagonals)
    {
        int xDiff = line.p2.x - line.p1.x;
        int yDiff = line.p2.y - line.p1.y;

        if (xDiff > 0 && yDiff > 0)
        {
            for (int x = line.p1.x, y = line.p1.y; x <= line.p2.x; x++, y++)
                grid[y * SIDE_LENGTH + x]++;
        }
        else if (xDiff < 0 && yDiff < 0)
        {
            for (int x = line.p1.x, y = line.p1.y; x >= line.p2.x; x--, y--)
                grid[y * SIDE_LENGTH + x]++;
        }
        else if (xDiff > 0 && yDiff < 0)
        {
            for (int x = line.p1.x, y = line.p1.y; x <= line.p2.x; x++, y--)
                grid[y * SIDE_LENGTH + x]++;
        }
        else
        {
            for (int x = line.p1.x, y = line.p1.y; x >= line.p2.x; x--, y++)
                grid[y * SIDE_LENGTH + x]++;
        }
    }

}

void generateLines(std::vector<Line>& horizontalVertical, std::vector<Line>& diagonals)
{
    std::ifstream input("input.txt");
    std::string text;

    while (std::getline(input, text))
    {
        std::string numberStr, part1, part2;
        Point p1;
        Point p2;
        std::string delimiter = " -> ";
        int splitPos = text.find(delimiter);
        part1 = text.substr(0, splitPos);
        part2 = text.substr(splitPos + delimiter.length(), text.length());

        p1 = parsePoint(part1);
        p2 = parsePoint(part2);

        Line line(p1, p2);

        int xDiff = line.p2.x - line.p1.x;
        int yDiff = line.p2.y - line.p1.y;

        if (xDiff != 0 && yDiff != 0)
        {
            int angle = std::atan2(yDiff, xDiff) * 180 / PI;
            if (angle % 45 != 0)
                continue;
            diagonals.push_back(line);
        }
        else
            horizontalVertical.push_back(line);
    }
}

int countDangerAreas(std::vector<int>& grid)
{
    int dangerousAreas = 0;
    for (int i = 0; i < SIDE_LENGTH * SIDE_LENGTH; i++)
    {
        if (grid[i] > 1)
            dangerousAreas++;
    }
    return dangerousAreas;
}

int main()
{
    std::vector<Line> lines;
    std::vector<Line> diagonals;
    std::vector<int> grid(SIDE_LENGTH * SIDE_LENGTH, 0);
    generateLines(lines, diagonals);

    mapVerticalAndHorizontalLines(lines, grid);
    int dangerAreas1 = 0, dangerAreas2 = 0;
    dangerAreas1 = countDangerAreas(grid);
    writeValueToFile("output1.txt", dangerAreas1);

    mapDiagonalLines(diagonals, grid);
    dangerAreas2 = countDangerAreas(grid);
    writeValueToFile("output2.txt", dangerAreas2);
}