#include "view.hxx"


// static ge211::Color const closedCell = {173, 173, 173};
static ge211::Color const hoveredCell = {224, 224, 224};
// colors for each of the value indicator squares
// static ge211::Color const zeroC = {245, 245, 245};
// static ge211::Color const oneC = {48, 41, 255};
// static ge211::Color const twoC = {51, 150, 24};
// static ge211::Color const threeC = {242, 27, 34};
// static ge211::Color const fourC = {73, 2, 156};
// static ge211::Color const fiveC = {112, 4, 0};
// static ge211::Color const sixC = {187, 232, 39};
// static ge211::Color const sevenC = {66, 66, 65};
// static ge211::Color const eightC = {255, 92, 211};




View::View(Model const& model)
        : model_(model),
          closed(ge211::Image_sprite("closed_cell.png")),
          hovered(ge211::Rectangle_sprite({grid_size - 1, grid_size - 1},
                                          hoveredCell)),
          zero(ge211::Image_sprite("open_empty_cell.png")),
          one(ge211::Image_sprite("cell1.png")),
          two(ge211::Image_sprite("cell2.png")),
          three(ge211::Image_sprite("cell3.png")),
          four(ge211::Image_sprite("cell4.png")),
          five(ge211::Image_sprite("cell5.png")),
          six(ge211::Image_sprite("cell6.png")),
          seven(ge211::Image_sprite("cell7.png")),
          eight(ge211::Image_sprite("cell8.png")),
          valKey{zero, one, two, three, four, five, six, seven, eight},
          flag(ge211::Image_sprite("flag.png")),
          bomb(ge211::Image_sprite("bomb.png")),
          loss(ge211::Text_sprite("L", ge211::Font("sans.ttf", 300))),
          victory(ge211::Text_sprite("W", ge211::Font("sans.ttf", 300)))









{ }

void
View::draw(ge211::Sprite_set& set, Position mouse_posn)
{
    // This needs to do something!
    Position mouseP = screen_to_board(mouse_posn);
    // if (model_.gameOver()) {
    //     if (!model_.won()) {
    //         //game_lost(set);
    //         draw_cells(set, {-30, -30});
    //         //std::cout << "This is being run" << "\n";
    //     }else {
    //         draw_cells(set, mouseP);
    //     }
    // } else {
    //
    //     draw_cells(set, mouseP);
    // }
    if (model_.gameOver()) {
        // ge211::Text_sprite::Builder bL(ge211::Font("sans.ttf", 300));
        // ge211::Text_sprite::Builder bW(ge211::Font("sans.ttf", 300));
        //
        // bL.add_message("L");
        // bL.color(ge211::Color(235, 0, 0));
        //
        // bW.add_message("W");
        // bW.color(ge211::Color(17, 240, 17));
        // ge211::Text_sprite tL = bL.build();
        // ge211::Text_sprite tW=  bW.build();

        Position pL(((model_.dims().width * grid_size / 2) - 75), ((model_
        .dims().height * grid_size / 2) - 270));

        Position pW(((model_.dims().width * grid_size / 2) - 160), ((model_
                    .dims().height * grid_size / 2) - 250));

        if (!model_.won()) {
            set.add_sprite(loss, pL, 2);
        } else {
            set.add_sprite(victory, pW, 2);
        }
    }

    draw_cells(set, mouseP);

}


void
View::draw_cells(ge211::Sprite_set &set, Position mousePosn) {
    ge211::Transform tr;

    // if the game is already over, we just want to run either the win or
    // lose animations, depending on the result
    // if (model_.gameOver()) {
    //     if (model_.won()) {
    //         game_won(set);
    //     } else {
    //         game_lost(set);
    //     }
    //     return;
    // }

    for (int y = 0; y < rows; y++) {

        for (int x = 0; x < cols; x++) {
            // for every cell in table
            Cell c = model_.viewCell({x, y});

            Position cPosn = {c.getX(), c.getY()};
            // if the cell has already been opened:
            if (c.isOpen()) {

                if (c.isMine()) {
                    set.add_sprite(bomb, board_to_screen(cPosn), 0, tr
                    .set_scale((5.0 / 216.0)));
                } else {

                    int val = c.getVal();
                    set.add_sprite(valKey[val], board_to_screen(cPosn), 0, tr
                            .set_scale((5.0 / 216.0)));

                }


            } else {
                // if the cell HASN't been opened
                if (c.isFlagged()) {
                    set.add_sprite(flag, board_to_screen(cPosn), 1, tr
                            .set_scale((5.0 / 216.0)));
                } else if (cPosn == mousePosn) {
                    // if the mousePosn is the same square as the current
                    // cell, color it w hovered color
                    set.add_sprite(hovered, board_to_screen(cPosn), 1);

                } else {
                    // if the mouse is not hovering, just draw a regular
                    // closed box
                    //set.add_sprite(closed, board_to_screen(cPosn), 2);

                    // TEMP VIEW MINES HERE
                    // if (c.isMine()) {
                    //     //std::cout << "WE got one: " << c;
                    //     set.add_sprite(bomb, board_to_screen(cPosn), 2, tr
                    //             .set_scale((5.0 / 216)));
                    // }

                    set.add_sprite(closed, board_to_screen(cPosn), 1,
                                   tr.set_scale(5.0 /
                                   216.0));


                }


            }

        }
    }
}

// void
// View::game_lost(ge211::Sprite_set &set)
// {
//     int n = model_.getN();
//
//     // create a red square that says L: and then the # of mines they missed
//     ge211::Color red = {235, 0, 0};
//     ge211::Rectangle_sprite const bg = ge211::Rectangle_sprite({200, 80}, red);
//     set.add_sprite(bg,
//                    Position(((model_.dims().width* grid_size / 2) - 100),
//                     ((model_.dims().height * grid_size / 2) - 40)), 2);
//
//     //std::string message = "L: " + std::to_string(n);
//     // set.add_sprite(ge211::Text_sprite(message,
//     //                                   ge211::Font("sans.ttf", 72)),
//     //                {((model_.dims().width * grid_size / 2) - 100),
//     //                 ((model_.dims().height * grid_size / 2) - 40)}, 4);
//
// }

// void
// View::game_won(ge211::Sprite_set& set)
// {
//     // idk do something here too9oooo
// }




View::Position
View::board_to_screen(Model::Position p) {
    /*int rowCols = sqrt(model_.all_positions().dimensions().width);
    //background squares
    int wid = model_.all_positions().dimensions().width / rowCols;
    int hei = model_.all_positions().dimensions().height / rowCols;*/
    // location of top left of square
    return {(p.x * grid_size), (p.y * grid_size)};
}

View::Position
View::screen_to_board(Model::Position p) {
    /*int rowCols = sqrt(model_.all_positions().dimensions().width);
    //background squares
    int wid = model_.all_positions().dimensions().width / rowCols;
    int hei = model_.all_positions().dimensions().height / rowCols;*/
    // based on top left
    //make sure we got ints here
    int nX = p.x / grid_size;
    int nY = p.y / grid_size;

    return {nX, nY};
}
