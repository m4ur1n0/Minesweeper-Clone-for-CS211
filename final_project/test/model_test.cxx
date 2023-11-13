#include "model.hxx"
//#include "cell.hxx"
#include <catch.hxx>

std::vector<Position> getSurrounding(Position coords) {
    std::vector<Position> sur;
    Position upLeft(coords.x - 1, coords.y - 1);
    Position up(coords.x, coords.y - 1);
    Position upRight(coords.x + 1, coords.y - 1);
    Position left(coords.x - 1, coords.y);
    Position right(coords.x + 1, coords.y);
    Position downLeft(coords.x - 1, coords.y + 1);
    Position down(coords.x, coords.y + 1);
    Position downRight(coords.x + 1, coords.y + 1);
    Position ps[8] = {upLeft, up, upRight, left, right, downLeft, down, downRight};

    for (Position p : ps) {
        if (!((p.x < 0) || (p.y < 0) || (p.x >= 30) || (p.y >= 16))) {
            sur.push_back(p);
        }


    }

    return sur;

}

TEST_CASE("test 1: specs 1 & 2")
{
    Model model = Model();

    // SPECS 1 AND 2
    int bombCount = 0;
    int expectedBombs = 99;
    int cellCount = 0;
    int expectedCells = (16 * 30);
    // assert that there are (30 * 16) cells, and that 99 of them are bombs
    for (int y = 0; y < model.dims().height; y++) {
        // for each row in the grid
        for (int x = 0; x < model.dims().width; x++) {
            // for each cell in each row

            Cell c = model.viewCell({x, y});
            // good lord empty cell is such a stupid fucking name for what
            // its purpose is why did I name it this
            if (!(c == model.emptyCell)) {
                // checking to make sure that there actually is a cell at
                // this location in the cells matrix, i.e. if this position
                // doesn't evaluate to empty cell, it exists, and we can
                // increment cellCount and check bomb status
                cellCount++;
                if (c.isMine()) {
                    bombCount++;
                }
            }

        }
    }

    CHECK(bombCount == expectedBombs);
    CHECK(cellCount == expectedCells);

    // prove that going any further would result in nonexistant cells
    Cell c = model.viewCell({model.dims().width, model.dims().height});
    // viewCell evaluates to emptyCell when the outside bounds of the cells
    // matrix
    CHECK(c == model.emptyCell);
}

TEST_CASE("test 2: spec 3") {

    Model model = Model();
    // SPECS 3 -- check that every cell's value (except mines cause they dont
    // matter) corresponds exactly to the number of bombs in the surrounding
    // 8 squares
    for (int y = 0; y < model.dims().height; y++) {
        for (int x = 0; x < model.dims().width; x++) {
            // for every cell in the map that ISN'T a mine,
            Cell curr = model.viewCell({x, y});
            if (!(curr.isMine())) {
                // get the surrounding valid cells, iterate through them,
                // counting the ones that are bombs, and then make sure the #
                // of bombs is equal to that non-bomb cell's value
                std::vector<Position> surrounding = getSurrounding({x, y});
                int val = curr.getVal();
                int bombs = 0;
                for (Position p : surrounding) {
                    if (model.viewCell(p).isMine()) {
                        bombs++;
                    }
                }

                CHECK(val == bombs);

            }
        }
    }




}

TEST_CASE("Test 3: opening cells? spec 7 sorta ") {

    Model model = Model();



    //- [ ] if the box in this position has been opened
    //     - [ ] If it is empty: do nothing
    //     - [ ] If it has a number, check to see if there are x flagged cells in
    //          the 8 squares surrounding it, where x is equal to the cell’s value.
//              If these flags exist, open up every other unopened cell in the
//              surrounding squares
    // - [ ] if the box in this position is yet closed, mark that cell object open,
    //       and stop drawing it, instead drawing the corresponding open_cell
    //       graphic, either:
    //     - [ ] Bomb
    //     - [ ] Number cell (corresponding to value)
    //     - [ ] Or empty cell
    //         - [ ] When an empty cell is open, open the 8 squares surrounding
    //               it, unless those squares are outside the board dimensions

    for (int y = 0; y < model.dims().height; y++) {
        for (int x = 0; x < model.dims().width; x++) {
            // p is the position we're on and curr is a view-only version of
            // the cell at that osition
            Position p = {x, y};
            Cell curr = model.viewCell(p);

            if (curr.isMine()) {
                if (!curr.isFlagged()) {
                    model.flagCell(p);
                }
                model.cellOpen(p);
                Cell updatedCurr = model.viewCell(p);
                // now cell should still not be open, because it was flagged,
                // and flagged cells shouldn't open.
                CHECK(!updatedCurr.isOpen());
                // now we unflag the cell so that at the end of this for loop
                // every cell will be open except the mines
                model.flagCell(p);

            } else if ((curr.getVal() == 0) && (!curr.isOpen())) {
                // want to assert that whenever an empty cell is opened, so
                // are the 8 cells surrounding it
                model.cellOpen(p);
                Cell updatedCurr = model.viewCell(p);
                CHECK(updatedCurr.isOpen());

                std::vector<Position> surrounding = getSurrounding(p);
                for (Position pos : surrounding) {
                    // for every cell surrounding the open cell we just opened,
                    Cell surroundingCell = model.viewCell(pos);
                    CHECK(surroundingCell.isOpen());
                }

            } else if (curr.isOpen()) {
                // if the cell is already open, cellOpen should
                // A) do nothing if the cell has a value of 0, i.e. if it is
                // just empty open cell
                // B) if the cell has a value greater than 0, then it should
                // open its surrounding cells IF AND ONLY IF the surrounding
                // cells contain x flagged cells where x is exactly equal to
                // the value of the cell (which we will manufacture for this
                // scenario)

                if (curr.getVal() > 0) {
                    std::vector<Position> surrounding = getSurrounding(p);

                    // initially on this map we're playing with nothing
                    // should be flagged, since we unflagged every bomb we tried
                    // so cellOpen()ing a open cell w val>0 should actually
                    // do nothing

                    int closedCells = 0;
                    int mines = 0;
                    for (Position pos : surrounding) {
                        Cell surrounded = model.viewCell(pos);
                        if (surrounded.isMine()) {
                            mines++;
                            model.flagCell(pos);
                        }
                        if (!surrounded.isOpen()) {
                            closedCells++;
                        }

                    }

                    // at this point every bomb around the number is flagged,
                    // and any other closed cells are just closed and existing
                    // theoretically there could be 5 closed cells around and
                    // 2 bombs, but after calling cellOpen to curr the only
                    // closed cells surrounding should be the flagged ones,
                    // which we said will just be the bombs. so we count again
                    model.cellOpen(p);

                    int closedCells2 = 0;
                    int mines2 = 0;
                    for (Position pos : surrounding) {
                        Cell surrounded = model.viewCell(pos);
                        if (surrounded.isMine()) {
                            mines2++;
                            model.flagCell(pos);
                        }
                        if (!surrounded.isOpen()) {
                            closedCells2++;
                        }

                    }
                    CHECK(mines2 == closedCells2);





                }


            }

        }
    }


}

TEST_CASE("test 4: opening bombs kills game and open all other bombs (spec "
          "9?)") {
    // get a map i know i'm familiar with
    srand(5);
    Model model = Model();

    // if this is being tested feel free to run the game with "TEMPORARY VIEW
    // MINES" uncommented in view and a seed of 5 so you can see how all of
    // this will work, but if you prefer not to, just trust that I have a
    // printed out minesweeper map sitting in front of me labeled with
    // coordinates on the side like a game of battleship so that I can make
    // specific calls to open specific cell like a mine and assert that this
    // does what its supposed to.

    CHECK(!model.gameOver());
    CHECK(!model.won());

    // with a seed of 5 there should be a bomb at 14, 5. there are tons obv i
    // just picked this one cause its pretty central
    // first we make sure every cell is closed as its meant to be at the start,
    for (int y = 0; y < model.dims().height; y++) {
        for (int x = 0; x < model.dims().width; x++) {
            // for every single cell
            Position p = {x, y};
            Cell curr = model.viewCell(p);
            CHECK(!curr.isOpen());

        }
    }

    // now we flag 5 bombs, to prove that these 5 stay un-opened after losing
    model.flagCell({4, 4});
    model.flagCell({4, 5});
    model.flagCell({4, 6});
    model.flagCell({4, 7});
    model.flagCell({23, 6});

    CHECK(model.viewCell({4, 4}).isMine());
    CHECK(model.viewCell({4, 5}).isMine());
    CHECK(model.viewCell({4, 6}).isMine());
    CHECK(model.viewCell({4, 7}).isMine());
    CHECK(model.viewCell({23, 6}).isMine());

    // now we open a mine
    model.cellOpen({14, 5});

    // NOW THE game has been killed so the game_over bool is true, but we
    // lost the game (bomb) so the win bool is still false;
    CHECK(model.gameOver());
    CHECK(!model.won());

    // check that losing the game opens every mine so you can weep with
    // terror at the sight of your incompetence :/

    int flaggedCount = 0;
    for (int y = 0; y < model.dims().height; y++) {
        for (int x = 0; x < model.dims().width; x++) {
            // for every cell, if its a bomb, check that its open
            Position p = {x, y};
            Cell curr = model.viewCell(p);
            if (curr.isMine()) {
                if (!curr.isOpen()) {
                    flaggedCount++;
                } else {
                    CHECK(curr.isOpen());
                }
            }

        }
    }

    // only five of the bombs should have been left unopened since we only
    // flagged 5 bombs
    CHECK(flaggedCount == 5);

    CHECK(!model.viewCell({4, 4}).isOpen());
    CHECK(!model.viewCell({4, 5}).isOpen());
    CHECK(!model.viewCell({4, 6}).isOpen());
    CHECK(!model.viewCell({4, 7}).isOpen());
    CHECK(!model.viewCell({23, 6}).isOpen());


}

TEST_CASE("test 5 game is winnable?") {
    Model model = Model();

    CHECK(!model.gameOver());
    CHECK(!model.won());

    // won and gameOver should both evaluate to false since neither is the
    // case, but if we go through and flag every bomb and then open every
    // non-bomb, both should evaluate to true;

    for (int y = 0; y < model.dims().height; y++) {
        for (int x = 0; x < model.dims().width; x++) {
            Position p = {x, y};
            Cell curr = model.viewCell(p);
            if (curr.isMine()) {
                model.flagCell(p);
            }

        }
    }

    for (int y = 0; y < model.dims().height; y++) {
        for (int x = 0; x < model.dims().width; x++) {
            Position p = {x, y};

            model.cellOpen(p);

        }
    }

    CHECK(model.gameOver());
    CHECK(model.won());

}