#pragma once

#include <ge211.hxx>
#include "cell.hxx"
#include <iostream>

static int const rows = 16;
static int const cols = 30;
class Model
{
public:
    Model();
    // board dimensions will use int coords
    using Dimensions = ge211::Dims<int>;

    // board positions will be ints
    using Position = ge211::Posn<int>;



    Cell emptyCell;
    Dimensions dims() const;
    Cell cells[rows][cols];

    Cell& cellAt(Position posn);
    void makeBomb(Position posn);
    void cellOpen(Position posn);
    void flagCell(Position posn);
    bool gameOver() const {return is_game_over;}
    bool won() const {return win;}
    Cell viewCell(Position posn) const;
    //int getN() const {return n;}

    void check_game_won();

    void openNearby(Position p);



private:
    Dimensions dims_;
    bool is_game_over;
    bool win;
    //int n;

    void killGame(bool winned);




};
