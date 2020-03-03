#ifndef _Obj_
#define _Obj_

namespace lyd
{
	class Obj
	{
	protected:
		char	_name[64];

	public:
		Obj();
		virtual ~Obj();

		virtual void setName(const char* name);

		//调用者释放空间
		char* getName();
	};
}
#endif // !_Obj_



