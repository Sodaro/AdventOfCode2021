#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}

class MapPoint
{
public:
    MapPoint(int p_height)
    {
        height = p_height;
    }
    void AddNeighbor(MapPoint* point)
    {
        neighbors.push_back(point);
    }
    int height = 0;
    std::vector<MapPoint*> neighbors;
};

void solvePart1(std::vector<MapPoint*> &data)
{
    std::vector<int> lowestPoints;
    for (int i = 0; i < data.size(); i++)
    {
        bool isLowest = true;
        for (int j = 0; j < data[i]->neighbors.size(); j++)
        {
            int height1 = data[i]->height;
            int height2 = data[i]->neighbors[j]->height;
            if (height1 >= height2)
            {
                isLowest = false;
                break;
            }
        }
        if (isLowest)
        {
            lowestPoints.push_back(data[i]->height);
        }
    }

    int sum = 0;
    for (int i = 0; i < lowestPoints.size(); i++)
        sum += lowestPoints[i] + 1;

    writeValueToFile("output1.txt", sum);
}

void solvePart2(std::vector<std::vector<int>>& data)
{
    //map out the 9's, get the length between them
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size(); j++)
        {



        }
    }
}

int main()
{
    std::ifstream input("test2.txt");
    std::string line;
    std::vector<MapPoint*> points;
    int rowCount = 0, lineLength = 0;
    while (std::getline(input, line))
    {
        lineLength = line.length();
        for (int i = 0; i < lineLength; i++)
        {
            int height = line[i] - '0';
            points.push_back(new MapPoint(height));
        }
        rowCount++;
    }

    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < lineLength; j++)
        {
            MapPoint* point = points[i * lineLength + j];
            if (i != 0)
            {
                //top row has no above neighbor
                point->AddNeighbor(points[(i-1) * lineLength + j]);
            }
            if (i != rowCount - 1)
            {
                //bottom row has no below neighbor
                point->AddNeighbor(points[(i + 1) * lineLength + j]);
            }
            if (j != 0)
            {
                //leftmost col has no left neighbor
                point->AddNeighbor(points[i * lineLength + (j - 1)]);
            }
            if (j != lineLength - 1)
            {
                //rightmost col has no right neighbor
                point->AddNeighbor(points[i * lineLength + (j + 1)]);
            }
        }
    }
    //solvePart1(points);
    std::cout << "beans" << std::endl;

        //for (int j = 0; j < data[i].size(); j++)
        //{
}
