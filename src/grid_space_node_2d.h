#pragma once

#include "godot_cpp/variant/vector2i.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/templates/hash_map.hpp"
#include "grid_node_2d.h"

namespace godot 
{
class GridSpaceNode2D : public Node2D {
	GDCLASS(GridSpaceNode2D, Node2D)

	private:
	HashMap<int, GridNode2D*> _grid_objects;
	Vector2i _grid_dimensions = Vector2i(1, 1);
	Vector2i _slot_dimensions = Vector2i(100, 100);
	Callable _object_dimensions_changed_callback;
	Callable _object_position_changed_callback;

	protected:
	static void _bind_methods();

	private:
	void _on_object_dimensions_changed(GridNode2D *p_obj, Vector2i p_old_dimensions, Vector2i p_new_dimensions);
	void _on_object_position_changed(GridNode2D *p_obj, Vector2i p_old_position, Vector2i p_new_position);
	int _generate_unique_id();

	public:
	void _enter_tree() override;
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
		for (KeyValue<int, GridNode2D*> &E : _grid_objects) {
			ret[E.key] = E.value;
		}
		return ret; 
	}
	GridNode2D* get_object_by_id(int id) const {
		if (!_grid_objects.has(id)) return nullptr;
		return _grid_objects[id];
	}
	bool has_object(const GridNode2D* p_obj) {
		for(KeyValue<int, GridNode2D*> &E : _grid_objects) {
			if (E.value == p_obj) return true;
		}
		return false;
	}
	bool object_with_id_exists(int id) const { return _grid_objects.has(id); }

	void remove_object(GridNode2D* p_obj);
	void remove_object_by_id(int id);
	void remove_all_objects();
	Rect2i get_pixel_bounds_for_object(const GridNode2D* p_obj) const;
	int add_object(GridNode2D *p_obj);
	TypedArray<int> add_objects(const TypedArray<GridNode2D> &p_obj_arr);
	bool object_is_overlapping(const GridNode2D* p_obj);
	bool object_is_outside_grid(const GridNode2D* p_obj) const;
	bool objects_are_overlapping(const GridNode2D* p_obj1, const GridNode2D* p_obj2) const;
	bool object_overlaps_at_position(const GridNode2D* p_obj, const Vector2i p_position);

	GridSpaceNode2D(){}
	~GridSpaceNode2D(){}
};
} //namespace godot