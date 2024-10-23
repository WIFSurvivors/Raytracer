#pragma once
#include <vector>

//#include <glm/glm.hpp>

struct component;

struct entity
{
	entity();

	std::vector<component>& get_components(component c);

private:	
	std::vector<component> _components{1};
};

