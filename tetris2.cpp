#include <iostream>
#include <vector>
#include <thread>
#include <ncurses.h>
#include <random>
#include <memory>
#include <chrono>

class fond {
public:
    int hauteur;
    int largeur;
    std::vector<std::vector<int>> bg;

    std::vector<std::vector<int>> backgroundSetup(const int &nx, const int &ny) {
        std::vector<std::vector<int>> bg(ny, std::vector<int>(nx, 0));
        for (int i = 0; i < nx; i++) {
            bg[0][i] = 1;
            bg[ny - 1][i] = 1;
        }
        for (int i = 0; i < ny; i++) {
            bg[i][0] = 1;
            bg[i][nx - 1] = 1;
        }
        return bg;
    }

    fond() {
        hauteur = 30;
        largeur = 10;
        bg = backgroundSetup(largeur, hauteur);
    }

    void print() {
        for (int j = 0; j < hauteur; j++) {
            for (int i = 0; i < largeur; i++) {
                if (bg[j][i] == 1) {
                    printw("# ");
                } else if (bg[j][i] == 0) {
                    printw("  ");
                } else if (bg[j][i] == 2) {
                    printw("O ");
                }
            }
            printw("\n");
        }
    }

    void placepiece(const class piece &A);
    void removepiece(const class piece &A);
    void clearFullLines();
};

class piece {
protected:
    std::vector<std::vector<int>> forme;
    int x, y;

public:
    piece(int x_, int y_) : x(x_), y(y_) {}
    virtual ~piece() {}
    virtual void initialiseforme() = 0;

    const std::vector<std::vector<int>> &getForme() const {
        return forme;
    }

    int getx() const { return x; }
    int gety() const { return y; }

    void setposition(int x_, int y_) {
        x = x_;
        y = y_;
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    bool canMove(int dx, int dy, const fond &bg) {
        const auto &forme = getForme();
        for (int i = 0; i < forme.size(); i++) {
            for (int j = 0; j < forme[i].size(); j++) {
                if (forme[i][j] == 2) {
                    int newX = x + j + dx;
                    int newY = y + i + dy;
                    if (newX < 0 || newX >= bg.largeur || newY < 0 || newY >= bg.hauteur || bg.bg[newY][newX] != 0) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void rotate() {
        std::vector<std::vector<int>> newForme(forme[0].size(), std::vector<int>(forme.size()));
        for (size_t i = 0; i < forme.size(); i++) {
            for (size_t j = 0; j < forme[0].size(); j++) {
                newForme[j][forme.size() - 1 - i] = forme[i][j];
            }
        }
        forme = newForme;
    }

    bool canRotate(const fond &bg) {
        std::vector<std::vector<int>> newForme(forme[0].size(), std::vector<int>(forme.size()));
    
        for (size_t i = 0; i < forme.size(); i++) {
            for (size_t j = 0; j < forme[0].size(); j++) {
                newForme[j][forme.size() - 1 - i] = forme[i][j];
            }
        }
    
        for (size_t i = 0; i < newForme.size(); i++) {
            for (size_t j = 0; j < newForme[i].size(); j++) {
                if (newForme[i][j] == 2) {
                    int newX = x + j;
                    int newY = y + i;
                    if (newX < 0 || newX >= bg.largeur || newY < 0 || newY >= bg.hauteur || bg.bg[newY][newX] != 0) {
                        return false;
                    }
                }
            }
        }
    
        return true;
    }
};

class pieceT : public piece {
public:
    pieceT(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {0, 2, 0},
            {2, 2, 2}
        };
    }
};

class pieceI : public piece {
    public:
        pieceI(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {2},
            {2},
            {2},
            {2}
        };
    }
};

class pieceL : public piece {
    public:
        pieceL(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {2, 0},
            {2, 0},
            {2, 2}
        };
    }
};

class pieceJ : public piece {
    public:
        pieceJ(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {0, 2},
            {0, 2},
            {2, 2}
        };
    }
};

class pieceS : public piece {
    public:
        pieceS(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {0, 2, 2},
            {2, 2, 0}
        };
    }
};

class pieceZ : public piece {
    public:
        pieceZ(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {2, 2, 0},
            {0, 2, 2}
        };
    }
};

class pieceO : public piece {
    public:
        pieceO(int x, int y) : piece(x, y) {
        initialiseforme();
    }

    void initialiseforme() override {
        forme = {
            {2, 2},
            {2, 2}
        };
    }
};

void fond::placepiece(const piece &A) {
    const auto &forme = A.getForme();
    int x = A.getx();
    int y = A.gety();
    for (int i = 0; i < forme.size(); i++) {
        for (int j = 0; j < forme[i].size(); j++) {
            if (forme[i][j] != 0) {
                bg[y + i][x + j] = forme[i][j];
            }
        }
    }
}

void fond::removepiece(const piece &A) {
    const auto &forme = A.getForme();
    int x = A.getx();
    int y = A.gety();
    for (int i = 0; i < forme.size(); i++) {
        for (int j = 0; j < forme[i].size(); j++) {
            if (forme[i][j] == 2) {
                bg[y + i][x + j] = 0;
            }
        }
    }
}


void fond::clearFullLines() {
    for (int i = hauteur - 2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < largeur - 1; j++) {
            if (bg[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                bg[k] = bg[k - 1];
            }
            bg[0] = std::vector<int>(largeur, 0);
            bg[0][0] = bg[0][largeur - 1] = 1;
            i++;
        }
    }
}

int main() {
    fond background;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    timeout(100);

    while (true) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 7);

        int random_number = distr(gen);

        std::unique_ptr<piece> piece;
        switch (random_number) {
            case 1: piece = std::make_unique<pieceT>(5, 1); break;
            case 2: piece = std::make_unique<pieceI>(5, 1); break;
            case 3: piece = std::make_unique<pieceL>(5, 1); break;
            case 4: piece = std::make_unique<pieceJ>(5, 1); break;
            case 5: piece = std::make_unique<pieceO>(5, 1); break;
            case 6: piece = std::make_unique<pieceS>(5, 1); break;
            case 7: piece = std::make_unique<pieceZ>(5, 1); break;
        }

        while (true) {
            clear();  
            background.placepiece(*piece);
            background.print();
            background.removepiece(*piece);

            int ch = getch();
            if (ch == 'a' && piece->canMove(-1, 0, background)) {
                piece->move(-1, 0);
            } else if (ch == 'd' && piece->canMove(1, 0, background)) {
                piece->move(1, 0);
            } else if (ch == 'w' && piece->canRotate(background)) {
                piece ->rotate();
            }

            if (!piece->canMove(0, 1, background)) {
                background.placepiece(*piece);
                break;
            }

            piece->move(0, 1);
            refresh();  
            background.clearFullLines();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    endwin();
    return 0;
}