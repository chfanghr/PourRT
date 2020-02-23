//
// Created by huang825172 on 2020/2/23.
// Reference: https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/

#ifndef PRT__ICOSAHEDRON_H_
#define PRT__ICOSAHEDRON_H_

#include <vector>
#include <array>
#include <map>
#include <cmath>

namespace icosahedron {

struct Triangle {
  int vertex[3];
};

using Index=int;
using v3=std::array<float, 3>;
using TriangleList=std::vector<Triangle>;
using VertexList=std::vector<v3>;
using Lookup=std::map<std::pair<Index, Index>, Index>;
using IndexedMesh=std::pair<VertexList, TriangleList>;

v3 operator+(const v3 &lhs, const v3 &rhs);
v3 normalize(const v3 &rhs);

const float X = .525731112119133606f;
const float Z = .850650808352039932f;
const float N = 0.f;

static const VertexList vertices = // NOLINT(cert-err58-cpp)
    {
        {-X, N, Z}, {X, N, Z}, {-X, N, -Z}, {X, N, -Z},
        {N, Z, X}, {N, Z, -X}, {N, -Z, X}, {N, -Z, -X},
        {Z, X, N}, {-Z, X, N}, {Z, -X, N}, {-Z, -X, N}
    };

static const TriangleList triangles = // NOLINT(cert-err58-cpp)
    {
        {0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1},
        {8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3},
        {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
        {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}
    };

Index vertex_for_edge(Lookup &lookup, VertexList &vertices, Index first, Index second);
TriangleList subdivide(VertexList &vertices, const TriangleList &triangles);
IndexedMesh make_icosphere(int subdivisions);
v3 get_normal(const VertexList &vl, const Triangle &tri);
}

#endif //PRT__ICOSAHEDRON_H_
