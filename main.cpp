#include "ComputerPlayer.h"

#include "Backgammon/Board.h"
#include "Backgammon/GameState.h"
#include "Backgammon/Move.h"
#include "Backgammon/types.h"

#include <nlohmann/json.hpp>

#include <cstdio>

using json = nlohmann::json;

#if !defined(NDEBUG)
static void drawBoard(Board const & b);
#endif

static constexpr char   OPTION_DEPTH_KEY[]      = "--depth=";
static constexpr size_t OPTION_DEPTH_KEY_LENGTH = sizeof(OPTION_DEPTH_KEY) - 1;

static int depth = 7;

int main(int argc, char ** argv)
{
    GameState s0;

    --argc;
    ++argv;
    while (argc > 0)
    {
        if (strncmp(*argv, OPTION_DEPTH_KEY, OPTION_DEPTH_KEY_LENGTH) == 0)
        {
            sscanf(*argv + OPTION_DEPTH_KEY_LENGTH, "%d", &depth);
        }
        else if (!s0.initializeFromId(*argv, *argv + 1))
        {
            fprintf(stderr, "Unable to parse input: %s %s\n", *argv, *argv + 1);
            exit(1);
        }
        --argc;
        ++argv;
    }

#if !defined(NDEBUG)
    drawBoard(s0.board_);
#endif

    ComputerPlayer computer(s0.whoseTurn_, depth);
    GameState      s1 = computer.myTurn(s0);
    printf("%s", s1.move_.notation().c_str());

#if !defined(NDEBUG)
    fprintf(stderr, "\n\n");

#if defined(ANALYSIS_PLAYER)
    json out = computer.analysisData_.toJson();
    fprintf(stderr, "%s\n", out.dump(4).c_str());
#endif
#endif

    return 0;
}

#if !defined(NDEBUG)
#define BLANK_ROW "|   |   |   |   |   |   ||   |   |   |   |   |   |"
static void drawBoard(Board const & b)
{
    fputs("13  14  15  16  17  18       19  20  21  22  23  24\n", stderr);
    fputs("+---+---+---+---+---+---||---+---+---+---+---+---+\n", stderr);

    for (int i = 0; i < 7; ++i)
    {
        char row[] = BLANK_ROW;
        for (int j = 13; j <= 23; ++j)
        {
            char c = (b[j] > 0) ? 'X' : 'O';
            int  n = std::abs(b[j]);
            int  x = ((j - 13) < 6) ? (j - 13) * 4 : (j - 19) * 4 + 29;
            if (n > 7)
                row[x] = (i > 0) ? c : '0' + (n - 6);
            else if (n > i)
                row[x] = c;
        }
        if (i == 4 && b.bar(Color::BLACK) > 0)
        {
            row[24] = (b.bar(Color::BLACK) >= 10) ? '1' : ' ';
            row[25] = '0' + b.bar(Color::BLACK) % 10;
        }
        fprintf(stderr, "    %s\n", row);
    }

    fputs(BLANK_ROW, stderr);
    fputs(BLANK_ROW, stderr);

    for (int i = 6; i >= 0; --i)
    {
        char row[] = BLANK_ROW;
        for (int j = 12; j >= 1; --j)
        {
            char c = (b[j] > 0) ? 'X' : 'O';
            int  n = std::abs(b[j]);
            int  x = ((12 - j) < 6) ? (12 - j) * 4 : (j - 19) * 4 + 29;
            if (n > 7)
                row[x] = (i > 0) ? c : '0' + (n - 6);
            else if (n > i)
                row[x] = c;
        }
        if (i == 4 && b.bar(Color::WHITE) > 0)
        {
            row[24] = (b.bar(Color::WHITE) >= 10) ? '1' : ' ';
            row[25] = '0' + b.bar(Color::WHITE) % 10;
        }
        fprintf(stderr, "    %s\n", row);
    }

    fputs("   +---+---+---+---+---+---||---+---+---+---+---+---+\n", stderr);
    fputs("   12  11  10  9   8   7        6   5   4   3   2   1\n", stderr);
}
#endif // !defined(NDEBUG)
