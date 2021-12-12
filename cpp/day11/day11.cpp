#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

const int LENGTH = 10;
const int MAX_STEPS = 10000;
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
     // mark the point so that I know if I passed through it.

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
        std::string output = "";
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

                output += std::to_string(octopuses[i][j]);
            }
            output += "\n";
        }

        std::cout << output << std::endl;
        //if (h < 10)
        //{
        //    Sleep(500);
        //    system("cls");
        //}
        //else if (h >= 224)
        //{
        //    Sleep(500);
        //    system("cls");
        //}
        
        

        if (isSynchronized)
        {
            firstSynchronizedStep = h;
            break;
        }
    }

    std::cout << "First step when all flash: " << firstSynchronizedStep+1 << std::endl;
}
