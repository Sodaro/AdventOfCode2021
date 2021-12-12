#include <fstream>
#include <string>

const int LENGTH = 10;
const int MAX_STEPS = 10000;

void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}


void setup(int(&octopuses)[LENGTH][LENGTH])
{
    int i = 0;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line))
    {
        int number;
        for (int j = 0; j < LENGTH; j++)
        {
            number = line[j] - '0';
            octopuses[i][j] = number;
        }
        i++;
    }
}

void flash(int row, int col, int (&octopuses)[LENGTH][LENGTH], int &nrOfFlashes)
{
    if (octopuses[row][col] == -1)
        return;

    octopuses[row][col]++;
    if (octopuses[row][col] > 9)
    {
        nrOfFlashes++;
        octopuses[row][col] = -1;
        if (row > 0 && octopuses[row - 1][col] != -1)
        {
            flash(row - 1, col, octopuses, nrOfFlashes);
        }
            

        if (col > 0 && octopuses[row][col - 1] != -1)
        {
            flash(row, col - 1, octopuses, nrOfFlashes);
        }
            

        if (row > 0 && col > 0
            && octopuses[row - 1][col - 1] != -1)
        {
            flash(row - 1, col - 1, octopuses, nrOfFlashes);
        }
           

        if (row < LENGTH - 1 && octopuses[row + 1][col] != -1)
        {
            flash(row + 1, col, octopuses, nrOfFlashes);
        }
            

        if (col < LENGTH - 1 && octopuses[row][col + 1] != -1)
        {
            flash(row, col + 1, octopuses, nrOfFlashes);
        }
            

        if (row < LENGTH - 1 && col < LENGTH - 1
            && octopuses[row + 1][col + 1] != -1)
        {
            flash(row + 1, col + 1, octopuses, nrOfFlashes);
        }
            

        if (row < LENGTH - 1 && col > 0
            && octopuses[row + 1][col - 1] != -1)
        {
            flash(row + 1, col - 1, octopuses, nrOfFlashes);
        }
            

        if (row > 0 && col < LENGTH - 1
            && octopuses[row - 1][col + 1] != -1)
        {
            flash(row - 1, col + 1, octopuses, nrOfFlashes);
        }
            
    }
}

int main()
{
    int octopuses[LENGTH][LENGTH] = {0};
    int nrOfFlashes = 0;
    int nrOfFlashesAtStep100 = 0;
    setup(octopuses);
    int firstSynchronizedStep = 0;
    for (int h = 0; h < MAX_STEPS; h++)
    {
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {
                octopuses[i][j]++;
            }
            
        }
        
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {   
                if (octopuses[i][j] > 9)
                {
                    flash(i, j, octopuses, nrOfFlashes);
                }
            }
        }
        if (h == 99)
            nrOfFlashesAtStep100 = nrOfFlashes;
        
        bool isSynchronized = true;
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {
                if (octopuses[i][j] == -1)
                    octopuses[i][j] = 0;
                else
                    isSynchronized = false;
            }
        }   

        if (isSynchronized)
        {
            firstSynchronizedStep = h;
            break;
        }
    }
    writeValueToFile("output1.txt", nrOfFlashesAtStep100);
    writeValueToFile("output2.txt", firstSynchronizedStep + 1);
}
