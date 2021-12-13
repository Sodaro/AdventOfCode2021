#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>


void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}
void writeToFile(std::string path, std::string value)
{
    std::ofstream output;
    output.open(path);
    output << value << std::endl;
    output.close();
}

struct Point
{
public:
    Point(int p_x, int p_y)
    {
        x = p_x;
        y = p_y;
    }
    int x = 0;
    int y = 0;
};

struct Instruction
{
public:
    Instruction(char axis, int axisVal)
    {
        axisToFold = axis;
        axisValue = axisVal;
    }
    char axisToFold;
    int axisValue;
};

void printPoints(int max_x, int max_y, std::vector<Point*> &points)
{
    std::string output = "";
    std::string line;
    for (int i = 0; i <= max_y; i++)
    {
        for (int j = 0; j <= max_x; j++)
        {
            bool pointExists = false;
            for (Point* point : points)
            {
                if (point->x == j && point->y == i)
                {
                    pointExists = true;
                    break;
                }
            }
            if (pointExists)
                line += "#";
            else
                line += ".";
        }
        line += "\n";
        output += line;
        line = "";
    }
    std::cout << output << std::endl;
    writeToFile("output2.txt", output);
}

void foldPoints(int &max_x, int &max_y, int n, std::vector<Point*> &points, std::queue<Instruction*> &instructions)
{
    while (n > 0)
    {
        Instruction* instruction = instructions.front();
        instructions.pop();
        n--;
        if (instruction->axisToFold == 'y')
        {
            int val = instruction->axisValue;
            for (int i = points.size() - 1; i >= 0; i--)
            {
                int distance = points[i]->y - val;
                if (distance > 0)
                {
                    points[i]->y = val - distance;
                }
                
            }
            max_y = val;
        }
        else
        {
            int val = instruction->axisValue;
            for (int i = points.size() - 1; i >= 0; i--)
            {
                int distance = points[i]->x - val;
                if (distance > 0)
                {
                    points[i]->x = val - distance;
                }
            }
            max_x = val;
        }
        
    }
}


void part1(std::vector<Point*> &points)
{
    std::vector<Point*> uniquePositions;

    for (Point* point1 : points)
    {
        bool isUnique = true;
        for (Point* point2 : uniquePositions)
        {
            if (point1->x == point2->x && point1->y == point2->y)
            {
                isUnique = false;
                break;
            }
        }
        if (isUnique)
        {
            uniquePositions.push_back(point1);
        }
    }
    std::cout << uniquePositions.size() << std::endl;
    writeValueToFile("output1.txt", uniquePositions.size());
}

int main()
{
    std::ifstream input("input.txt");
    std::string line;
    char delimiters[2] = { ',', '=' };
    int max_x = 0, max_y = 0;
    std::vector<Point*> points;
    std::queue<Instruction*> instructions;
    while (std::getline(input, line))
    {
        if (line == "")
            continue;
        int findpos = line.find(delimiters[0]);
        if (findpos == -1)
        {
            line = line.replace(0,11,"");
            int findpos = line.find(delimiters[1]);
            int value = std::stoi(line.substr(findpos + 1, line.length()));
            Instruction* instruction = new Instruction(line.substr(0, findpos)[0], value);
            instructions.push(instruction);
        }
        else
        {
            int x = std::stoi(line.substr(0, findpos));
            int y = std::stoi(line.substr(findpos + 1, line.length()));
            Point* point = new Point(x, y);
            points.push_back(point);
            if (x > max_x)
                max_x = x;
            if (y > max_y)
                max_y = y;
        }
    }
    foldPoints(max_x, max_y, 1, points, instructions);
    part1(points);
    foldPoints(max_x, max_y, instructions.size(), points, instructions);
    printPoints(max_x, max_y, points);
}
