// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include "raylib.h"
using namespace std;
using f = float;
using ll = long long;
using ull = unsigned long long;

vector<Color> colors = { RED, BLUE, YELLOW, GREEN };
int current_color = 2;

ll tick = 0;
ll score = 0;


ll color_to_ll(Color c) {
    return c.r + 256 * c.g + 65536 * c.b + 16777216 * c.a;
}

struct Vector2int {
    int x;
    int y;
};

class Figure {
protected:
    vector<Vector2int> points;
    int rotates;
    int r_rotate;
public:
    string name;
    Figure(vector<Vector2int> _points, int _rotates, int _r_rotate) {
        points = _points;
        rotates = _rotates;
        r_rotate = _r_rotate;
    }
    Figure() {
        points = {};
        rotates = 1;
        r_rotate = 1;
    }
    virtual vector<Vector2int> getPoints() const = 0;
    // right rotate
    virtual void rotate(bool is_back = 0) = 0;
};

class Square : public Figure {
public:
    Square() {
        points = { {0, 0}, {0, -1}, {1, 0}, {1, -1} };
        rotates = 1;
        r_rotate = 1;
    }
    vector<Vector2int> getPoints() const{
        return points;
    }
    void rotate(bool is_back = 0) override {
        // nothsing
    }
};

class Line : public Figure {
public:
    Line() {
        points = { {0, 0}, {0, -1}, {0, -2}, {0, -3} };
        rotates = 2;
        r_rotate = 2;
    }
    vector<Vector2int> getPoints() const {
        return points;
    }
    void rotate(bool is_back = 0) override {
        r_rotate = (r_rotate - 1 + (is_back ? -1 : 1) + rotates) % rotates + 1;
        if      (r_rotate == 1) points = { {-1, 0}, {0, 0}, {1, 0}, {2, 0} };
        else if (r_rotate == 2) points = { {0, 0}, {0, -1}, {0, -2}, {0, -3} };
    }
};

class T_Form : public Figure {
public:
    T_Form() {
        points = { {0, 0}, {-1, -1}, {0, -1}, {1, -1} };
        rotates = 4;
        r_rotate = 1;
    }
    vector<Vector2int> getPoints() const {
        return points;
    }
    void rotate(bool is_back = 0) override {
        r_rotate = (r_rotate - 1 + (is_back ? -1 : 1) + rotates) % rotates + 1;
        if      (r_rotate == 1) points = { {0,0},{-1,0},{1,0},{0,1} };
        else if (r_rotate == 2) points = { { 0, 0 }, { 0,1 }, { 0,-1 }, { -1,0 } };
        else if (r_rotate == 3) points = { {0,0},{-1,0},{1,0},{0,-1} };
        else if (r_rotate == 4) points = { {0,0},{0,1},{0,-1},{1,0} };
    }
};

class L_Form : public Figure {
public:
    L_Form() {
        points = { {0, 0}, {0, -1}, {0, -2}, {1, -2} };
        rotates = 4;
        r_rotate = 4;
    }
    vector<Vector2int> getPoints() const {
        return points;
    }
    void rotate(bool is_back = 0) override {
        is_back = !is_back;
        r_rotate = (r_rotate - 1 + (is_back ? -1 : 1) + rotates) % rotates + 1;
        if (r_rotate == 1) points = { {-1,-1}, {0,-1}, {1,-1}, {-1,-2} };
        else if (r_rotate == 2) points = { {0, 0}, {-1, 0}, {0,-1}, {0,-2} };
        else if (r_rotate == 3) points = { {-1,-1}, {0,-1}, {1,-1}, {1, 0} };   
        else if (r_rotate == 4) points = { {0, 0}, {0,-1}, {0,-2}, {1,-2} };
    }
};

class S_Form : public Figure {
public:
    S_Form() {
        points = { {0, 0}, {0, -1}, {-1, -1}, {-1, -2} };
        rotates = 2;
        r_rotate = 2; 
    }
    vector<Vector2int> getPoints() const {
        return points;
    }
    void rotate(bool is_back = 0) override {
        r_rotate = (r_rotate - 1 + (is_back ? -1 : 1) + rotates) % rotates + 1;
        if (r_rotate == 1) points = { {-1,-1}, {0,-1}, {0,-2}, {1,-2} };
        else if (r_rotate == 2) points = { {0, 0}, {0,-1}, {-1,-1}, {-1,-2} };
    }
};


int fig_count = 5;


Line line;
Square square;
T_Form T_form;
L_Form L_form;
S_Form S_form;


Figure& getRandFigure(int t) {
    int r = (rand()+t) % fig_count;
    if (r == 0) {
        line = {};
        return line;
    }
    if (r == 1) {
        square = {};
        return square;
    }
    if (r == 2) {
        T_form = {};
        return T_form;
    }
    if (r == 3) {
        L_form = {};
        return L_form;
    }
    if (r == 4) {
        S_form = {};
        return S_form;
    }
}



class plaints {
private:
    int x;
    int y;
    bool is_static;
    bool is_ful;
    Color pl_Color;
public:
    plaints(int _x, int _y, bool _is_static, bool _is_ful, Color _pl_Color) {
        x = _x;
        y = _y;
        is_static = _is_static;
        is_ful = _is_ful;
        pl_Color = _pl_Color;
    }

    plaints(int _x, int _y) {
        x = _x;
        y = _y;
        is_static = 1;
        is_ful = 0;
        pl_Color = DARKGRAY;
    }

    plaints() : plaints(0, 0) {}

    void setAll(bool _is_static, bool _is_ful, Color _pl_Color) {
        is_static = _is_static;
        is_ful = _is_ful;
        pl_Color = _pl_Color;
    }

    bool getFul() const {
        return is_ful && is_static;
    }

    bool getStatic() const {
        return is_static;
    }

    void setStatic(bool _is_static) {
        is_static = _is_static;
    }

    Color getColor() const {
        return pl_Color;
    }
};

class Grid {
private:
    int height;
    int width;
    vector<vector<plaints>> grid;
public:
    Grid(int _height, int _width) {
        height = _height;
        width = _width;
        grid.resize(height);
        for (int y = 0; y < height; y++) {
            grid[y].resize(width);
            for (int x = 0; x < width; x++) {
                grid[y][x] = plaints(x, y);
            }
        }
    }

    plaints& getAt(int x, int y) {
        return grid[y][x];
    }

    void setPl(int x, int y, bool _is_static, bool _is_ful, Color _pl_Color) {
        grid[y][x].setAll(_is_static, _is_ful, _pl_Color);
    }

    bool is_lines_ful(int stairs) {
        bool k = 1;
        for (int x = 0; x < width; x++) {
            k = k && grid[stairs][x].getFul();
        }
        return k;
    }

    bool is_lines_void(int stairs) {
        bool k = 1;
        for (int x = 0; x < width; x++) {
            k = k && !grid[stairs][x].getFul();
        }
        return k;
    }

    void del_ful_lines() {
        int count = 0;
        for (int y = 0; y < height; y++) {
            if (is_lines_ful(y)) {
                for (int x = 0; x < width; x++) {
                    grid[y][x].setAll(1, 0, DARKGRAY);
                }
                count++;
            }
        }
        score += 50 * width * (count * count + count) / 2;
    }


    void drawFigure(const Figure& f, Vector2int start_pos) {
        for (const Vector2int& i : f.getPoints()) {
            grid[start_pos.y + i.y][start_pos.x + i.x].setAll(0, 1, colors[current_color]);
        }

    }


    void ltick() {
        for (int y = 0; y < height - 1; y++) {
            if (is_lines_void(y)) {
                for (int x = 0; x < width; x++) {
                    if (grid[y + 1][x].getStatic() && (color_to_ll(grid[y + 1][x].getColor())) != color_to_ll(DARKGRAY)) { // lib autor don't add operator== to his class
                        grid[y][x] = grid[y + 1][x];
                        grid[y + 1][x].setAll(1, 0, DARKGRAY);
                    }
                }
            }
        }
    }
    bool dtick(const Figure& f, Vector2int& start_pos){
        bool k = 1;
        for (const Vector2int& i : f.getPoints()) {
            int lx = i.x + start_pos.x;
            int ly = i.y + start_pos.y;
            if (ly - 1 < 0) {
                k = 0;
                break;
            }
            if (!(color_to_ll(grid[ly - 1][lx].getColor()) == color_to_ll(DARKGRAY) || !grid[ly - 1][lx].getStatic())) k = 0;
        }
        if (k) {
            for (int y = 0; y < height - 1; y++) {
                for (int x = 0; x < width; x++) {
                    if (!grid[y + 1][x].getStatic()) {
                        grid[y][x] = grid[y + 1][x];
                        grid[y + 1][x].setAll(1, 0, DARKGRAY);
                    }
                }
            }
            start_pos.y--;
            return true;
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid[y][x].setStatic(true);
            }
        }
        return false;
    }

    void move_left() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width - 1; x++) {
                if (!grid[y][x + 1].getStatic()) {
                    grid[y][x] = grid[y][x + 1];
                    grid[y][x + 1].setAll(1, 0, DARKGRAY);
                }
            }
        }
    }

    void move_right() {
        for (int y = 0; y < height; y++) {
            for (int x = width; x > 0; x--) {
                if (!grid[y][x - 1].getStatic()) {
                    grid[y][x] = grid[y][x - 1];
                    grid[y][x - 1].setAll(1, 0, DARKGRAY);
                }
            }
        }
    }

    bool is_free(const Figure& f, Vector2int start_pos, bool ignore_nstatic = 0) {
        bool k = 1;
        for (const Vector2int& i : f.getPoints()) {
            int lx = i.x + start_pos.x;
            int ly = i.y + start_pos.y;
            if (lx < 0 || lx >= width || ly < 0 || ly >= height) {
                k = 0;
                break;
            }
            if (grid[ly][lx].getFul()) {
                k = 0;
                break;
            }
        }
        return k;
    }

    void del_all_nstatic() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (!grid[y][x].getStatic()) {
                    grid[y][x].setAll(1, 0, DARKGRAY);
                }
            }
        }
    }
};


Vector2int spawnPoint = { 5, 30 };
int main()
{
    int fps = 8;
    int height = 32;
    int width = 15;
    int cell_size = 30;
    Grid g = { height, width };
    InitWindow(width * cell_size + 20 + 250, height * cell_size + 20, "raylib");

    Figure* current_figure = &getRandFigure(tick);
    Vector2int figure_pos = spawnPoint;
    g.drawFigure(*current_figure, figure_pos);
    bool is_live = 1;
    bool temp = 1;

    SetTargetFPS(1);
    while (!WindowShouldClose()) {
        if (temp) {
            SetTargetFPS(fps);
            temp = 0;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(width * cell_size + 10, 10, 250, height * cell_size, BLACK);
        if (!is_live) {
            figure_pos = spawnPoint;
            
            current_figure = &getRandFigure(tick);
            if (!g.is_free(*current_figure, figure_pos)) break;
            current_color = rand() % 4;
            g.drawFigure(*current_figure, figure_pos);
        }

        if (IsKeyDown(KEY_LEFT) && g.is_free(*current_figure, { figure_pos.x - 1, figure_pos.y })) {
            figure_pos.x -= 1;
            g.move_left();
        }

        if (IsKeyDown(KEY_RIGHT) && g.is_free(*current_figure, { figure_pos.x + 1, figure_pos.y })) {
            figure_pos.x += 1;
            g.move_right();
        }

        if (IsKeyPressed(KEY_DOWN)) {
            fps *= 4;
            temp = 1;
        }
        if (IsKeyReleased(KEY_DOWN)) {
            fps /= 4;
            temp = 1;
        }

        if (IsKeyDown(KEY_UP)) {
            current_figure->rotate();
            if (g.is_free(*current_figure, figure_pos, 1)) {
                g.del_all_nstatic();
                g.drawFigure(*current_figure, figure_pos);
            }
            else {
                current_figure->rotate(1);
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                DrawRectangle(10 + x * cell_size, 10 + y * cell_size, cell_size, cell_size, g.getAt(x, height - y - 1).getColor());
                if (g.getAt(x, height - y - 1).getStatic()) DrawRectangleLines(10 + x * cell_size, 10 + y * cell_size, cell_size, cell_size, BLACK);
                else DrawRectangleLines(10 + x * cell_size, 10 + y * cell_size, cell_size, cell_size, WHITE);
            }
        }

        string text = "SCORE " + to_string(score);

        DrawText(text.c_str(), width * cell_size + 40, 50, 30, GREEN);


        g.ltick();
        is_live = g.dtick(*current_figure, figure_pos);
        EndDrawing();

        g.del_ful_lines();

        tick++;
        if (tick > (fps * 50 / (IsKeyDown(KEY_DOWN) * 3 + 1)) && fps != 16) {
            temp = 1;
            fps *= 2;
            fps = min(fps, 16);
        }
    }
    cout << "Ha-Ha loser >_<\n" << "Score: " << score << endl;
    CloseWindow();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
