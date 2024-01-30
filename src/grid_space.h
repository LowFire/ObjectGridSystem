#pragma once

#include "godot_cpp/variant/vector2i.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/templates/hash_map.hpp"
#include "grid_object.h"

namespace godot 
{
class GridSpace : public RefCounted {
	GDCLASS(GridSpace, RefCounted)

	private:
	int _id_counter = 0;
	HashMap<int, Ref<GridObject>> _grid_objects;
	Vector2i _grid_dimensions = Vector2i(1, 1);
	Vector2i _slot_dimensions = Vector2i(100, 100);
	Callable _object_dimensions_changed_callback;
	Callable _object_position_changed_callback;

	protected:
	static void _bind_methods();

	private:
	void _on_object_dimensions_changed(Ref<GridObject> p_obj, Vector2i p_old_dimensions, Vector2i p_new_dimensions);
	void _on_object_position_changed(Ref<GridObject> p_obj, Vector2i p_old_position, Vector2i p_new_position);
	int _generate_unique_id();
	void _init();

	public:
	//void _enter_tree() override;
	Vector2i get_grid_dimensions() const { return _grid_dimensions; }
	void set_grid_dimensions(Vector2i p_grid_dimensions) {
		Vector2i old_dimensions = _grid_dimensions;
		_grid_dimensions = p_grid_dimensions;
		emit_signal("grid_dimensions_changed", old_dimensions, _grid_dimensions);
	}
	Vector2i get_slot_dimensions() const { return _slot_dimensions; }
	void set_slot_dimensions(Vector2i p_slot_dimensions) {
		Vector2i old_slot_dimensions = _slot_dimensions;
		_slot_dimensions = p_slot_dimensions;
		emit_signal("slot_dimensions_changed", old_slot_dimensions, _slot_dimensions);
	}
	Dictionary get_all_objects() { 
		Dictionary ret;
		for (KeyValue<int, Ref<GridObject>> &E : _grid_objects) {
			ret[E.key] = E.value;
		}
		return ret; 
	}
	Ref<GridObject> get_object_by_id(int id) const {
		if (!_grid_objects.has(id)) return nullptr;
		return _grid_objects[id];
	}
	bool has_object(const Ref<GridObject> p_obj) {
		for(KeyValue<int, Ref<GridObject>> &E : _grid_objects) {
			if (E.value == p_obj) return true;
		}
		return false;
	}
	bool object_with_id_exists(int id) const { return _grid_objects.has(id); }

	void remove_object(Ref<GridObject> p_obj);
	void remove_object_by_id(int id);
	void remove_all_objects();
	Rect2i get_pixel_bounds_for_object(const Ref<GridObject> p_obj) const;
	int add_object(Ref<GridObject> p_obj);
	void add_object_with_id(Ref<GridObject> p_obj, int id);
	TypedArray<int> add_objects(const TypedArray<GridObject> &p_obj_arr);
	bool object_is_overlapping(const Ref<GridObject> p_obj);
	bool object_is_outside_grid(const Ref<GridObject> p_obj) const;
	bool objects_are_overlapping(const Ref<GridObject> p_obj1, const Ref<GridObject> p_obj2) const;
	bool object_overlaps_at_position(const Ref<GridObject> p_obj, const Vector2i p_position);
	bool find_best_fit_for_object(const Ref<GridObject> p_obj, Dictionary out_data);

	GridSpace(){
		_object_dimensions_changed_callback = Callable(this, "_on_object_dimensions_changed");
		_object_position_changed_callback = Callable(this, "_on_object_position_changed");
	}
	~GridSpace(){}
};
} //namespace godot