#include "controller.hxx"
#include "view.hxx"
using namespace std;
Controller::Controller()
        : mouseP_{-30, -30},
          model_(),
          view_(model_)

{ }

Model::Dimensions
Controller::initial_window_dimensions() const
{
    return {this->model_.dims().width * grid_size, this->model_.dims().height
    * grid_size};
}

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set, this->mouseP_);

}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('c')) {
        if (!model_.gameOver()) {
            model_.flagCell(view_.screen_to_board(mouseP_));
        }
    }
    if (key == ge211::Key::code(' ')) {
        model_ = Model();
    }

}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int> mouse_posn)
{
    // want it to tell model to open the cell at mousePosn
    Position mouseBoardP = view_.screen_to_board(mouse_posn);
    if (!model_.gameOver()) {
        model_.cellOpen(mouseBoardP);
    }

}

void
Controller::on_mouse_move(ge211::Posn<int> p)
{
    this->mouseP_ = p;
}
