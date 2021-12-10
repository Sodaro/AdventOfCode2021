#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum class Direction
{
    North = 0,
    South = 1,
    West = 2,
    East = 3
};

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
    MapPoint* getNeighbor(Direction dir)
    {
        return neighbors[static_cast<int>(dir)];
    }
    void AddNeighbor(MapPoint* point, Direction dir)
    {
        neighbors[static_cast<int>(dir)] = point;
    }
    int height = 0;
    MapPoint* neighbors[4] = { nullptr };
};

void solvePart1(std::vector<MapPoint*> &data)
{
    std::vector<int> lowestPoints;
    for (int i = 0; i < data.size(); i++)
    {
        bool isLowest = true;
        for (int j = 0; j < 4; j++)
        {
            if (data[i]->neighbors[j] == nullptr)
                continue;

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

void flood_fill(MapPoint* point, int &size)
{
    if (point == nullptr || point->height == 9 || point->height == -1)
        return;                                             

    point->height = -1; // mark the point so that I know if I passed through it. 
    size++;
    flood_fill(point->getNeighbor(Direction::North), size);
    flood_fill(point->getNeighbor(Direction::South), size);
    flood_fill(point->getNeighbor(Direction::West), size);
    flood_fill(point->getNeighbor(Direction::East), size);
}

void solvePart2(std::vector<MapPoint*> &data, int rowCount, int lineLength)
{
    //map out the 9's, get the length between them
    std::vector<int> basinSizes;
    for (int i = 0; i < data.size(); i++)
    {
        int basinSize = 0;
        flood_fill(data[i], basinSize);
        if (basinSize == 0)
            continue;
        basinSizes.push_back(basinSize);
    }

    std::sort(basinSizes.begin(), basinSizes.end(), std::greater<>());

    int multipliedSizes = basinSizes[0] * basinSizes[1] * basinSizes[2];
    std::cout << multipliedSizes << std::endl;

}

int main()
{
    std::ifstream input("input.txt");
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
                point->AddNeighbor(points[(i-1) * lineLength + j], Direction::North);
            }
            if (i != rowCount - 1)
            {
                //bottom row has no below neighbor
                point->AddNeighbor(points[(i + 1) * lineLength + j], Direction::South);
            }
            if (j != 0)
            {
                //leftmost col has no left neighbor
                point->AddNeighbor(points[i * lineLength + (j - 1)], Direction::West);
            }
            if (j != lineLength - 1)
            {
                //rightmost col has no right neighbor
                point->AddNeighbor(points[i * lineLength + (j + 1)], Direction::East);
            }
        }
    }
    //solvePart1(points);

    solvePart2(points, rowCount, lineLength);
    std::cout << "beans" << std::endl;

        //for (int j = 0; j < data[i].size(); j++)
        //{
}
