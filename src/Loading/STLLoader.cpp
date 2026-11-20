
#include "STLLoader.h"

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <limits>

static_assert(sizeof(float) == 4, "STL requires 32-bit floats");
static_assert(std::numeric_limits<float>::is_iec559, "STL requires IEEE 754 floating-point");

Mesh loadBinarySTL(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Could not open STL file: " + filepath);
    }

    // Skip the 80-byte header.
    file.seekg(80);

    // Read the number of triangles.
    uint32_t triangleCount;

    file.read(
        reinterpret_cast<char*>(&triangleCount),
        sizeof(triangleCount)
    );

    if (!file) {
        throw std::runtime_error("Invalid STL header.");
    }

    Mesh mesh;

    mesh.vertices.reserve(static_cast<size_t>(triangleCount) * 3);
    mesh.triangles.reserve(triangleCount);

    for (uint32_t i = 0; i < triangleCount; i++) {
        // Read and ignore the facet normal.
        float normal[3];

        file.read(
            reinterpret_cast<char*>(normal),
            sizeof(normal)
        );

        int firstVertex = static_cast<int>(mesh.vertices.size());

        // Read the three vertices.
        for (int j = 0; j < 3; j++) {
            float vertex[3];

            file.read(
                reinterpret_cast<char*>(vertex),
                sizeof(vertex)
            );

            if (!file) {
                throw std::runtime_error("Invalid STL triangle data.");
            }

            mesh.vertices.push_back({
                vertex[0],
                vertex[1],
                vertex[2]
            });
        }

        mesh.triangles.push_back({
            firstVertex,
            firstVertex + 1,
            firstVertex + 2
        });

        // Skip the 2-byte attribute field.
        file.seekg(2, std::ios::cur);

        if (!file) {
            throw std::runtime_error("Invalid STL triangle data.");
        }
    }

    return mesh;
}
