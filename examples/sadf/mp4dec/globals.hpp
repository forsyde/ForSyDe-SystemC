/**********************************************************************
    * globals.hpp -- The globals Definitions for the MPEG-4 decoder   *
    *                                                                 *
    * Author:  Hosein Attarzadeh (h_attarzadeh@sbu.ac.ir)             *
    *                                                                 *
    * Purpose: Demonstration of an example in the SADF MoC.           *
    *                                                                 *
    * Usage:   MPEG4-SP example                                       *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/


#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <forsyde.hpp>
#include <variant>
#include <iostream>

using namespace ForSyDe;
using namespace std;

// The size of the frame
constexpr size_t fsr = 176;
constexpr size_t fsc = 44;

// The size of the macroblock
constexpr size_t bs = 8;

// Number of macro blocks in a frame
constexpr size_t nmb = fsr*fsc / (bs*bs);

// A matrix is a vector of vectors
template <typename T, size_t M, size_t N>
using Matrix = array<array<T, N>, M>;

// A frame is a 2D matrix of integer pixels
template <size_t M, size_t N>
using Frame = Matrix<int, M, N>;

// Similarly, a block type is a (smaller) frame
template <size_t N>
using Block = Frame<N, N>;

template <size_t N>
struct FullB {
    tuple<int, int> pos;
    tuple<int, int> motionV;
    Block<N> block;
};
// implement the '<<' stream operator for FullB
template <size_t N>
ostream& operator<<(ostream& os, const FullB<N>& b) {
    os << "FullB { pos = (" << get<0>(b.pos) << ", " << get<1>(b.pos) << "), motionV = (" << get<0>(b.motionV) << ", " << get<1>(b.motionV) << "), block = " << endl;
    for (size_t i = 0; i < N; i++) {
        os << "  ";
        for (size_t j = 0; j < N; j++) {
            os << b.block[i][j] << " ";
        }
        os << endl;
    }
    os << "}";
    return os;
}

template <size_t N>
struct PosB {
    tuple<int, int> pos;
    Block<N> block;
};
// implement the '<<' stream operator for PosB
template <size_t N>
ostream& operator<<(ostream& os, const PosB<N>& b) {
    os << "PosB { pos = (" << get<0>(b.pos) << ", " << get<1>(b.pos) << "), block = " << endl;
    for (size_t i = 0; i < N; i++) {
        os << "  ";
        for (size_t j = 0; j < N; j++) {
            os << b.block[i][j] << " ";
        }
        os << endl;
    }
    os << "}";
    return os;
}

template <size_t N>
using MacroBlock = variant<FullB<N>, PosB<N>>;
// implement the '<<' stream operator for MacroBlock
template <size_t N>
ostream& operator<<(ostream& os, const MacroBlock<N>& b) {
    if (holds_alternative<FullB<N>>(b)) {
        os << get<FullB<N>>(b);
    } else {
        os << get<PosB<N>>(b);
    }
    return os;
}

struct MotionVec {
    tuple<int, int> mvPos;
    tuple<int, int> mvVec;
};
// implement the '<<' stream operator for MotionVec
ostream& operator<<(ostream& os, const MotionVec& b) {
    os << "MotionVec { mvPos = (" << get<0>(b.mvPos) << ", " << get<1>(b.mvPos) << "), mvVec = (" << get<0>(b.mvVec) << ", " << get<1>(b.mvVec) << ")}";
    return os;
}

enum frame_type {I, P0, P30, P40, P50, P60, P70, P80, P99};

// ----------------------------------------------------------
// --                 Auxiliar functions
// ----------------------------------------------------------

// -- Apply the Inverse Discrete Cosine Transform (IDCT) to a matrix of Ints
template <size_t N>
Block<N> inverseDCT(const Block<N>& x) {
    Block<N> y;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) { // TODO: use std::transform
            y[i][j] = 0;
            for (size_t k = 0; k < N; k++) {
                y[i][j] += cos((2.0 * j + 1.0) * k * M_PI / (2.0 * N)) * x[i][k];
            }
            y[i][j] *= sqrt(2.0 / N);
            if (j == 0) {
                y[i][j] /= sqrt(2.0);
            }
        }
    }
    return y;
}

// -- Creates a n x n matrix for the Discrete Cosine Transform (DCT)

// -- Block adds matrix x to matrix y in a the position given by (row,col)
template <size_t M,size_t Nr,size_t Nc>
Frame<Nr,Nc> blockAdd(const MacroBlock<M>& b, const Frame<Nr,Nc>& y) {
    Frame<Nr,Nc> out;
    for (size_t i = 0; i < Nr; i++) {
        for (size_t j = 0; j < Nc; j++) {
            out[i][j] = y[i][j];
        }
    }
    const auto& b1 = get<PosB<M>>(b);
    const auto& x = b1.block;
    const auto& [row, col] = b1.pos;
    for (size_t i = row; i < min(Nr, row + M); i++) {
        for (size_t j = col; j < min(Nc, col + M); j++) {
            out[i][j] += x[i - row][j - col];
        }
    }
    return out;
}

// the submatrix function to be used in the function below
template <size_t Nr, size_t Nc,size_t M>
Block<M> submatrix(size_t r1, size_t c1, const Frame<Nr,Nc>& x) {
    Block<M> out;
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < M; j++) {
            out[i][j] = x[r1 + i][c1 + j];
        }
    }
    return out;
}

// -- Split a large block into a list of macro blocks of size (dr,dc) or smaller
template <size_t Nr, size_t Nc, size_t M>
vector<MacroBlock<M>> frame2mblocks(const Frame<Nr,Nc>& x) {
    vector<MacroBlock<M>> out;
    for (size_t i = 0; i < Nr; i+=M) {
        for (size_t j = 0; j < Nc; j+=M) {
            out.push_back(PosB<M>{make_tuple(i, j), submatrix<Nr,Nc, M>(i, j, x)});
        }
    }
    return out;
}


// -- Gets a matrix x and a list of motion vectors and returns a motion compensated matrix
template <size_t M,size_t Nr,size_t Nc>
Frame<Nr,Nc> motionComp(const vector<MotionVec>& mvs, const Frame<Nr,Nc>& x) {
    Frame<Nr,Nc> out;
    // for (size_t i = 0; i < N; i++) {
    //     for (size_t j = 0; j < N; j++) {
    //         out[i][j] = 0;
    //     }
    // }
    auto y = frame2mblocks<Nr,Nc,M>(x);
    vector<MacroBlock<M>> z1;
    for (const auto& a : y) {
        for (const auto& b : mvs) {
            const auto& a1 = get<PosB<M>>(a);
            if (a1.pos == b.mvPos) {
                z1.push_back(PosB<M>{make_tuple(get<0>(a1.pos) + get<0>(b.mvVec), get<1>(a1.pos) + get<1>(b.mvVec)), a1.block});
            }
        }
    }
    vector<MacroBlock<M>> z2;
    for (const auto& a : y) {
        bool found = false;
        for (const auto& b : mvs) {
            const auto& a1 = get<PosB<M>>(a);
            if (a1.pos == b.mvPos) {
                found = true;
                break;
            }
        }
        if (!found)
            z2.push_back(a);
    }
    for (const auto& a : z1)
        out = blockAdd(a, out);
    for (const auto& a : z2) 
        out = blockAdd(a, out);
    return out;
}


// -- Reconstruct frame based on a former frame x and a list of macro blocks mbs
template <size_t M,size_t Nr,size_t Nc>
Frame<Nr,Nc> frameRC(const vector<MacroBlock<M>>& mbs, const Frame<Nr,Nc>& x) {
    Frame<Nr,Nc> out = x;
    for (const auto& a : mbs) {
        out = blockAdd(a, out);
    }
    return out;
}

#endif
