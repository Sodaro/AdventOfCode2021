#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

const int ROW_LENGTH = 5;
const int ROW_COUNT = 5;
const int NUMBERS_DRAWN = 100;
void writeValueToFile(std::string path, int value)
{
    std::ofstream output;
    output.open(path);
    output << std::to_string(value) << std::endl;
    output.close();
}

struct Node
{
public:
    int value = 0;
    bool isDrawn = false;
};

struct Row
{
public:
    Node* values[ROW_LENGTH];
};

struct Board
{
public:
    int index = 0;
    //5 cols, 5 rows, 2 diagonals
    Row* rows[10] = { 0 };
    int rowCount = 0;
    void addRow(Row* row)
    {
        rows[rowCount] = row;
        rowCount++;
    }
};

bool checkIfBingo(Board* board)
{
    int drawnNumbersInRow = 0;
    for (Row* row : board->rows)
    {
        for (Node* node : row->values)
        {
            if (node->isDrawn)
                drawnNumbersInRow++;
        }
        if (drawnNumbersInRow == 5)
        {
            return true;
        }
        drawnNumbersInRow = 0; 
    }
    return false;
}

std::vector<Board*> checkNumberAgainstBoards(int number, std::vector<Board*> &boards)
{
    //checks the drawn number against boards, returns a vector of updated boards
    std::vector<Board*> updatedBoards;
    for (Board* board : boards)
    {
        for (int i = 0; i < ROW_LENGTH; i++)
        {
            for (Node* node : board->rows[i]->values)
            {
                if (node->value == number)
                {
                    node->isDrawn = true;
                    updatedBoards.push_back(board);
                    break;
                }
            }
        }
        
    }
    return updatedBoards;
}

int getRowSum(Row* row)
{
    int sum = 0;
    for (Node* node : row->values)
    {
        if (!node->isDrawn)
            sum += node->value;
    }
        
    return sum;
}
int getBoardSum(Board* board)
{
    int sum = 0;
    for (int i = 0; i < ROW_LENGTH; i++)
        sum += getRowSum(board->rows[i]);

    return sum;
}

void setupGame(std::vector<Board*> &boards, int (&drawnNumbers)[NUMBERS_DRAWN])
{
    std::ifstream input("input.txt");

    //first line contains 100 numbers we check against board in draw order
    //there are 100 boards, each board is 5x5
    std::string line;
    std::getline(input, line);
    
    int i = 0;
    int j = 0;
    int number;

    std::string nrString;
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
        {
            number = std::stoi(nrString);
            drawnNumbers[j] = number;
            j++;
            nrString = "";
            continue;
        }
        nrString += line[i];
    }
    number = std::stoi(nrString);
    drawnNumbers[j] = number;


    i = 0; j = 0;
    Board* board = new Board();
    Row* row = new Row();
    int boardIndex = 0;
    while (input >> number)
    {

        Node* node = new Node();
        node->value = number;
        row->values[i] = node;
        i++;
        if (i == 5)
        {
            board->addRow(row);
            row = new Row();
            j++;
            if (j == 5)
            {
                for (int x = 0; x < 5; x++)
                {
                    Row* col = new Row();
                    for (int y = 0; y < 5; y++)
                    {
                        col->values[y] = board->rows[y]->values[x];
                    }
                    board->addRow(col);
                }
                //Row* diagonal1 = new Row();
                //for (int x = 0, y = 0; x < 5; x++, y++)
                //    diagonal1->values[x] = board->rows[x]->values[y];

                //Row* diagonal2 = new Row();
                //for (int x = 0, y = 4; x < 5; x++, y--)
                //    diagonal2->values[x] = board->rows[x]->values[y];

                //board->addRow(diagonal1);
                //board->addRow(diagonal2);
                board->index = boardIndex;
                boards.push_back(board);
                boardIndex++;
                board = new Board();
                j = 0;
            }
            i = 0;
        }
    }
    input.close();
}

void solvePart1(std::vector<Board*>&boards, int (&drawnNumbers)[NUMBERS_DRAWN])
{
    for (int number : drawnNumbers)
    {
        std::vector<Board*> updatedBoards = checkNumberAgainstBoards(number, boards);
        for (Board* board : updatedBoards)
        {
            if (checkIfBingo(board))
            {
                writeValueToFile("output1.txt", getBoardSum(board) * number);
                return;
            }
        }
    }
}

void deleteBoard(std::vector<Board*>& boards, int index)
{
    for (int i = boards.size() - 1; i >= 0; i--)
    {
        if (boards[i]->index == index)
        {
            for (int j = ROW_COUNT-1; j >= 0; j--)
            {
                for (int k = ROW_LENGTH-1; k >= 0; k--)
                {
                    free(boards[i]->rows[j]->values[k]);
                }
                free(boards[i]->rows[j]);
            }
            free(boards[i]);
            //free(boards[i]->rows[j]);
            boards.erase(boards.begin() + i);
        }
    }
}

void solvePart2(std::vector<Board*> &boards, int(&drawnNumbers)[NUMBERS_DRAWN])
{
    std::vector<Board*> boardsCopy = boards;
    bool foundBingo = false;
    for (int number : drawnNumbers)
    {
        std::vector<Board*> updatedBoards = checkNumberAgainstBoards(number, boardsCopy);
        for (Board* board : updatedBoards)
        {
            foundBingo = checkIfBingo(board);
            if (foundBingo)
            {
                if (boardsCopy.size() == 1)
                {
                    writeValueToFile("output2.txt", getBoardSum(boardsCopy[0]) * number);
                    return;
                }
                for (int i = boardsCopy.size() - 1; i >= 0; i--)
                {
                    if (boardsCopy[i]->index == board->index)
                    {
                        boardsCopy.erase(boardsCopy.begin() + i);
                        break;
                    }
                }
            }
        }

    }
}
void executeAndMeasure(void (*ptr)(std::vector<Board*>&, int (&)[NUMBERS_DRAWN]), std::vector<Board*> boards, int(&numbers)[NUMBERS_DRAWN])
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();
    ptr(boards, numbers);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
}


int main()
{
    int drawnNumbers[NUMBERS_DRAWN] = { 0 };
    std::vector<Board*> boards;
    setupGame(boards, drawnNumbers);
    executeAndMeasure(solvePart1, boards, drawnNumbers);
    executeAndMeasure(solvePart2, boards, drawnNumbers);
    return 0;
}
