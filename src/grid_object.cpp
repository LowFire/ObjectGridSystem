#pragma once

#include "grid_object.h"

using namespace godot;

void GridObject::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_dimensions"), &GridObject::get_grid_dimensions);
	ClassDB::bind_method(D_METHOD("set_grid_dimensions", "dimensions"), &GridObject::set_grid_dimensions);
	ClassDB::bind_method(D_METHOD("get_grid_position"), &GridObject::get_grid_position);
	ClassDB::bind_method(D_METHOD("set_grid_position", "position"), &GridObject::set_grid_position);
	ClassDB::bind_method(D_METHOD("get_grid_bounds"), &GridObject::get_grid_bounds);
	ClassDB::bind_method(D_METHOD("set_grid_bounds", "bounds"), &GridObject::set_grid_bounds);

	ADD_SIGNAL(MethodInfo("grid_dimensions_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("grid_position_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_position"), PropertyInfo(Variant::VECTOR2I, "new_position")));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_dimensions"), "set_grid_dimensions", "get_grid_dimensions");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_position"), "set_grid_position", "get_grid_position");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2I, "grid_bounds"), "set_grid_bounds", "get_grid_bounds");
}
