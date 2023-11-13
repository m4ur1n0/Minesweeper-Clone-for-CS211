#include <ge211.hxx>


// board positions will be ints
using Position = ge211::Posn<int>;



class Cell {
public:


    Cell();
    explicit Cell(int lat, int lon);


    void openCell();
    void makeMine();
    void incrVal();
    void flag();
    void unFlag();


    int getVal() const {return val;}
    int getX() const {return x;}
    int getY() const {return y;}
    bool isMine() const {return mine;}
    bool isOpen() const {return open;}
    bool isFlagged() const {return flagged;}



private:
    int x;
    int y;
    int val;
    bool mine;
    bool open;
    bool flagged;
};

bool operator==(Cell const&, Cell const&);
std::ostream& operator<<(std::ostream&, Cell c);