#include <fstream>
#include <string>

void writeValueToFile(std::string path, int value);
struct Position
{
    public:
        int x = 0;
        int y = 0;
};
int main()
{
    int aim = 0;
    Position pos1, pos2;
    std::ifstream input;
    input.open("input.txt");

    std::string line;
    while (std::getline(input, line))
    {
        int splitPos = line.find(' ');
        std::string action = line.substr(0, splitPos);
        std::string value = line.substr(splitPos + 1, line.length());
        int valueAsInt = std::stoi(value);
        if (action == "up")
        {
            pos1.y -= valueAsInt;
            aim -= valueAsInt;
        }
        else if (action == "down")
        {
            pos1.y += valueAsInt;
            aim += valueAsInt;
        }
        else if (action == "forward")
        {
            pos1.x += valueAsInt;

            pos2.x += valueAsInt;
            pos2.y += aim * valueAsInt;
        } 
    }
    input.close();
    writeValueToFile("output1.txt", (pos1.x * pos1.y));
    writeValueToFile("output2.txt", (pos2.x * pos2.y));
}
void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}