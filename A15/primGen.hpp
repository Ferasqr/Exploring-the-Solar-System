void Assignment15::createBoxMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
    float width = 1024.0f;
    float height = 512.0f;
    float box_len = 1;
    float thickness = box_len / 3.7;

    // First rectangle left
    int starting_point = vDef.size();
    vDef.push_back({ {-box_len, -box_len, -thickness}, {1, 0, 0}, {0.0f / width, 415.0f / height} });
    vDef.push_back({ {-box_len, -box_len, thickness}, {1, 0, 0}, {92.0f / width, 413.0f / height} });
    vDef.push_back({ {-box_len, box_len, -thickness}, {1, 0, 0}, {0.0f / width, 96.0f / height} });
    vDef.push_back({ {-box_len, box_len, thickness}, {1, 0, 0}, {92.0f / width, 96.0f / height} });

    vIdx.push_back(starting_point); 
    vIdx.push_back(starting_point + 1);
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 3);

    // Second rectangle right
    starting_point = vDef.size();
    vDef.push_back({ {box_len, -box_len, -thickness}, {1, 0, 0}, {536.0f / width, 415.0f / height} });
    vDef.push_back({ {box_len, -box_len, thickness}, {1, 0, 0}, {445.0f / width, 413.0f / height} });
    vDef.push_back({ {box_len, box_len, -thickness}, {1, 0, 0}, {536.0f / width, 96.0f / height} });
    vDef.push_back({ {box_len, box_len, thickness}, {1, 0, 0}, {445.0f / width, 96.0f / height} });

    vIdx.push_back(starting_point);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 1);
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 3);
    // Third rectangle down
    starting_point = vDef.size();
    vDef.push_back({ {-box_len, -box_len, -thickness}, {0, 1, 0}, {96.0f / width, 512.0f / height} });
    vDef.push_back({ {-box_len, -box_len, thickness}, {0, 1, 0}, {96.0f / width, 421.0f / height} });
    vDef.push_back({ {box_len, -box_len, -thickness}, {0, 1, 0}, {442.0f / width, 512.0f / height} });
    vDef.push_back({ {box_len, -box_len, thickness}, {0, 1, 0}, {442.0f / width, 421.0f / height} });

    vIdx.push_back(starting_point);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2); 
    vIdx.push_back(starting_point + 3);

    // Fourth rectangle up
    starting_point = vDef.size();
    vDef.push_back({ {-box_len, box_len, -thickness}, {0, 1, 0}, {96.0f / width, 0.0f / height} });
    vDef.push_back({ {-box_len, box_len, thickness}, {0, 1, 0}, {96.0f / width, 90.0f / height} });
    vDef.push_back({ {box_len, box_len, -thickness}, {0, 1, 0}, {442.0f / width, 0.0f / height} });
    vDef.push_back({ {box_len, box_len, thickness}, {0, 1, 0}, {442.0f / width, 90.0f / height} });

    vIdx.push_back(starting_point);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 1);
    vIdx.push_back(starting_point + 2); 
    vIdx.push_back(starting_point + 3);

    // Fifth rectangle back
    starting_point = vDef.size();
    vDef.push_back({ {-box_len, -box_len, -thickness}, {0, 0, 1}, {887.0f / width, 415.0f / height} });
    vDef.push_back({ {-box_len, box_len, -thickness}, {0, 0, 1}, {887.0f / width, 96.0f / height} });
    vDef.push_back({ {box_len, -box_len, -thickness}, {0, 0, 1}, {540.0f / width, 413.0f / height} });
    vDef.push_back({ {box_len, box_len, -thickness}, {0, 0, 1}, {540.0f / width, 96.0f / height} });

    vIdx.push_back(starting_point);
    vIdx.push_back(starting_point + 1);
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2); 
    vIdx.push_back(starting_point + 3);
    // Sixth rectangle front
    starting_point = vDef.size();
    vDef.push_back({ {-box_len, -box_len, thickness}, {0, 0, 1}, {96.0f / width, 415.0f / height} });
    vDef.push_back({ {-box_len, box_len, thickness}, {0, 0, 1}, {96.0f / width, 96.0f / height} });
    vDef.push_back({ {box_len, -box_len, thickness}, {0, 0, 1}, {442.0f / width, 413.0f / height} });
    vDef.push_back({ {box_len, box_len, thickness}, {0, 0, 1}, {442.0f / width, 96.0f / height} });

    vIdx.push_back(starting_point);
    vIdx.push_back(starting_point + 1); 
    vIdx.push_back(starting_point + 2);
    vIdx.push_back(starting_point + 1);
    vIdx.push_back(starting_point + 2); 
    vIdx.push_back(starting_point + 3);
}
void Assignment15::createSphereMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx)
{
    float sphereRadius = 1.0f;
    int latitudeSegments = 50;

    for (int lat = 0; lat <= latitudeSegments; ++lat)
    {
        float theta = lat * glm::pi<float>() / latitudeSegments;
        for (int lon = 0; lon <= latitudeSegments; ++lon)
        {
            float phi = lon * (2 * glm::pi<float>() / latitudeSegments);
            float x = sphereRadius * sin(theta) * cos(phi);
            float y = sphereRadius * cos(theta);
            float z = sphereRadius * sin(theta) * sin(phi);
            float u = static_cast<float>(lon) / latitudeSegments;
            float v = static_cast<float>(lat) / latitudeSegments;
            vDef.push_back({ {x, y, z}, {x, y, z}, {u, v} });
        }
    }

    for (int lat = 0; lat < latitudeSegments; ++lat)
    {
        for (int lon = 0; lon < latitudeSegments; ++lon)
        {
            int first = lat * (latitudeSegments + 1) + lon;
            int second = first + latitudeSegments + 1;

            vIdx.push_back(first);
            vIdx.push_back(second + 1);
            vIdx.push_back(second);

            vIdx.push_back(first);
            vIdx.push_back(first + 1);
            vIdx.push_back(second + 1);
        }
    }
}

