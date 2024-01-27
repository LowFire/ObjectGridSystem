#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include "godot_cpp/variant/rect2i.hpp"

namespace godot 
{
class GridObject : public RefCounted {
	GDCLASS(GridObject, RefCounted);

	private:
	Rect2i _grid_bounds = Rect2i(0,0,1,1);

	protected:
	static void _bind_methods();

	public:
	Vector2i get_grid_dimensions() const { return _grid_bounds.size; }
	void set_grid_dimensions(const Vector2i &dimensions) {
		Vector2i old_dimensions = _grid_bounds.size;
		_grid_bounds.size = dimensions;
		emit_signal("grid_dimensions_changed", this, old_dimensions, _grid_bounds.size);
	}
	Vector2i get_grid_position() const { return _grid_bounds.position; }
	void set_grid_position(const Vector2i &pos) {
		Vector2i old_position = _grid_bounds.position;
		_grid_bounds.position = pos;
		emit_signal("grid_position_changed", this, old_position, _grid_bounds.position);
	}
	Rect2i get_grid_bounds() const { return _grid_bounds; }
	void set_grid_bounds(const Rect2i &p_bounds) {
		Rect2i old_bounds = _grid_bounds;
		_grid_bounds = p_bounds;
		emit_signal("grid_dimensions_changed", this, old_bounds.size, _grid_bounds.size);
		emit_signal("grid_position_changed", this, old_bounds.position, _grid_bounds.position);
	}
	void rotate() {}

	GridObject(){}
	~GridObject(){}
};
} //namespace godot