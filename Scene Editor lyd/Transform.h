#ifndef _Transform_
#define _Transform_

#include "Obj.h"
#include "mathf\CELLMath.hpp"

namespace lyd
{
	class Transform : public Obj
	{
	protected:
		quatr		_quat;
		real3		_scale;
		real3		_position;
		matrix4		_local;
	public:
		Transform();
		virtual ~Transform();

		quatr	getQuat() const;
		void	setQuat(const quatr& quat);

		real3	getScale() const;
		void	setScale(const real3& scale);

		real3	getPosition() const;
		void	setPosition(const real3& pos);

		matrix4 local() const;
		void	update();

		void    setAngle(float angle, float3 axis);
	};
}

#endif // !_Transform_

