#pragma once

#include "grid_space.h"

using namespace godot;

void GridSpace::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_dimensions"), &GridSpace::get_grid_dimensions);
	ClassDB::bind_method(D_METHOD("set_grid_dimensions", "grid_dimensions"), &GridSpace::set_grid_dimensions);
	ClassDB::bind_method(D_METHOD("get_slot_dimensions"), &GridSpace::get_slot_dimensions);
	ClassDB::bind_method(D_METHOD("set_slot_dimensions", "slot_dimensions"), &GridSpace::set_slot_dimensions);
	ClassDB::bind_method(D_METHOD("get_all_objects"), &GridSpace::get_all_objects);
	ClassDB::bind_method(D_METHOD("get_object_by_id", "id"), &GridSpace::get_object_by_id);
	ClassDB::bind_method(D_METHOD("has_object", "object"), &GridSpace::has_object);
	ClassDB::bind_method(D_METHOD("object_with_id_exists", "id"), &GridSpace::object_with_id_exists);
	ClassDB::bind_method(D_METHOD("remove_object", "object"), &GridSpace::remove_object);
	ClassDB::bind_method(D_METHOD("remove_object_by_id", "id"), &GridSpace::remove_object_by_id);
	ClassDB::bind_method(D_METHOD("remove_all_objects"), &GridSpace::remove_all_objects);
	ClassDB::bind_method(D_METHOD("add_object", "object"), &GridSpace::add_object);
	ClassDB::bind_method(D_METHOD("add_objects", "object_array"), &GridSpace::add_objects);
	ClassDB::bind_method(D_METHOD("object_is_overlapping", "object"), &GridSpace::object_is_overlapping);
	ClassDB::bind_method(D_METHOD("object_is_outside_grid", "object"), &GridSpace::object_is_outside_grid);
	ClassDB::bind_method(D_METHOD("objects_are_overlapping", "object1", "object2"), &GridSpace::objects_are_overlapping);
	ClassDB::bind_method(D_METHOD("object_overlaps_at_position", "object", "position"), &GridSpace::object_overlaps_at_position);
	ClassDB::bind_method(D_METHOD("get_pixel_bounds_for_object", "object"), &GridSpace::get_pixel_bounds_for_object);
	ClassDB::bind_method(D_METHOD("_on_object_dimensions_changed"), &GridSpace::_on_object_dimensions_changed);
	ClassDB::bind_method(D_METHOD("_on_object_position_changed"), &GridSpace::_on_object_position_changed);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_dimensions"), "set_grid_dimensions", "get_grid_dimensions");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "slot_dimensions"), "set_slot_dimensions", "get_slot_dimensions");

	ADD_SIGNAL(MethodInfo("grid_dimensions_changed", PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("slot_dimensions_changed", PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("object_added", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("object_removed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("object_dimensions_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_dimensions"), PropertyInfo(Variant::VECTOR2I, "new_dimensions")));
	ADD_SIGNAL(MethodInfo("object_position_changed", PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::VECTOR2I, "old_position"), PropertyInfo(Variant::VECTOR2I, "new_position")));
}

void GridSpace::remove_object(GridObject* p_obj) {
	for (KeyValue<int, GridObject*> &E : _grid_objects) {
		if (p_obj == E.value) {
			_grid_objects.erase(E.key);
			p_obj->disconnect("grid_dimensions_changed", _object_dimensions_changed_callback);
			p_obj->disconnect("grid_position_changed", _object_position_changed_callback);
			emit_signal("object_removed", p_obj, E.key);
			break;
		}
	}
}

void GridSpace::remove_object_by_id(int id) {
	if (!_grid_objects.has(id)) return;

	GridObject* remove = _grid_objects[id];
	remove->disconnect("grid_dimensions_changed", _object_dimensions_changed_callback);
	remove->disconnect("grid_position_changed", _object_position_changed_callback);
	_grid_objects.erase(id);
	emit_signal("object_removed", remove, id);
}

void GridSpace::remove_all_objects() {
	Dictionary all_objs = get_all_objects();
	Array keys = all_objs.keys();
	for(int i = 0; i < keys.size(); i++) {
		remove_object_by_id(keys[i]);
	}
}

int GridSpace::add_object(GridObject *p_obj) {
	int id = _generate_unique_id();
	_grid_objects.insert(id, p_obj);
	p_obj->connect("grid_dimensions_changed", _object_dimensions_changed_callback);
	p_obj->connect("grid_position_changed", _object_position_changed_callback);
	emit_signal("object_added", p_obj, id);
	return id;
 }

TypedArray<int> GridSpace::add_objects(const TypedArray<GridObject> &p_obj_arr) {
	TypedArray<int> ret_ids;
	for (int index = 0; index < p_obj_arr.size(); index++) {
		GridObject* obj = Object::cast_to<GridObject>(p_obj_arr[index]);
		ERR_CONTINUE_MSG(obj == nullptr, "Failed to add object. Casting to GridObject2D has failed");
		int id = add_object(obj);
		ret_ids.append(id);
	}
	return ret_ids;
}

bool GridSpace::object_is_overlapping(const GridObject* p_obj) {
	for(KeyValue<int, GridObject*> &E : _grid_objects) {
		if (E.value == p_obj) continue;
		if (objects_are_overlapping(p_obj, E.value)) {
			return true;
		}
	}
	return false;
}

bool GridSpace::object_is_outside_grid(const GridObject* p_obj) const {
	Rect2i obj_bounds = p_obj->get_grid_bounds();
	Rect2i grid_bounds = Rect2i(0,0,_grid_dimensions.x, _grid_dimensions.y);
	if (!grid_bounds.encloses(obj_bounds)) {
		return true;
	}
	return false;
}

bool GridSpace::objects_are_overlapping(const GridObject* p_obj1, const GridObject* p_obj2) const {
	Rect2i obj1_bounds = p_obj1->get_grid_bounds();
	Rect2i obj2_bounds = p_obj2->get_grid_bounds();
	if (obj1_bounds.intersects(obj2_bounds)) {
		return true;
	}
	return false;
}

bool GridSpace::object_overlaps_at_position(const GridObject* p_obj, const Vector2i p_position) {
	Rect2i obj_bounds = p_obj->get_grid_bounds();
	Rect2i proxy_bounds = Rect2i(p_position.x, p_position.y, obj_bounds.size.x, obj_bounds.size.y);
	for(KeyValue<int, GridObject*> &E : _grid_objects) {
		if (E.value == p_obj) continue;
		if (proxy_bounds.intersects(E.value->get_grid_bounds())){
			return true;
		}
	}
	return false;
}

Rect2i GridSpace::get_pixel_bounds_for_object(const GridObject* p_obj) const {
	Rect2i obj_bounds = p_obj->get_grid_bounds();
	Rect2i ret;
	ret.size = Vector2i(obj_bounds.size.x * _slot_dimensions.x, obj_bounds.size.y * _slot_dimensions.y);
	ret.position = Vector2i(obj_bounds.position.x * _slot_dimensions.x, obj_bounds.position.y * _slot_dimensions.y);
	return ret;
}


void GridSpace::_on_object_dimensions_changed(GridObject *p_obj, Vector2i p_old_dimensions, Vector2i p_new_dimensions) {
	emit_signal("object_dimensions_changed", p_obj, p_old_dimensions, p_new_dimensions);
}

void GridSpace::_on_object_position_changed(GridObject *p_obj, Vector2i p_old_position, Vector2i p_new_position) {
	emit_signal("object_position_changed", p_obj, p_old_position, p_new_position);
}

int GridSpace::_generate_unique_id() {
	int new_id = _id_counter;
	_id_counter++;
	return new_id;
}