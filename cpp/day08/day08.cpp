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

int countSharedLetters(std::string w1, std::string w2)
{
    bool freq1[7] = { false };
    bool freq2[7] = { false };

    for (int i = 0; i < w1.length(); i++)
        freq1[w1[i] - 'a'] = true;

    for (int i = 0; i < w2.length(); i++)
        freq2[w2[i] - 'a'] = true;

    int sharedLetters = 0;
    for (int i = 0; i < 7; i++)
    {
        if (freq1[i] && freq2[i])
            sharedLetters++;
    }
    return sharedLetters;
}
bool isSame(std::string w1, std::string w2)
{
    bool freq1[7] = { false };
    bool freq2[7] = { false };

    for (int i = 0; i < w1.length(); i++)
        freq1[w1[i] - 'a'] = true;

    for (int i = 0; i < w2.length(); i++)
        freq2[w2[i] - 'a'] = true;

    for (int i = 0; i < 7; i++)
    {
        if (freq1[i] != freq2[i])
            return false;
    }
    return true;
}

std::vector<std::string> getWords(std::string line)
{
    int length = line.length();
    std::vector<std::string> words;
    std::string word;
    for (int i = 0; i < length; i++)
    {
        char c = line[i];
        if (i == length - 1)
            word += c;

        if (c == ' ' || i == length - 1)
        {
            words.push_back(word);
            word = "";
        }
        else {
            word += c;
        }
    }
    return words;
}

void findNumberConfigs(std::string (&numbers)[10], std::string config)
{
    int length = config.length();
    std::vector<std::string> allwords = getWords(config);
    std::vector<std::string> sixLetterWords; //0,6,9
    std::vector<std::string> fiveLetterWords; //2,3,5
    for (std::string word : allwords)
    {
        int wordLength = word.length();
        if (wordLength == 2)
            numbers[1] = word;
        else if (wordLength == 3)
            numbers[7] = word;
        else if (wordLength == 4)
            numbers[4] = word;
        else if (wordLength == 5)
            fiveLetterWords.push_back(word);
        else if (wordLength == 6)
            sixLetterWords.push_back(word);
        else if (wordLength == 7)
            numbers[8] = word;
    }
    for (int i = sixLetterWords.size()-1; i >= 0; i--)
    {
        std::string word = sixLetterWords[i];
        if (countSharedLetters(word, numbers[4]) == 4)
            numbers[9] = word;
        else if (countSharedLetters(word, numbers[8]) == 6 && countSharedLetters(word, numbers[1]) == 1)
            numbers[6] = word;
        else
            numbers[0] = word;

        sixLetterWords.erase(sixLetterWords.begin() + i);
    }
    for (int i = fiveLetterWords.size() - 1; i >= 0; i--)
    {
        std::string word = fiveLetterWords[i];
        if (countSharedLetters(word, numbers[6]) == 5)
            numbers[5] = word;
        else if (countSharedLetters(word, numbers[4]) == 3)
            numbers[3] = word;
        else
            numbers[2] = word;
        fiveLetterWords.erase(fiveLetterWords.begin() + i);
    }
}

int solvePart1(std::string line, std::string (&numbers)[10])
{
    int length = line.length();
    int count = 0;
    std::vector<std::string> allwords = getWords(line);
    for (auto word : allwords)
    {
        int wordLength = word.length();
        if (wordLength == numbers[1].length() || wordLength == numbers[4].length() ||
            wordLength == numbers[7].length() || wordLength == numbers[8].length())
        {
            count++;
        }
    }
    return count;
}

int solvePart2(std::string line, std::string(&numbers)[10])
{
    int length = line.length();
    int sum = 0;
    std::vector<std::string> allwords = getWords(line);
    std::string outputValue;
    for (auto word : allwords)
    {
        for (int j = 0; j < 10; j++)
        {
            if (isSame(word, numbers[j]))
            {
                outputValue += std::to_string(j);
                break;
            }
        }
    }
    return std::stoi(outputValue);
}

int main()
{
    std::string numbers[10] = { "" };
    std::ifstream file("input.txt");
    std::string line, config, outputPart;
    int part1 = 0, part2 = 0, numberSequence = 0;
    while (std::getline(file, line))
    {
        int splitPos = line.find(" | ");
        config = line.substr(0, splitPos);
        outputPart = line.substr(splitPos + 3);
        findNumberConfigs(numbers, config);
        part1 += solvePart1(outputPart, numbers);
        numberSequence = solvePart2(outputPart, numbers);
        part2 += numberSequence;
    }
    writeValueToFile("output1.txt", part1);
    writeValueToFile("output2.txt", part2);
}