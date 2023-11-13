#include "cell.hxx"

Cell::Cell() {
    Cell(-1, -1);
}
Cell::Cell(int lat, int lon)
    : x(lat),
      y(lon),
      val(0),
      mine(false),
      open(false),
      flagged(false)

{

}

void
Cell::openCell() {
    this->open = true;
    //return this->val;
}

void
Cell::makeMine()
{
    this->mine = true;
}

void
Cell::incrVal()
{
    (this->val)++;
}

void
Cell::flag()
{
    this->flagged = true;
}

void
Cell::unFlag()
{
    this->flagged = false;
}

bool
operator==(Cell const& c1, Cell const& c2) {
    if (c1.getX() == c2.getX() && c1.getY() == c2.getY()) {
        return true;
    }
    return false;

}

std::ostream& operator<<(std::ostream& os, Cell c) {
    return os << "----------------------\n" << "X: " << c.getX() << " - Y: " << c
    .getY() << "\nVAL: " << c.getVal() << "\nMINE: " << c.isMine() << "\nOPEN: "
    << c.isOpen()
    << "\n----------------------\n";
}


