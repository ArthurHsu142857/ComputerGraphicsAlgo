#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace Utility {
	static void PrintVector(const char* vectorName, glm::vec3 vector) { 
		std::cout << vectorName << ": (" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
	};
}