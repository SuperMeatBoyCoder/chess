#pragma once
#include "structs.h"
#include "chess_piece.h"

namespace Chess {
class King : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    King(PieceInfo info) : ChessPiece(info) {log("King was constructed");}
    ~King() {log("King was deconstructed");}
    std::vector<ChessMove> PossibleMovement(Board* board) const override;
    bool IsChecking(Board* board, Square king_square) const override;
};


class Pawn : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Pawn(PieceInfo info) : ChessPiece(info) {log("Pawn was constructed");}
    ~Pawn() {log("Pawn was deconstructed");}
    std::vector<ChessMove> PossibleMovement(Board* board) const override;
    bool IsChecking(Board* board, Square king_square) const override;
};

// Knight is Night. It's a feature, not a bug
class Night : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Night(PieceInfo info) : ChessPiece(info) {log("Night was constructed");}
    ~Night() {log("Night was deconstructed");}
    std::vector<ChessMove> PossibleMovement(Board* board) const override;
    bool IsChecking(Board* board, Square king_square) const override;
};

class Bishop : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Bishop(PieceInfo info) : ChessPiece(info) {log("Bishop was constructed");}
    ~Bishop() {log("Bishop was deconstructed");}
    std::vector<ChessMove> PossibleMovement(Board* board) const override;
    bool IsChecking(Board* board, Square king_square) const override;
};

class Rook : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Rook(PieceInfo info) : ChessPiece(info) {log("Rook was constructed");}
    ~Rook() {log("Rook was deconstructed");}
    std::vector<ChessMove> PossibleMovement(Board* board) const override;
    bool IsChecking(Board* board, Square king_square) const override;
};

class Queen : public ChessPiece {
public:
    using ChessPiece::ChessPiece;
    Queen(PieceInfo info) : ChessPiece(info) {log("Queen was constructed");}
    ~Queen() {log("Queen was deconstructed");}
    std::vector<ChessMove> PossibleMovement(Board* board) const override;
    bool IsChecking(Board* board, Square king_square) const override;
};
}