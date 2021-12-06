#include <iostream>
#include <fstream>
#include <string>

void writeValueToFile(std::string path, uint64_t value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}

void leftRotate(uint64_t arr[], int n)
{
    uint64_t temp = arr[0];
    for (int i = 0; i < n-1; i++)
        arr[i] = arr[i+1];
    arr[n - 1] = temp;
}

uint64_t getPopulation(int days, uint64_t(&fishSchools)[9])
{
    for (int i = 0; i < days; i++)
    {
        leftRotate(fishSchools, 9);
        fishSchools[6] += fishSchools[8];
    }
    uint64_t totalFish = 0;
    for (int i = 0; i < 9; i++)
    {
        totalFish += fishSchools[i];
    }
    return totalFish;
}

int main()
{
    std::ifstream file("input.txt");
    std::string line;
    std::getline(file, line);
    //days: 0-8
    //make groups of fish for each day, add new fish to group 8 and old fish to group 6
    uint64_t fishSchools[9] = { 0 };
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
            continue;

        int dayVal = line[i] - '0';
        fishSchools[dayVal]++;
    }
    uint64_t part1 = getPopulation(80, fishSchools);
    uint64_t part2 = getPopulation(256, fishSchools);

    writeValueToFile("output1.txt", part1);
    writeValueToFile("output2.txt", part2);
}
