void Assignment14::createCubeMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    // The primitive built here is a cube centered at the point (0, 0, 1), with edge length 2.

    float offset = -1.0f; // Offset to center the cube at (0, 0, 1)

    vDef.push_back({ {-1.0f, -1.0f, -1.0f + offset}, {0.5f, 0.0f, 0.866f} });  // vertex 0 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, -1.0f + offset}, {0.0f, 0.0f, 1.0f} });     // vertex 1 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, -1.0f + offset}, {0.0f, 0.0f, 1.0f} });     // vertex 2 - Position and Normal
    vDef.push_back({ {1.0f, 1.0f, -1.0f + offset}, {-0.5f, 0.0f, 0.866f} });   // vertex 3 - Position and Normal
    vDef.push_back({ {-1.0f, -1.0f, 1.0f + offset}, {0.5f, 0.0f, 0.866f} });   // vertex 4 - Position and Normal
    vDef.push_back({ {1.0f, -1.0f, 1.0f + offset}, {0.0f, 0.0f, 1.0f} });      // vertex 5 - Position and Normal
    vDef.push_back({ {-1.0f, 1.0f, 1.0f + offset}, {0.0f, 0.0f, 1.0f} });      // vertex 6 - Position and Normal
    vDef.push_back({ {1.0f, 1.0f, 1.0f + offset}, {-0.5f, 0.0f, 0.866f} });    // vertex 7 - Position and Normal

    // Fill the array vIdx with the indices of the vertices of the triangles
    vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);  // Front face - First triangle
    vIdx.push_back(1); vIdx.push_back(3); vIdx.push_back(2);  // Front face - Second triangle
    vIdx.push_back(1); vIdx.push_back(5); vIdx.push_back(3);  // Right face - First triangle
    vIdx.push_back(5); vIdx.push_back(7); vIdx.push_back(3);  // Right face - Second triangle
    vIdx.push_back(5); vIdx.push_back(4); vIdx.push_back(7);  // Back face - First triangle
    vIdx.push_back(4); vIdx.push_back(6); vIdx.push_back(7);  // Back face - Second triangle
    vIdx.push_back(4); vIdx.push_back(0); vIdx.push_back(6);  // Left face - First triangle
    vIdx.push_back(0); vIdx.push_back(2); vIdx.push_back(6);  // Left face - Second triangle
    vIdx.push_back(6); vIdx.push_back(2); vIdx.push_back(3);  // Top face - First triangle
    vIdx.push_back(6); vIdx.push_back(3); vIdx.push_back(7);  // Top face - Second triangle
    vIdx.push_back(4); vIdx.push_back(0); vIdx.push_back(1);  // Bottom face - First triangle
    vIdx.push_back(4); vIdx.push_back(1); vIdx.push_back(5);  // Bottom face - Second triangle
}

void Assignment14::createFunctionMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    const float minX = -3.0f;
    const float maxX = 3.0f;
    const float minZ = -3.0f;
    const float maxZ = 3.0f;
    const float step = 0.1f;

    for (float x = minX; x <= maxX; x += step) {
        for (float z = minZ; z <= maxZ; z += step) {
            float y = std::sin(x) * std::cos(z);

            float nx = std::sin(x + step) * std::cos(z) - std::sin(x - step) * std::cos(z);
            float nz = std::sin(x) * std::cos(z + step) - std::sin(x) * std::cos(z - step);
            float ny = 2.0f * step;

            float length = std::sqrt(nx * nx + ny * ny + nz * nz);
            nx /= length;
            ny /= length;
            nz /= length;

            vDef.push_back({ { x, y, z }, { nx, ny, nz } });
        }
    }

    for (uint32_t i = 0; i < (maxX - minX) / step; i++) {
        for (uint32_t j = 0; j < (maxZ - minZ) / step; j++) {
            uint32_t topLeft = i * ((maxZ - minZ) / step + 1) + j;
            uint32_t topRight = topLeft + 1;
            uint32_t bottomLeft = topLeft + ((maxZ - minZ) / step + 1);
            uint32_t bottomRight = bottomLeft + 1;

            vIdx.push_back(topLeft);
            vIdx.push_back(bottomLeft);
            vIdx.push_back(topRight);

            vIdx.push_back(topRight);
            vIdx.push_back(bottomLeft);
            vIdx.push_back(bottomRight);
        }
    }
}

#define M_PI 3.141595f
void Assignment14::createCylinderMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    // The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.

    const int numSegments = 50;
    const float angleStep = (2 * M_PI) / numSegments;

    // Fill array vPos with the positions of the vertices of the mesh
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x = cos(angle);
        float z = sin(angle);
        vDef.push_back({ {x, -1.0f, z}, {x, 0.0f, z} });
        vDef.push_back({ {x, 1.0f, z}, {x, 0.0f, z} });
    }

    // Fill the array vIdx with the indices of the vertices of the triangles
    for (int i = 0; i < numSegments; ++i) {
        int idx0 = i * 2;
        int idx1 = idx0 + 1;
        int idx2 = (i + 1) * 2;
        int idx3 = idx2 + 1;

        vIdx.push_back(idx0);
        vIdx.push_back(idx1);
        vIdx.push_back(idx2);

        vIdx.push_back(idx1);
        vIdx.push_back(idx3);
        vIdx.push_back(idx2);
    }
}

