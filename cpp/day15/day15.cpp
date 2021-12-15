#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <queue>
#include <tuple>

const int LENGTH = 100;


enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3
};
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h, r;
};

class Point
{
public:
    Point(int p_risk, int p_x, int p_y)
    {
        risk = p_risk;
        col = p_x;
        row = p_y;
    }
    Point* getNeighbor(Direction dir)
    {
        return neighbors[static_cast<int>(dir)];
    }
    void AddNeighbor(Point* point, Direction dir)
    {
        neighbors[static_cast<int>(dir)] = point;
    }
    int risk = 0;
    int col = 0;
    int row = 0;
    Point* neighbors[4] = { nullptr };

};
bool operator==(const Point& lhs, const Point& rhs) {
    return (lhs.col == rhs.col && lhs.row == rhs.row);
}

bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < LENGTH) && (col >= 0)
        && (col < LENGTH);
}

double calculateHValue(Point* src, Point* dest)
{
    // h is estimated with the two points distance formula
    return abs(dest->col - src->col) + abs(dest->row - src->row);
}
bool isDestination(Point* position, Point* dest)
{
    return *position == *dest;
}

void tracePath(cell cellDetails[LENGTH][LENGTH], std::vector<Point*> &points, Point* dest)
{
    printf("\nThe Path is ");
    int row = dest->row;
    int col = dest->col;

    std::stack<std::pair<int,int>> Path;
    int totalRisk = 0;
    while (!(cellDetails[row][col].parent_i == row
        && cellDetails[row][col].parent_j == col)) {
        Path.push(std::make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        totalRisk += cellDetails[row][col].r;
        row = temp_row;
        col = temp_col;
    }

    Path.push(std::make_pair(row, col));
    while (!Path.empty()) {
        std::pair<int,int> pair = Path.top();
        Path.pop();
        std::cout << " -> " << pair.second << "," << pair.first;
    }
    std::cout << std::endl;

    //TODO: Path risk is off by +5
    std::cout << totalRisk << std::endl;
    return;
}

void aStar(Point* start, Point* dest, std::vector<Point*> &points)
{
    // If the destination cell is the same as source cell
    if (isDestination(start, dest)){
        printf("We are already at the destination\n");
        return;
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[LENGTH][LENGTH] = {false};

    // Declare a 2D array of structure to hold the details
    // of that cell
    cell cellDetails[LENGTH][LENGTH];

    int i, j;
    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < LENGTH; j++) {
            Point* p = points[i * LENGTH + j];
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].r = p->risk;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }
    i = start->row, j = start->col;

    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    std::priority_queue<std::pair<double, Point*>, std::vector<std::pair<double, Point*>>, 
        std::greater<std::pair<double, Point*>>> openList;
        

    // Put the starting cell on the open list and set its
    // 'f' as 0
    
    openList.push(std::make_pair(0.0, start));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;


    while (!openList.empty()) {
        auto p = openList.top();
        openList.pop();
        i = p.second->row;
        j = p.second->col;
        closedList[i][j] = true;

        for (Point* n : p.second->neighbors)
        {
            if (n == nullptr)
                continue;
            double gNew, hNew, fNew;

            int i2, j2;
            i2 = n->row;
            j2 = n->col;
            if (isValid(i2, j2) == true) {
                if (isDestination(n, dest)) {
                    cellDetails[i2][j2].parent_i = i;
                    cellDetails[i2][j2].parent_j = j;
                    printf("The destination cell is found\n");
                    tracePath(cellDetails, points, dest);
                    foundDest = true;
                    return;
                }
                else if (closedList[i2][j2] == false)
                {
                    gNew = cellDetails[i][j].g + 1;
                    hNew = calculateHValue(n, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i2][j2].f == FLT_MAX
                        || cellDetails[i2][j2].f > fNew) {
                        openList.push(std::make_pair(
                            fNew, n));

                        cellDetails[i2][j2].f = fNew;
                        cellDetails[i2][j2].g = gNew;
                        cellDetails[i2][j2].h = hNew;
                        cellDetails[i2][j2].parent_i = i;
                        cellDetails[i2][j2].parent_j = j;
                    }
                }
            }
        }
    }
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return;
}



void solvePart1(std::vector<Point*>& points)
{
    aStar(points[0], points[LENGTH * LENGTH-1], points);
}

int main()
{
    std::ifstream input("test.txt");
    std::string line;

    std::vector<Point*> points;

    int i = 0;
    while (std::getline(input, line))
    {
        for (int j = 0; j < line.length(); j++)
        {
            int risk = line[j] - '0';
            points.push_back(new Point(risk, j, i));
        }
        i++;
    }

    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            Point* point = points[i * LENGTH + j];
            if (i != 0)
            {
                point->AddNeighbor(points[(i - 1) * LENGTH + j], Direction::Up);
            }
            if (i != LENGTH - 1)
            {
                point->AddNeighbor(points[(i + 1) * LENGTH + j], Direction::Down);
            }
            if (j != 0)
            {
                point->AddNeighbor(points[i * LENGTH + (j - 1)], Direction::Left);
            }
            if (j != LENGTH - 1)
            {
                point->AddNeighbor(points[i * LENGTH + (j + 1)], Direction::Right);
            }
        }
    }

    solvePart1(points);
}
