#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <algorithm>


char getCloser(char opener)
{
    switch (opener)
    {
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    case '<':
        return '>';
    default:
        return ' ';
    }
}

char getOpener(char closer)
{
    switch (closer)
    {
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    case '>':
        return '<';
    default:
        return ' ';
    }
}

bool isLineCorrupt(std::string line, char& illegalCharacter)
{
    std::stack<char> openSymbols;
    for (char c : line)
    {
        switch (c)
        {
        case '(':
        case '[':
        case '{':
        case '<':
            openSymbols.push(c);
            break;
        case ')':
        case ']':
        case '}':
        case '>':
            if (openSymbols.size() > 0)
            {
                if (openSymbols.top() == getOpener(c))
                {
                    openSymbols.pop();
                    continue;
                }
                else
                {
                    illegalCharacter = c;
                    return true;
                }
            }
            else
            {
                illegalCharacter = c;
                return true;
            }
        break;
        default:
            break;
        }

    }
    return false;
}
uint64_t getUnfinishedLineScore(std::string line, std::map<char, int> (&symbolValues))
{
    std::stack<char> openSymbols;


    for (char c : line)
    {
        switch (c)
        {
        case '(':
        case '[':
        case '{':
        case '<':
            openSymbols.push(c);
            break;
        case ')':
        case ']':
        case '}':
        case '>':
            if (openSymbols.size() > 0)
            {
                if (openSymbols.top() == getOpener(c))
                {
                    openSymbols.pop();
                    continue;
                }
            }
            break;
        }
    }

    uint64_t score = 0;

    while (openSymbols.size() > 0)
    {
        char opener = openSymbols.top();
        openSymbols.pop();
        score *= 5;
        char closer = getCloser(opener);
        score += symbolValues[closer];
    }

    return score;
}


int main()
{
    std::ifstream input("input.txt");
    std::string line;

    std::map<char, int> symbolValues1 = 
    {
        {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}
    };
    std::map<char, int> symbolValues2 =
    {
        {')', 1}, {']', 2}, {'}', 3}, {'>', 4}
    };
    std::map<char, int> symbolCount =
    {
        {')', 0}, {']', 0}, {'}', 0}, {'>', 0}
    };

    std::vector<uint64_t> part2Scores;
    
    while (std::getline(input, line))
    {
        char illegalChar = ' ';
        if (isLineCorrupt(line, illegalChar)) //part1
        {
            if (illegalChar != ' ')
            {
                if (symbolValues1.count(illegalChar) > 0)
                {
                    symbolCount[illegalChar]++;
                    continue;
                }
            }
        }
        else //part2, no complete lines by default
        {
            part2Scores.push_back(getUnfinishedLineScore(line, symbolValues2));
        }
    }
    std::map<char, int>::iterator it;
    uint64_t score = 0;
    for (it = symbolCount.begin(); it != symbolCount.end(); it++)
    {
        score += it->second * symbolValues1[it->first];
    }

    std::sort(part2Scores.begin(), part2Scores.end());

    uint64_t middleScore = part2Scores[(part2Scores.size()-1) / 2];

    std::cout << score << std::endl;
    std::cout << middleScore << std::endl;
}
