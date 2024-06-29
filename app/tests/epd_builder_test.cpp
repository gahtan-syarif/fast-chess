#include <epd/epd_builder.hpp>

#include "doctest/doctest.hpp"

namespace fast_chess {
TEST_SUITE("EPD Builder Tests") {
    TEST_CASE("EPD Creation") {
        MatchData match_data;
        match_data.players.first.config.name = "engine1";
        match_data.players.first.color       = chess::Color::WHITE;
        match_data.players.first.result      = chess::GameResult::WIN;

        match_data.players.second.config.name = "engine2";
        match_data.players.second.color       = chess::Color::BLACK;
        match_data.players.second.result      = chess::GameResult::LOSE;

        match_data.moves = {MoveData("e2e4", "+1.00", 1321, 15, 4, 0, 0),
                            MoveData("e7e5", "+1.23", 430, 15, 3, 0, 0),
                            MoveData("g1f3", "+1.45", 310, 16, 24, 0, 0),
                            MoveData("g8f6", "+10.15", 1821, 18, 7, 0, 0)};

        match_data.fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        match_data.reason = "engine2 got checkmated";

        options::Tournament options;
        options.site = "localhost";

        std::string expected = "rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - hmvc 2; fmvn 3;\n";

        epd::EpdBuilder epd_builder = epd::EpdBuilder(match_data, options);
        CHECK(epd_builder.get() == expected);
    }

    TEST_CASE("EPD Creation Black Win") {
        MatchData match_data;
        match_data.players.first.config.name = "engine1";
        match_data.players.first.color       = chess::Color::WHITE;
        match_data.players.first.result      = chess::GameResult::LOSE;

        match_data.players.second.config.name = "engine2";
        match_data.players.second.color       = chess::Color::BLACK;
        match_data.players.second.result      = chess::GameResult::WIN;

        match_data.moves = {MoveData("e2e4", "+1.00", 1321, 15, 4, 0, 0),
                            MoveData("e7e5", "+1.23", 430, 15, 3, 0, 0),
                            MoveData("g1f3", "+1.45", 310, 16, 24, 0, 0),
                            MoveData("g8f6", "+10.15", 1821, 18, 7, 0, 0)};

        match_data.fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        match_data.reason = "engine1 got checkmated";

        options::Tournament options;
        options.site = "localhost";

        std::string expected = "rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - hmvc 2; fmvn 3;\n";

        epd::EpdBuilder epd_builder = epd::EpdBuilder(match_data, options);
        CHECK(epd_builder.get() == expected);
    }

    TEST_CASE("EPD Creation Black Start") {
        MatchData match_data;
        match_data.players.first.config.name = "engine1";
        match_data.players.first.color       = chess::Color::BLACK;
        match_data.players.first.result      = chess::GameResult::NONE;

        match_data.players.second.config.name = "engine2";
        match_data.players.second.color       = chess::Color::WHITE;
        match_data.players.second.result      = chess::GameResult::NONE;

        match_data.moves = {MoveData("e8g8", "+1.00", 1321, 15, 4, 0, 0),
                            MoveData("e1g1", "+1.23", 430, 15, 3, 0, 0),
                            MoveData("a6c5", "+1.45", 310, 16, 24, 0, 0)};

        match_data.fen = "r2qk2r/1bpp2pp/n3pn2/p2P1p2/1bP5/2N1BNP1/1PQ1PPBP/R3K2R b KQkq - 0 1";

        match_data.reason = "aborted";

        options::Tournament options;
        options.site = "localhost";

        std::string expected = R"([Event "Fast-Chess Tournament"]
[Site "localhost"]
[Round "1"]
[White "engine2"]
[Black "engine1"]
[Result "*"]
[SetUp "1"]
[FEN "r2qk2r/1bpp2pp/n3pn2/p2P1p2/1bP5/2N1BNP1/1PQ1PPBP/R3K2R b KQkq - 0 1"]
[PlyCount "3"]
[TimeControl "-"]

1... O-O {+1.00/15, 1.321s} 2. O-O {+1.23/15, 0.430s}
Nc5 {+1.45/16, 0.310s, aborted} *

)";

        epd::EpdBuilder epd_builder = epd::EpdBuilder(match_data, options);
        CHECK(epd_builder.get() == expected);
    }

    TEST_CASE("EPD Creation Fixed Time per Move") {
        MatchData match_data;
        match_data.players.first.config.name                = "engine1";
        match_data.players.first.color                      = chess::Color::BLACK;
        match_data.players.first.result                     = chess::GameResult::NONE;
        match_data.players.first.config.limit.tc.fixed_time = 1000;

        match_data.players.second.config.name                = "engine2";
        match_data.players.second.color                      = chess::Color::WHITE;
        match_data.players.second.result                     = chess::GameResult::NONE;
        match_data.players.second.config.limit.tc.fixed_time = 1000;

        match_data.moves = {MoveData("e8g8", "+1.00", 1321, 15, 4, 0, 0),
                            MoveData("e1g1", "+1.23", 430, 15, 3, 0, 0),
                            MoveData("a6c5", "+1.45", 310, 16, 24, 0, 0)};

        match_data.fen = "r2qk2r/1bpp2pp/n3pn2/p2P1p2/1bP5/2N1BNP1/1PQ1PPBP/R3K2R b KQkq - 0 1";

        match_data.reason = "aborted";

        options::Tournament options;
        options.site = "localhost";

        std::string expected = R"([Event "Fast-Chess Tournament"]
[Site "localhost"]
[Round "1"]
[White "engine2"]
[Black "engine1"]
[Result "*"]
[SetUp "1"]
[FEN "r2qk2r/1bpp2pp/n3pn2/p2P1p2/1bP5/2N1BNP1/1PQ1PPBP/R3K2R b KQkq - 0 1"]
[PlyCount "3"]
[TimeControl "1/move"]

1... O-O {+1.00/15, 1.321s} 2. O-O {+1.23/15, 0.430s}
Nc5 {+1.45/16, 0.310s, aborted} *

)";

        epd::EpdBuilder epd_builder = epd::EpdBuilder(match_data, options);
        CHECK(epd_builder.get() == expected);
    }

    TEST_CASE("EPD Creation Multiple Fixed Time per Move") {
        MatchData match_data;
        match_data.players.first.config.name                = "engine1";
        match_data.players.first.color                      = chess::Color::BLACK;
        match_data.players.first.result                     = chess::GameResult::NONE;
        match_data.players.first.config.limit.tc.fixed_time = 200;

        match_data.players.second.config.name                = "engine2";
        match_data.players.second.color                      = chess::Color::WHITE;
        match_data.players.second.result                     = chess::GameResult::NONE;
        match_data.players.second.config.limit.tc.fixed_time = 1000;

        match_data.moves = {MoveData("e8g8", "+1.00", 1321, 15, 4, 0, 0),
                            MoveData("e1g1", "+1.23", 430, 15, 3, 0, 0),
                            MoveData("a6c5", "+1.45", 310, 16, 24, 0, 0)};

        match_data.fen = "r2qk2r/1bpp2pp/n3pn2/p2P1p2/1bP5/2N1BNP1/1PQ1PPBP/R3K2R b KQkq - 0 1";

        match_data.reason = "aborted";

        options::Tournament options;
        options.site = "localhost";

        std::string expected = R"([Event "Fast-Chess Tournament"]
[Site "localhost"]
[Round "1"]
[White "engine2"]
[Black "engine1"]
[Result "*"]
[SetUp "1"]
[FEN "r2qk2r/1bpp2pp/n3pn2/p2P1p2/1bP5/2N1BNP1/1PQ1PPBP/R3K2R b KQkq - 0 1"]
[PlyCount "3"]
[WhiteTimeControl "1/move"]
[BlackTimeControl "0.2/move"]

1... O-O {+1.00/15, 1.321s} 2. O-O {+1.23/15, 0.430s}
Nc5 {+1.45/16, 0.310s, aborted} *

)";

        epd::EpdBuilder epd_builder = epd::EpdBuilder(match_data, options);
        CHECK(epd_builder.get() == expected);
    }
}
}  // namespace fast_chess
