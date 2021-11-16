#include "Obj2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

// CREATE_SQUARE DIN LABORATOR_3 -> object2D.cpp
Mesh* Obj2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* Obj2D::CreateCirlce(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(corner));
    std::vector<unsigned int> indices;
    indices.push_back(0);
    int nrOfPoints = 35;
    float theta = 2 * M_PI / nrOfPoints;
    float alpha = 0;
    
    for (int i = 1; i <= nrOfPoints; i++) {
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * sin(alpha), radius * cos(alpha), 0), color));
        alpha += theta;
        indices.push_back(i);
    }
    vertices.push_back(VertexFormat(corner + glm::vec3(radius * sin(alpha), radius * cos(alpha), 0), color));
    indices.push_back(nrOfPoints + 1);

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }
    circle->InitFromData(vertices, indices);
    return circle;
}