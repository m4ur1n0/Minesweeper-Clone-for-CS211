#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    Controller();
    Model::Dimensions initial_window_dimensions() const override;
    Model::Position mouseP_;

    /// Defines how the game responds to key events. There are two keys
    /// recognized:
    ///
    ///  - On 'q', quits.
    ///
    ///  - On ' ', tells the model to launch the ball.
    void on_key(ge211::Key) override;

    /// Defines how the game responds to mouse clicks (which is by
    /// telling the model to launch the ball).
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;

    /// Defines how the game responds to mouse motion (which is by
    /// telling the model to change the x coordinate of the paddle).
    void on_mouse_move(ge211::Posn<int>) override;


protected:
    void draw(ge211::Sprite_set& set) override;

private:
    Model model_;
    View view_;
};
