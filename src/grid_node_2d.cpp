#pragma once

#include "grid_node_2d.h"

using namespace godot;

void GridNode2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_dimensions"), &GridNode2D::get_grid_dimensions);
	ClassDB::bind_method(D_METHOD("set_grid_dimensions", "dimensions"), &GridNode2D::set_grid_dimensions);
	ClassDB::bind_method(D_METHOD("get_grid_position"), &GridNode2D::get_grid_position);
	ClassDB::bind_method(D_METHOD("set_grid_position", "position"), &GridNode2D::set_grid_position);
	ClassDB::bind_method(D_METHOD("get_grid_bounds"), &GridNode2D::get_grid_bounds);
	ClassDB::bind_method(D_METHOD("set_grid_bounds", "bounds"), &GridNode2D::set_grid_bounds);

	ADD_SIGNAL(MethodInfo("grid_dimensions_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("grid_position_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_position"), PropertyInfo(Variant::VECTOR2I, "new_position")));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_dimensions"), "set_grid_dimensions", "get_grid_dimensions");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_position"), "set_grid_position", "get_grid_position");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2I, "grid_bounds"), "set_grid_bounds", "get_grid_bounds");
}
