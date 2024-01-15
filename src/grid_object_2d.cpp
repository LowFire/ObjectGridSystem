#pragma once

#include "grid_object_2d.h"

using namespace godot;

void GridObject2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_dimensions"), &GridObject2D::get_grid_dimensions);
	ClassDB::bind_method(D_METHOD("set_grid_dimensions", "dimensions"), &GridObject2D::set_grid_dimensions);
	ClassDB::bind_method(D_METHOD("get_grid_position"), &GridObject2D::get_grid_position);
	ClassDB::bind_method(D_METHOD("set_grid_position", "position"), &GridObject2D::set_grid_position);
	ClassDB::bind_method(D_METHOD("get_grid_bounds"), &GridObject2D::get_grid_bounds);
	ClassDB::bind_method(D_METHOD("set_grid_bounds", "bounds"), &GridObject2D::set_grid_bounds);

	ADD_SIGNAL(MethodInfo("grid_dimensions_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("grid_position_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_position"), PropertyInfo(Variant::VECTOR2I, "new_position")));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "dimensions"), "set_grid_dimensions", "get_dimensions");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "position"), "set_position", "get_position");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2I, "bounds"), "set_bounds", "get_bounds");
}
