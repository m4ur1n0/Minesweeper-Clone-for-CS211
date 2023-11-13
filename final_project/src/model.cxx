#include "model.hxx"
#include <ge211.hxx>
#include <cstdlib>



Model::Model()
  :     emptyCell(Cell(-1, -1)),
        dims_(Model::Dimensions(cols, rows)),
        is_game_over(false),
        win(false)
        //cells(){
{


    for (int y = 0; y < rows; y++) {
        // for every row in the grid
        for (int x = 0; x < cols; x++) {
            // for each individual cell in the row
            // add to the cells vector a new Cell obj with
            // lat of x and lon of y
            this->cells[y][x] = Cell(x, y);


        }
    }
    // at the end of this we should have a 2d array of cells where their lat,
    // lon is the same as their x, y

    int n = 0;
    while (n < 99) {
        int randX = rand() % cols;
        int randY = rand() % rows;
        Cell& opCel = cellAt({randX, randY});
        // after getting a random x and y between 0 and 29 or 0 and 15
        // respectively, it makes sure that cell isn't already a bomb, and if
        // it isn't, it makes it a bomb and increases surrounding vals
        if (!opCel.isMine()){
            makeBomb({randX, randY});
            //std::cout << "BOMB MADE @ {" << randX << ", " << randY << "}\n";
            // only increases n if we actually make a bomb, so we can be sure
            // to get 99 bombs
            n++;
        }


    }

    // for (int y = 0; y < rows; y++) {
    //     // for every row in the grid
    //     for (int x = 0; x < cols; x++) {
    //         // for each individual cell in the row
    //         // add to the cells vector a new Cell obj with
    //         // lat of x and lon of y
    //         std::cout << this->cells[y][x];
    //
    //
    //     }
    // }
    // cells[2][5].makeMine();
    // std::cout << cells[2][5];


}

Model::Dimensions
Model::dims() const
{
    return this->dims_;
}

Cell&
Model::cellAt(Model::Position posn)
{
    // IMPORTANT: this funciton assumes it is getting BOARD COORDINATES
    // which are unique for cells, and can be used to identify them
    if ((posn.x < cols && posn.y < rows) && (posn.x >= 0 && posn.y >= 0)) {
        return cells[posn.y][posn.x];
    }

    // if a posn gets passed which somehow doesn't have a corresponding cell
    // we return a ref to a cell with x and y of -1, so it can be clear its
    // not real
    return emptyCell;
}

void
Model::makeBomb(Position coords)
{
    // this one will take a cell's indices, find the cell, and assign its mine
    // value to true, unless the cell is a mine already, then it goes to all
    // cells in the surrounding areas and increases their value by 1, because
    // now they are touching a bomb

    // FUNCTION ASSUMES IT IS GETTING BOARD COORDINATES
    Cell& c = cellAt(coords);
    if (c == emptyCell) {
        return;
    }
    if (c.isMine()) {
        return;
    }

    c.makeMine();

    Position upLeft(coords.x - 1, coords.y - 1);
    Position up(coords.x, coords.y - 1);
    Position upRight(coords.x + 1, coords.y - 1);
    Position left(coords.x - 1, coords.y);
    Position right(coords.x + 1, coords.y);
    Position downLeft(coords.x - 1, coords.y + 1);
    Position down(coords.x, coords.y + 1);
    Position downRight(coords.x + 1, coords.y + 1);

    Position surrounding[8] = {upLeft, up, upRight, left, right, downLeft,
                               down, downRight};

    for (Position p : surrounding) {
        // for every position surrounding the cell we just made a bomb,
        // increase the val of the cells surrounding by 1
        if (!((p.x >= cols) || (p.x < 0) || (p.y >= rows) || (p.y < 0))) {
            cellAt(p).incrVal();
        }
    }

}

void
Model::cellOpen(Model::Position coords)
{
    // THIS FUNCTION EXPECTS BOARD COORDS NOT SCREEN COORDS
    Cell& c = cellAt(coords);
    if (c.isOpen() && (c.getVal() != 0)) {
        openNearby(coords);
        return;
    }

    if (c.isFlagged()) {
        return;
    }

    if (c.isMine()) {
        killGame(false);
        c.openCell();
        return;
    }
    c.openCell();
    check_game_won();

    // after that cell is deemed open, if it is a 0 (no bombs around it) then
    // we have to go ahead and cellOpen every cell around it
    if (c.getVal() != 0) {
        return;
    }
    // same as before, we define the 8 surrounding positions as their own
    // list of vars
    Position upLeft(coords.x - 1, coords.y - 1);
    Position up(coords.x, coords.y - 1);
    Position upRight(coords.x + 1, coords.y - 1);
    Position left(coords.x - 1, coords.y);
    Position right(coords.x + 1, coords.y);
    Position downLeft(coords.x - 1, coords.y + 1);
    Position down(coords.x, coords.y + 1);
    Position downRight(coords.x + 1, coords.y + 1);

    Position surrounding[8] = {upLeft, up, upRight, left, right, downLeft,
                               down, downRight};

    for (Position p : surrounding) {
        if ((!cellAt(p).isOpen()) && (!cellAt(p).isMine())) {
            // if the surrounding cell is not already open, then it
            // recursively calls cellOpen on it, meaning cells with val 0
            // will continue to open up surrounding cells, but cells with a
            // val higher than 0 will not...
            cellOpen(p);

        }
    }

    check_game_won();


}

void
Model::flagCell(Model::Position posn)
{
    Cell& c = cellAt(posn);
    if (c.isFlagged()) {
        c.unFlag();
    } else {
        c.flag();
    }



}

void
Model::killGame(bool winned)
{
    this->is_game_over = true;
    this->win = winned;
    if (!winned) {
        // n is going to count the unflagged bombs as we open them so that we can
        // share our loser's big loss score with them

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                // for each cell in the table, get a ref to it
                Cell& c = cellAt({x, y});
                if ((!c.isFlagged()) && (c.isMine())) {
                    // if the ref is not flagged and is a mine then open it
                    // so that the mine will be shown
                    c.openCell();
                    //this->n++;
                    //std::cout << "Made mine # " << n << " open: " << c.isOpen
                    //() << "\n";
                }

            }
        }
    }



}

Cell
Model::viewCell(Model::Position posn) const
{
    // IMPORTANT: this funciton assumes it is getting BOARD COORDINATES
    // which are unique for cells, and can be used to identify them
    if ((posn.x < cols && posn.y < rows) && (posn.x >= 0 && posn.y >= 0)) {
        return cells[posn.y][posn.x];
    }

    // if a posn gets passed which somehow doesn't have a corresponding cell
    // we return a ref to a cell with x and y of -1, so it can be clear its
    // not real
    return emptyCell;
}

void
Model::check_game_won()
{
    if (!is_game_over) {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                Cell c = viewCell({x, y});
                if ((!c.isOpen()) && (!c.isFlagged())) {
                    return;
                }
            }
        }


        killGame(true);
    }

}

void
Model::openNearby(Model::Position coords)
{
    // same as before, we define the 8 surrounding positions as their own
    // list of vars
    Position upLeft(coords.x - 1, coords.y - 1);
    Position up(coords.x, coords.y - 1);
    Position upRight(coords.x + 1, coords.y - 1);
    Position left(coords.x - 1, coords.y);
    Position right(coords.x + 1, coords.y);
    Position downLeft(coords.x - 1, coords.y + 1);
    Position down(coords.x, coords.y + 1);
    Position downRight(coords.x + 1, coords.y + 1);

    Position surrounding[8] = {upLeft, up, upRight, left, right, downLeft,
                               down, downRight};

    Cell curr = viewCell(coords);
    if (curr == emptyCell) {
        return;
    }
    int value = curr.getVal();
    int count = 0;
    for (Position p : surrounding) {
        Cell near = viewCell(p);
        if (!(near == emptyCell)) {

            if (near.isFlagged()) {
                count++;
                //std::cout << "counting here \n";
            }
        }
    }

    if (count == value) {
        for (Position p : surrounding) {
            Cell current = viewCell(p);
            if (!(current == emptyCell)) {
                if (!current.isOpen()) {
                    cellOpen(p);
                }
            }

        }
    }

}


