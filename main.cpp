#include <iostream>
#include <queue>
#include <algorithm>
#include <windows.h>

#define ROW 8
#define COL 8

#define leftArrow 0x25
#define rightArrow 0x27
#define downArrow 0x28
#define upArrow 0x26
#define enter 0x0D
#define escape 0x1B
#define xKey 0x58
#define zKey 0x5A

std::deque<int> queue;
std::vector<int> visited;
std::vector<int> currentLocation;

int index = 0;
int editX = 0;
int editY = 0;
bool bfsON = false;
bool start = false;


int maze[ROW][COL] = {
    {0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,1,0},
    {0,0,0,0,1,0,0,0},
    {0,0,0,0,1,0,0,0},
    {1,0,0,1,1,1,1,1},
    {0,0,0,0,1,1,0,1},
    {0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,1,1},
};

int colorMap[ROW][COL];

struct list {
    int x;
    int y;
};

std::vector<list> path;
std::vector<int> reconstructedPath;

list convertCombined(int coordinates) {
    int x;
    int y;
    x = coordinates % 10;
    y = coordinates / 10;
    list result = { x, y };
    return result;
}

bool hasInsideVector(std::vector<int> vector, int value) {
    for (int i : vector) {
        if (i == value) {
            return 1;
        }
    }
    return 0;
}

bool hasInsideQueue(std::deque<int> queue, int value) {
    for (int i : queue) {
        if (i == value) {
            return 1;
        }
    }
    return 0;
}


bool checkTile(int currentLocation) {
    if (!(hasInsideVector(visited, currentLocation)) && !(hasInsideQueue(queue, currentLocation))) {
        return 1;
    }
    else {
        return 0;
    }
}

bool checkValid(int maze[ROW][COL], int currentLocation) {
    int x;
    int y;
    list location = convertCombined(currentLocation);
    x = location.x - 1;
    y = location.y - 1;
    return maze[y][x] != 1 && x >= 0 && x < COL && y >= 0 && y < ROW;
}

void addQueue(int maze[ROW][COL]) {
    if (checkValid(maze, currentLocation.back() + 1) && checkTile(currentLocation.back() + 1)) {
        queue.push_back(currentLocation.back() + 1);
        path.push_back({ queue.back(), currentLocation.back() });
    } 
    if (checkValid(maze, currentLocation.back() + 10) && checkTile(currentLocation.back() + 10)) {
        queue.push_back(currentLocation.back() + 10);
        path.push_back({ queue.back(), currentLocation.back() });
    } 
    if (checkValid(maze, currentLocation.back() - 10) && checkTile(currentLocation.back() - 10)) {
        queue.push_back(currentLocation.back() - 10);
        path.push_back({ queue.back(), currentLocation.back() });
    }
    if (checkValid(maze, currentLocation.back() - 1) && checkTile(currentLocation.back() - 1)) {
        queue.push_back(currentLocation.back() - 1);
        path.push_back({ queue.back(), currentLocation.back() });

    }
}

void makePath(int end) {
    std::reverse(path.begin(), path.end());
    int prev = path.front().y;
    reconstructedPath.push_back(path.front().x);
    for (auto i : path) {
        if (i.x == prev) {
            reconstructedPath.push_back(i.x);
            prev = i.y;
        }
      }
}

void errorTab() {
    std::system("cls");
    std::cout << "There has been an error" << std::endl;
    std::system("pause");
}

void bfs(int maze[ROW][COL], int start, int end) {
    if (maze[convertCombined(start).y - 1][convertCombined(start).x - 1] != 1) {
        bfsON = true;
        visited.push_back(start);
        queue.push_back(start);
        path.push_back({ start,start });
        while (queue.size() != 0) {
            currentLocation.push_back(queue.front());
            if (queue.front() == end) {
                path.push_back({ end,end });
                makePath(end);
                return;
            }
            visited.push_back(queue.front());
            queue.pop_front();
            addQueue(maze);
        }
    }
    else{
        errorTab();
    }

}




void showMaze(){
    std::system("cls");
    std::cout << "     Maze" << std::endl;
    std::cout << std::endl;
    

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (bfsON) {
                for (int k : reconstructedPath) {
                    int x;
                    int y;
                    list coord;
                    coord = convertCombined(k);
                    x = coord.x - 1;
                    y = coord.y - 1;
                    colorMap[y][x] = 32;
                }
                if (maze[i][j] == 1) {
                    if (j == 7) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                        std::cout << maze[i][j];
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                        std::cout << " ";
                    }
                    else {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                        std::cout << maze[i][j] << " ";
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    }
                }
                if (maze[i][j] == 0) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                    std::cout << maze[i][j] << " ";
                }
            }
            else {
                if(start){
                
                    if (j == editX && i == editY) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 30);
                        std::cout << maze[i][j] << " ";
                        continue;
                    }
                }
                if (maze[i][j] == 1) {
                    colorMap[i][j] = 240;
                    if (j == 7) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                        std::cout << maze[i][j];
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                        std::cout << " ";
                    }
                    else {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                        std::cout << maze[i][j] << " ";
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    }
                }
                if (maze[i][j] == 0) {
                    colorMap[i][j] = 15;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                    std::cout << maze[i][j] << " ";
                }
            }
        }
            std::cout << std::endl;
    }
    if (start) { 
        std::cout << std::endl; 
        std::cout << "Z - Start\tX - End" << std::endl; 
    }
    std::cout << std::endl;
}

void showMenu() {
    std::system("cls");
    
    showMaze();

    switch (index) {
    case 0:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
        std::cout << "Start\t";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        std::cout << "Edit\t";
        std::cout << "Exit";
        break;
    case 1:
        std::cout << "Start\t";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
        std::cout << "Edit\t";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        std::cout << "Exit";
        break;
    case 2:
        std::cout << "Start\t";
        std::cout << "Edit\t";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
        std::cout << "Exit";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
        return;
    }

}

void editMenu() {
    std::system("cls");
    std::cout << "   Edit Maze" << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (j == editX && i == editY) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 30);
                std::cout << maze[i][j] << " ";
                continue;
            }
            if (maze[i][j] == 1) {
                colorMap[i][j] = 240;
                if (j == 7) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                    std::cout << maze[i][j];
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    std::cout << " ";
                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                    std::cout << maze[i][j] << " ";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                }
            }
            if (maze[i][j] == 0) {
                colorMap[i][j] = 15;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[i][j]);
                std::cout << maze[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Z - Toggle\tESC - Return " << std::endl;
}

void editTab() {
    std::system("cls");
    bfsON = false;
    queue.clear();
    visited.clear();
    currentLocation.clear();
    path.clear();
    reconstructedPath.clear();

    editMenu();
    while (true) {
        if (GetKeyState(escape) & 0x8000) {
            showMenu();
            return;
        }
        if (GetKeyState(zKey) & 0x8000) {
            if (maze[editY][editX] == 0) {
                Sleep(150);
                maze[editY][editX] = 1;
                colorMap[editY][editX] = 240;
                editMenu();
            }
        }
        if (GetKeyState(zKey) & 0x8000) {
            if (maze[editY][editX] == 1) {
                Sleep(150);
                maze[editY][editX] = 0;
                colorMap[editY][editX] = 15;
                editMenu();
            }
        }
        if ((GetKeyState(rightArrow) & 0x8000) && editX < ROW) {
            editX++;
            Sleep(150);
            editMenu();
        }
        if ((GetKeyState(leftArrow) & 0x8000) && editX > 0) {
            editX--;
            Sleep(150);
            editMenu();
        }
        if ((GetKeyState(upArrow) & 0x8000) && editY > 0) {
            editY--;
            Sleep(150);
            editMenu();
        }
        if ((GetKeyState(downArrow) & 0x8000) && editY < COL) {
            editY++; 
            Sleep(150); 
            editMenu();
        }
        
    }
}

void startTab() {
    int beginning = 0;
    int end = 0;
    bfsON = false;
    start = true;
    bool inside = true;
    queue.clear();
    visited.clear();
    currentLocation.clear();
    path.clear();
    reconstructedPath.clear();
    std::system("cls");

    showMaze();
    while (inside) {
        if (GetKeyState(escape) & 0x8000) {
            showMenu();
            return;
        }
        if (GetKeyState(zKey) & 0x8000) {
            beginning = (editY + 1) * 10 + (editX + 1);
            Sleep(150);
            showMaze();
        }
        if (GetKeyState(xKey) & 0x8000) {
            end = (editY + 1) * 10 + (editX + 1);
            Sleep(150);
            inside = false;
            showMaze();
        }
        if ((GetKeyState(rightArrow) & 0x8000) && editX < ROW) {
            editX++;
            Sleep(150);
            showMaze();
        }
        if ((GetKeyState(leftArrow) & 0x8000) && editX > 0) {
            editX--;
            Sleep(150);
            showMaze();
        }
        if ((GetKeyState(upArrow) & 0x8000) && editY > 0) {
            editY--;
            Sleep(150);
            showMaze();
        }
        if ((GetKeyState(downArrow) & 0x8000) && editY < COL) {
            editY++;
            Sleep(150);
            showMaze();
        }
    }
    bfs(maze, beginning, end);
    std::system("cls");
    showMenu();
    
}


void menu() {
    showMenu();
    while (true) {  
        if (GetKeyState(enter) & 0x8000) {
            switch(index){
            case 0:
                startTab();
                break;
            case 1:
                editTab();
                break;
            case 2:                
                return;
            }
        }
        if ((GetKeyState(rightArrow) & 0x8000) && index < 2){
            index++;
            Sleep(150);
            showMenu();
        }
        if ((GetKeyState(leftArrow) & 0x8000) && index > 0) {
            index--;
            Sleep(150);
            showMenu();
        }
    }
}

int main() {
    menu();
    return 0;
}
