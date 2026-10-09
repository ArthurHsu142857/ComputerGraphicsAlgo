#pragma once

#include <glm/glm.hpp>
#include <iostream>

namespace Utility {
static void PrintVector(const char* vectorName, glm::vec3 vector) {
  std::cout << vectorName << ": (" << vector.x << ", " << vector.y << ", "
            << vector.z << ")" << std::endl;
};
}  // namespace Utility