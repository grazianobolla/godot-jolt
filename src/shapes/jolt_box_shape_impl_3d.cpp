#include "jolt_box_shape_impl_3d.hpp"

Variant JoltBoxShapeImpl3D::get_data() const {
	return half_extents;
}

void JoltBoxShapeImpl3D::set_data(const Variant& p_data) {
	ON_SCOPE_EXIT {
		invalidated();
	};

	destroy();

	ERR_FAIL_COND(p_data.get_type() != Variant::VECTOR3);

	half_extents = p_data;
}

void JoltBoxShapeImpl3D::set_margin(float p_margin) {
	ON_SCOPE_EXIT {
		invalidated();
	};

	destroy();

	margin = p_margin;
}

String JoltBoxShapeImpl3D::to_string() const {
	return vformat("{half_extents=%v margin=%f}", half_extents, margin);
}

JPH::ShapeRefC JoltBoxShapeImpl3D::build() const {
	const float shortest_axis = half_extents[half_extents.min_axis_index()];

	ERR_FAIL_COND_D_MSG(
		shortest_axis <= margin,
		vformat(
			"Failed to build box shape with %s. "
			"Its half extents must be greater than its margin.",
			to_string()
		)
	);

	const JPH::BoxShapeSettings shape_settings(to_jolt(half_extents), margin);
	const JPH::ShapeSettings::ShapeResult shape_result = shape_settings.Create();

	ERR_FAIL_COND_D_MSG(
		shape_result.HasError(),
		vformat(
			"Failed to build box shape with %s. "
			"It returned the following error: '%s'.",
			to_string(),
			to_godot(shape_result.GetError())
		)
	);

	return shape_result.Get();
}