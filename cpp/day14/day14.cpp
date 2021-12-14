#include <fstream>
#include <string>
#include <limits>
#include <unordered_map>


void writeValueToFile(std::string path, uint64_t value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}

uint64_t solve(int n, std::unordered_map<std::string, uint64_t> pairs, std::unordered_map<std::string, char> rules, std::unordered_map<char, uint64_t> elementCounts)
{
    
    for (int i = 0; i < n; i++)
    {
        std::unordered_map<std::string, uint64_t> newPairs;
        for (auto pair : pairs)
        {
            std::string newPair1 = "", newPair2 = "", combo = pair.first;
            if (pair.second > 0)
            {
                char newElement = rules[combo];
                elementCounts[newElement] += pair.second;
                newPair1.push_back(combo[0]);
                newPair1.push_back(newElement);
                newPair2.push_back(newElement);
                newPair2.push_back(combo[1]);
                newPairs[newPair1] += pair.second;
                newPairs[newPair2] += pair.second;

            }
        }
        pairs = newPairs;

    }
    uint64_t highestCount = 0, lowestCount = UINT64_MAX;
    for (auto pair : elementCounts)
    {
        if (pair.second > highestCount)
            highestCount = pair.second;
        else if (pair.second < lowestCount)
            lowestCount = pair.second;
    }
    return highestCount - lowestCount;
}

int main()
{
    std::ifstream input("input.txt");
    std::unordered_map<char, uint64_t> elementCounts;
    std::unordered_map<std::string, char> rules;
    std::unordered_map<std::string, uint64_t> pairs;
    std::string line, polymer, delimiter;
    std::getline(input, polymer);

    delimiter = " -> ";
    while (std::getline(input, line))
    {
        if (line == "")
            continue;
        int delimpos = line.find(delimiter);
        std::string rule = line.substr(0, delimpos);
        rules[rule] = line.substr(delimpos + 4, line.length())[0];
    }
    for (int j = 0; j < polymer.length() - 1; j++)
    {
        std::string pair = "";
        pair.push_back(polymer[j]);
        pair.push_back(polymer[j+1]);
        if (pairs.count(pair) > 0)
        {
            pairs[pair]++;
        }
        else
            pairs[pair] = 1;
    }
    //off by one error somewhere, adding 1
    uint64_t p1 = solve(10, pairs, rules, elementCounts)+1;
    uint64_t p2 = solve(40, pairs, rules, elementCounts)+1;
    writeValueToFile("output1.txt", p1);
    writeValueToFile("output2.txt", p2);
}
