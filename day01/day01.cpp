// day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int GetSumOfRange(std::vector<int> values, int startIndex, int endIndex)
{
    int sum = 0;
    for (int i = startIndex; i <= endIndex; i++)
        sum += values[i];

    return sum;
}
void SolvePart1(std::string input, std::string output)
{
    std::string line;
    std::ifstream inputFile;
    std::ofstream outputFile;
    int lastDepthValue = 0;
    int increasedDepthCount = 0;
    inputFile.open(input);
    if (!inputFile.is_open())
        return;

    while (std::getline(inputFile, line))
    {
        int depthValue = std::stoi(line);
        if (lastDepthValue != 0 && depthValue > lastDepthValue)
            increasedDepthCount++;

        lastDepthValue = depthValue;
    }
    inputFile.close();
    outputFile.open(output);
    outputFile << std::to_string(increasedDepthCount) << std::endl;
}

void SolvePart2(std::string input, std::string output)
{
    std::string line;
    std::ifstream inputFile;
    std::ofstream outputFile;
    inputFile.open(input);
    if (!inputFile.is_open())
        return;

    std::vector<int> depthValues;
    int increasedDepthCount = 0;
    while (std::getline(inputFile, line))
    {
        //add number to end of vector
        depthValues.push_back(std::stoi(line));
        if (depthValues.size() == 4)
        {
            //every 4 numbers compare the first 3 numbers with the second 3 numbers, and then remove the first number
            int a = GetSumOfRange(depthValues, 0, 2);
            int b = GetSumOfRange(depthValues, 1, 3);
            if (b > a)
                increasedDepthCount++;

            depthValues.erase(depthValues.begin());
        }
    }
    inputFile.close();
    outputFile.open(output);
    outputFile << std::to_string(increasedDepthCount) << std::endl;
}

int main()
{
    SolvePart1("input.txt", "output1.txt");
    SolvePart2("input.txt", "output2.txt");
}

