#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void writeValueToFile(std::string path, uint64_t value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}


int nthTriangleNumber(int n)
{
    return n * (n + 1) / 2;
}

int recursiveSumSteps(int n)
{
    if (n == 1)
        return 1;
    
    return n + recursiveSumSteps(n - 1);
}

int solvePart1(std::vector<int>& numbers)
{
    std::vector<int> fuelCosts;
    int minPos, maxPos, fuelCost = 0;
    minPos = numbers.front();
    maxPos = numbers.back();
    for (int i = minPos; i < maxPos; i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {
            fuelCost += abs(numbers[j] - i);
        }
        fuelCosts.push_back(fuelCost);
        fuelCost = 0;
    }
    std::sort(fuelCosts.begin(), fuelCosts.end());
    return fuelCosts.front();
}

int solvePart2(std::vector<int>& numbers)
{
    std::vector<int> fuelCosts;
    int minPos, maxPos, fuelCost = 0;
    minPos = numbers.front();
    maxPos = numbers.back();
    for (int i = minPos; i < maxPos; i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {
            int distance = abs(numbers[j] - i);
            if (distance == 0)
                continue;
            fuelCost += nthTriangleNumber(distance);
        }
        fuelCosts.push_back(fuelCost);
        fuelCost = 0;
    }
    std::sort(fuelCosts.begin(), fuelCosts.end());
    return fuelCosts.front();
}

int main()
{
    std::ifstream file("input.txt");
    std::string line;
    std::getline(file, line);
    std::vector<int> numbers;
    std::string numberString;
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
        {
            numbers.push_back(std::stoi(numberString));
            numberString = "";
        }
        else
            numberString += line[i];   
    }
    numbers.push_back(std::stoi(numberString));
    std::sort(numbers.begin(), numbers.end());

    int part1 = solvePart1(numbers);
    int part2 = solvePart2(numbers);
    writeValueToFile("output1.txt", part1);
    writeValueToFile("output2.txt", part2);
}
