#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include "godot_cpp/variant/rect2i.hpp"

namespace godot 
{
class GridObject : public RefCounted {
	GDCLASS(GridObject, RefCounted);

	private:
	Vector2i _original_dimensions = Vector2i(1,1);
	Rect2i _grid_bounds = Rect2i(0,0,1,1);
	bool _rotated = false;

	protected:
	static void _bind_methods();

	public:
	Vector2i get_grid_dimensions() const { return _grid_bounds.size; }
	void set_grid_dimensions(const Vector2i &dimensions);
	Vector2i get_original_dimensions() const { return _original_dimensions; }
	Vector2i get_grid_position() const { return _grid_bounds.position; }
	void set_grid_position(const Vector2i &pos);
	Rect2i get_grid_bounds() const { return _grid_bounds; }
	void set_grid_bounds(const Rect2i &p_bounds);
	void set_rotated(bool rotate);
	bool is_rotated() { return _rotated; }
	Dictionary to_dict();
	Ref<GridObject> from_dict(Dictionary data);

	GridObject(){}
	~GridObject(){}
};
} //namespace godot