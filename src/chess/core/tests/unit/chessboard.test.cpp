#include "chess/core/chessboard.h"

#include <gtest/gtest.h>

namespace chess::core {

struct ChessboardFixture : public testing::Test {
    void SetUp() override { chessboard = std::make_unique<Chessboard>(kDefaultChessboard); }

    std::unique_ptr<Chessboard> chessboard;
};

TEST_F(ChessboardFixture, is_in_range) {
    EXPECT_FALSE(chessboard->is_in_range(CellIndex{-1, 0}));
    EXPECT_TRUE(chessboard->is_in_range(CellIndex{1, 1}));
}

}  // namespace chess::core
