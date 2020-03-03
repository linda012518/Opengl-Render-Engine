#include "Transform.h"

namespace lyd
{
	Transform::Transform()
	{
		setPosition(float3(0, 0, 0));
		setScale(float3(1, 1, 1));
		setAngle(0, real3(0, 1, 0));
		update();
	}

	Transform::~Transform()
	{
	}

	quatr	Transform::getQuat() const
	{
		return _quat;
	}
	void	Transform::setQuat(const quatr& quat)
	{
		_quat = quat;
	}

	real3	Transform::getScale() const
	{
		return _scale;
	}
	void	Transform::setScale(const real3& scale)
	{
		_scale = scale;
	}

	real3	Transform::getPosition() const
	{
		return _position;
	}
	void	Transform::setPosition(const real3& pos)
	{
		_position = pos;
	}

	matrix4 Transform::local() const
	{
		return _local;
	}
	void	Transform::update()
	{
		_local = makeTransform(_position, _scale, _quat);
	}

	void    Transform::setAngle(float angle, float3 axis)
	{
		_quat = angleAxis(angle, axis);
	}
}