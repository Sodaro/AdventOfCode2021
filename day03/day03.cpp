#include <fstream>
#include <string>
#include <bitset>
#include <vector>

const int NR_OF_BITS = 12;

void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}

void countBitsInPosition(int (&onesArr)[NR_OF_BITS], int (&zeroesArr)[NR_OF_BITS], std::vector<std::bitset<NR_OF_BITS>>& bitsets)
{
    for (auto bitset : bitsets)
    {
        for (int i = 0; i < bitset.size(); i++)
        {
            bitset[i] == 0 ? zeroesArr[i]++ : onesArr[i]++;
        }
    }
}

std::bitset<NR_OF_BITS> getMostCommonBits(std::vector<std::bitset<NR_OF_BITS>>& inputnumbers)
{
    //count the bits and return a bitset with the most common bits
    std::bitset<NR_OF_BITS> retVal;
    int onesArr[NR_OF_BITS] = {0}; int zeroesArr[NR_OF_BITS] = {0};
    countBitsInPosition(onesArr, zeroesArr, inputnumbers);
    for (int i = 0; i < NR_OF_BITS; i++)
    {
        retVal[i] = onesArr[i] >= zeroesArr[i];
    }
    return retVal;
}

std::bitset<NR_OF_BITS> getLeastCommonBits(std::vector<std::bitset<NR_OF_BITS>>& inputnumbers)
{
    //count the bits and return a bitset with the least common bits
    std::bitset<NR_OF_BITS> retVal;
    int onesArr[NR_OF_BITS] = { 0 }; int zeroesArr[NR_OF_BITS] = { 0 };
    countBitsInPosition(onesArr, zeroesArr, inputnumbers);
    for (int i = 0; i < NR_OF_BITS; i++)
    {
        retVal[i] = (onesArr[i] < zeroesArr[i]);
    }
    return retVal;
}

void solvePart1(std::vector<std::bitset<NR_OF_BITS>>& inputnumbers, std::string outputPath)
{
    std::bitset<NR_OF_BITS> gamma = getMostCommonBits(inputnumbers);
    std::bitset<NR_OF_BITS> epsilon = getLeastCommonBits(inputnumbers);
    int powerConsumption = (int)(gamma.to_ulong() * epsilon.to_ulong());
    writeValueToFile(outputPath, powerConsumption);
}

void solvePart2(std::vector<std::bitset<NR_OF_BITS>> &bitsets, std::string outputPath)
{
    int O2 = 0, CO2 = 0, lifeSupport = 0;
    std::vector<std::bitset<NR_OF_BITS>> bitsetsCopy;

    for (auto bits : bitsets)
        bitsetsCopy.push_back(bits);

    for (int i = NR_OF_BITS - 1; i >= 0; i--)
    {
        std::bitset<NR_OF_BITS> O2Criteria = getMostCommonBits(bitsetsCopy);
        bool bitCriteria = O2Criteria[i];
        for (int j = bitsetsCopy.size() - 1; j >= 0; j--)
        {
            if (bitsetsCopy[j][i] != bitCriteria)
                bitsetsCopy.erase(bitsetsCopy.begin()+j);
        }
        if (bitsetsCopy.size() == 1)
            break;
    }

    O2 = (int)bitsetsCopy[0].to_ulong();

    for (int i = NR_OF_BITS - 1; i >= 0; i--)
    {
        std::bitset<NR_OF_BITS> CO2Criteria = getLeastCommonBits(bitsets);
        bool bitCriteria = CO2Criteria[i];
        for (int j = bitsets.size() - 1; j >= 0; j--)
        {
            if (bitsets[j][i] != bitCriteria)
                bitsets.erase(bitsets.begin() + j);
        }
        if (bitsets.size() == 1)
            break;
    }
    CO2 = (int)bitsets[0].to_ulong();

    lifeSupport = O2 * CO2;
    writeValueToFile("output2.txt", lifeSupport);
}

int main()
{
    std::vector<std::bitset<NR_OF_BITS>> bitsets;
    std::ifstream input;
    input.open("input.txt");
    std::string line;

    while (std::getline(input, line))
        bitsets.push_back(std::bitset<NR_OF_BITS>(line));

    input.close();
    
    solvePart1(bitsets, "output1.txt");
    solvePart2(bitsets, "output2.txt");
}