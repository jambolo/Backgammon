#include "Board.h"

#include "types.h"

#include <cassert>
#include <Misc/Base64.h>
#include <numeric>
#include <string.h>
#include <vector>

class PositionSerializer
{
public:
    // Pushes a single bit (0 or 1)
    void push(int x)
    {
        if (index_ == 0)
            value_.push_back(0);
        uint8_t k = value_.back();
        k <<= 1;
        k  += (x & 1);
        value_.back() = k;
        index_ = (index_ + 1) % 8;
    }

    // Pushes multiple 0s or multiple 1s
    void push(int n, int x)
    {
        for (int j = 0; j < n; ++j)
        {
            push(x);
        }
    }

    std::vector<uint8_t> const & value() const { return value_; }

private:
    size_t index_ = 0;
    std::vector<uint8_t> value_;
};

class PositionDeserializer
{
public:
    PositionDeserializer(std::vector<uint8_t> const & v)
        : value_(v)
    {
    }

    int pop()
    {
        if (index_ >= value_.size() * 8)
            return 0;

        int b = (value_[index_ / 8] >> (index_ % 8)) & 1;
        ++index_;
        return b;
    }

    int popOnes()
    {
        int count = 0;
        for (int b = pop(); b != 0; b = pop())
        {
            ++count;
        }
        return count;
    }

private:
    size_t index_ = 0;
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

static int deserializeBlacksPosition(PositionDeserializer * deserializer, int * board, int * bar)
{
    int total = 0;
    for (int i = 0; i < Board::SIZE; ++i)
    {
        int n = deserializer->popOnes();
        board[i] = n;
        total   += n;
    }
    *bar = deserializer->popOnes();
    return total + *bar;
}

static int deserializeWhitesPosition(PositionDeserializer * deserializer, int * board, int * bar)
{
    int total = 0;
    for (int i = Board::SIZE - 1; i >= 0; --i)
    {
        int n = deserializer->popOnes();
        board[i] = -n;
        total   += n;
    }
    *bar = deserializer->popOnes();
    return total + *bar;
}

Board::Board()
{
    clear();
}

Board::Board(Color color, char const * positionId)
{
    clear();
    std::string id(positionId);
    assert(id.size() == 14);
    id += "==";
    std::string key = Base64::decode(id);

    PositionDeserializer deserializer(std::vector<uint8_t>(key.begin(), key.end()));
    if (color == Color::BLACK)
    {
        off_[(int)Color::BLACK] = deserializeBlacksPosition(&deserializer, board_, &bar_[(int)Color::BLACK]);
        off_[(int)Color::WHITE] = deserializeWhitesPosition(&deserializer, board_, &bar_[(int)Color::WHITE]);
    }
    else
    {
        off_[(int)Color::WHITE] = deserializeWhitesPosition(&deserializer, board_, &bar_[(int)Color::WHITE]);
        off_[(int)Color::BLACK] = deserializeBlacksPosition(&deserializer, board_, &bar_[(int)Color::BLACK]);
    }
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

void Board::add(Color color, int to)
{
    if (to == OFF)
    {
        assert(off_[(int)color] < NUMBER_OF_CHECKERS);
        ++off_[(int)color];
    }
    else if (to == BAR)
    {
        assert(bar_[(int)color] < NUMBER_OF_CHECKERS);
        ++bar_[(int)color];
    }
    else
    {
        --to;
        if (color == Color::BLACK)
        {
            assert(board_[to] >= 0);
            ++board_[to];
        }
        else
        {
            assert(board_[to] <= 0);
            --board_[to];
        }
    }
}

void Board::remove(Color color, int from)
{
    if (from == OFF)
    {
        assert(off_[(int)color] > 0);
        --off_[(int)color];
    }
    else if (from == BAR)
    {
        assert(bar_[(int)color] > 0);
        --bar_[(int)color];
    }
    else
    {
        --from;
        if (color == Color::BLACK)
        {
            assert(board_[from] > 0);
            --board_[from];
        }
        else
        {
            assert(board_[from] < 0);
            ++board_[from];
        }
    }
}

void Board::move(Color color, int from, int to)
{
    remove(color, from);
    add(color, to);
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
    std::string id = Base64::encode(key.data(), key.size());
    assert(id.size() == 16);

    // Return the id without the trailing "==". The ID is always 14 characters.
    return id.substr(0, 14);
}

void Board::clear()
{
    memset(board_, 0, sizeof board_);
    bar_[(int)Color::BLACK] = 0;
    bar_[(int)Color::WHITE] = 0;
    off_[(int)Color::BLACK] = 0;
    off_[(int)Color::WHITE] = 0;
}

bool operator ==(Board const & x, Board const & y)
{
    return memcmp(x.board_, y.board_, sizeof x.board_) == 0;
}
