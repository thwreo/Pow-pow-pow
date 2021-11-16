#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

// CREATE_SQUARE DIN LABORATOR_3 -> object2D.h
namespace Obj2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    // Create circle with given center, radius and color
    Mesh* CreateCirlce(const std::string& name, glm::vec3 circleCenter, float radius, glm::vec3 color, bool fill = false);
}
