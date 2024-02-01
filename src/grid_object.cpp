#pragma once

#include "grid_object.h"

using namespace godot;

void GridObject::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_dimensions"), &GridObject::get_grid_dimensions);
	ClassDB::bind_method(D_METHOD("get_original_dimensions"), &GridObject::get_original_dimensions);
	ClassDB::bind_method(D_METHOD("set_grid_dimensions", "dimensions"), &GridObject::set_grid_dimensions);
	ClassDB::bind_method(D_METHOD("get_grid_position"), &GridObject::get_grid_position);
	ClassDB::bind_method(D_METHOD("set_grid_position", "position"), &GridObject::set_grid_position);
	ClassDB::bind_method(D_METHOD("get_grid_bounds"), &GridObject::get_grid_bounds);
	ClassDB::bind_method(D_METHOD("set_grid_bounds", "bounds"), &GridObject::set_grid_bounds);
	ClassDB::bind_method(D_METHOD("set_rotated", "rotate"), &GridObject::set_rotated);
	ClassDB::bind_method(D_METHOD("is_rotated"), &GridObject::is_rotated);
	ClassDB::bind_method(D_METHOD("to_dict"), &GridObject::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "data"), &GridObject::from_dict);

	ADD_SIGNAL(MethodInfo("grid_dimensions_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("grid_position_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_position"), PropertyInfo(Variant::VECTOR2I, "new_position")));
	ADD_SIGNAL(MethodInfo("rotated", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "is_rotated")));

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_dimensions"), "set_grid_dimensions", "get_grid_dimensions");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_position"), "set_grid_position", "get_grid_position");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2I, "grid_bounds"), "set_grid_bounds", "get_grid_bounds");
}

void GridObject::set_grid_bounds(const Rect2i &p_bounds) {
	Rect2i old_bounds = _grid_bounds;
	_grid_bounds.position = p_bounds.position;
	_original_dimensions = p_bounds.size;
	if (_rotated) {
		_grid_bounds.size = Vector2i(_original_dimensions.y, _original_dimensions.x);
	} else {
		_grid_bounds.size = Vector2i(_original_dimensions.x, _original_dimensions.y);
	}
	emit_signal("grid_dimensions_changed", this, old_bounds.size, _grid_bounds.size);
	emit_signal("grid_position_changed", this, old_bounds.position, _grid_bounds.position);
}

void GridObject::set_grid_dimensions(const Vector2i &dimensions) {
	Vector2i old_dimensions = _grid_bounds.size;
	_original_dimensions = dimensions;
	if (_rotated) {
		_grid_bounds.size = Vector2i(_original_dimensions.y, _original_dimensions.x);
	} else {
		_grid_bounds.size = Vector2i(_original_dimensions.x, _original_dimensions.y);
	}
	emit_signal("grid_dimensions_changed", this, old_dimensions, _grid_bounds.size);
}

void GridObject::set_rotated(bool rotate) {
	_rotated = rotate; //invert
	if (_rotated) {
		_grid_bounds.size = Vector2i(_grid_bounds.size.y, _grid_bounds.size.x);
	} else {
		_grid_bounds.size = Vector2i(_grid_bounds.size.x, _grid_bounds.size.y);
	}
	emit_signal("rotated", _rotated);
}

void GridObject::set_grid_position(const Vector2i &pos) {
	Vector2i old_position = _grid_bounds.position;
	_grid_bounds.position = pos;
	emit_signal("grid_position_changed", this, old_position, _grid_bounds.position);
}

Dictionary GridObject::to_dict() {
	Dictionary ret;
	ret["grid_dimensions"] = _grid_bounds.size;
	ret["original_dimensions"] = _original_dimensions;
	ret["grid_position"] = _grid_bounds.position;
	ret["rotated"] = _rotated;
	return ret;
}

Ref<GridObject> GridObject::from_dict(Dictionary data) {
	set_grid_dimensions(data["grid_dimensions"]);
	set_grid_position(data["grid_position"]);
	set_rotated(data["rotated"]);
	return this;
}