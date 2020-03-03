#ifndef _Material_
#define _Material_

#include "Obj.h"
#include "mathf\CELLMath.hpp"

namespace lyd
{
	class Shader;
	class TextureMgr;
}

namespace lyd
{
	class Material : public Obj
	{
	protected:
		Shader*		_shader = nullptr;
		float4		_color;

		TextureMgr*	_texMgr = nullptr;

		unsigned long	_currentTime;
		
	public:
		Material(Shader* shader);
		Material();

		template <class T>
		Material()
		{
			_shader = new T();
		}

		virtual ~Material();

		virtual void setName(const char* name);

		Shader* getShader() const;

		void	setShader(Shader* shader);

		virtual bool	begin();

		virtual void	end();

		void	setMVP(matrix4& m, matrix4& v, matrix4& p);

		void	setCameraPos(float3& cameraPos);

		void	setColor(float4& color);
		void	setColor(float r, float g, float b, float a);

	};
}

#endif // !_Material_



