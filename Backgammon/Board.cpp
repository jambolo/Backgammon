#include "Board.h"

#include "types.h"

#include <cassert>
#include <numeric>
#include <string.h>
#include <vector>

class PositionSerializer
{
public:
    void push(int x)
    {
        if (i_ == 0)
            value_.push_back(0);
        uint8_t k = value_.back();
        k <<= 1;
        k  += (x & 1);
        value_.back() = k;
        i_ = (i_ + 1) % 8;
    }
    void push(int n, int x)
    {
        for (int j = 0; j < n; ++j)
        {
            push(x);
        }
    }

    std::vector<uint8_t> const & value() const { return value_; }

private:
    size_t i_ = 0;
    std::vector<uint8_t> value_;
};

static void serializeWhitesPosition(PositionSerializer * serializer, int const * board, int bar)
{
    for (int i = Board::SIZE - 1; i >= 0; --i)
    {
        int whiteCount = (board[i] < 0) ? -board[i] : 0;
        serializer->push(whiteCount, 1);
        serializer->push(0);
    }

    serializer->push(bar, 1);
    serializer->push(0);
}

static void serializeBlacksPosition(PositionSerializer * serializer, int const * board, int bar)
{
    for (int i = 0; i < Board::SIZE; ++i)
    {
        int blackCount = (board[i] > 0) ? board[i] : 0;
        serializer->push(blackCount, 1);
        serializer->push(0);
    }

    serializer->push(bar, 1);
    serializer->push(0);
}

Board::Board()
{
    clear();
}

void Board::initialize()
{
    clear();

    board_[0]  = -2;
    board_[5]  =  5;
    board_[7]  =  3;
    board_[11] = -5;
    board_[12] =  5;
    board_[16] = -3;
    board_[18] = -5;
    board_[23] =  2;
    assert(std::accumulate(std::begin(board_), std::end(board_), 0) == 0);
    assert(std::accumulate(std::begin(board_),
                           std::end(board_),
                           0,
                           [] (int a, int b) {
                               return std::abs(a) + std::abs(b);
                           }) == NUMBER_OF_CHECKERS);
}

std::string Board::positionId(Color color) const
{
    // 1. For every point around the board (starting at the ace point of the player on roll, continuing around to the 24 point and
    //    ending at the bar) :
    //      A. append as many 1s as the player on roll has on that point(if any).
    //      B. append a 0.
    // 2. For every point around the board (starting at the ace point of the opponent, continuing around to the opponent's 24 point
    //    and ending at the bar):
    //      A. append as many 1s as the opponent has on that point(if any).
    //      B. append a 0.
    // 3. Pad out the string to 80 bits with 0s.

    PositionSerializer serializer;
    if (color == Color::BLACK)
    {
        serializeBlacksPosition(&serializer, board_, bar_[(int)Color::BLACK]);
        serializeWhitesPosition(&serializer, board_, bar_[(int)Color::WHITE]);
    }
    else
    {
        serializeWhitesPosition(&serializer, board_, bar_[(int)Color::WHITE]);
        serializeBlacksPosition(&serializer, board_, bar_[(int)Color::BLACK]);
    }

    std::vector<uint8_t> key = serializer.value();

    // Pad to 80 bits
    key.resize(10);

    // Base-64 encode the key
    std::string id; // = Base64::encode(key);

    return id;
}

void Board::clear()
{
    memset(board_, 0, sizeof board_);
    bar_[(int)Color::BLACK] = 0;
    bar_[(int)Color::WHITE] = 0;
}

bool operator ==(Board const & x, Board const & y)
{
    return memcmp(x.board_, y.board_, sizeof x.board_) == 0;
}
