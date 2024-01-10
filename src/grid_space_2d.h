#pragma once

#include "godot_cpp/classes/node2d.hpp"

namespace godot 
{
class GridSpace2D : public Node2D {
	GDCLASS(GridSpace2D, Node2D)

	protected:
	static void _bind_methods();
};
} //namespace godot