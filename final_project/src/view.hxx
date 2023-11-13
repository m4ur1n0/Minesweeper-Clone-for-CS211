#pragma once

#include "model.hxx"

static int const grid_size = 26;

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;
    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set, Position mouse_posn);



    Position board_to_screen(Position p);
    Position screen_to_board(Position p);

private:
    Model const& model_;
    void draw_cells(ge211::Sprite_set &set, Position mousePosn);
    //void game_lost(ge211::Sprite_set &set);
    //void game_won(ge211::Sprite_set &set);

    ge211::Image_sprite const closed;
    ge211::Rectangle_sprite const hovered;

    // one rect for each of the 9 possible values to signify which is which
    ge211::Image_sprite const zero;
    ge211::Image_sprite const one;
    ge211::Image_sprite const two;
    ge211::Image_sprite const three;
    ge211::Image_sprite const four;
    ge211::Image_sprite const five;
    ge211::Image_sprite const six;
    ge211::Image_sprite const seven;
    ge211::Image_sprite const eight;
    ge211::Image_sprite valKey[9];

    ge211::Image_sprite const flag;
    ge211::Image_sprite const bomb;

    ge211::Text_sprite const loss;
    ge211::Text_sprite const victory;



};


