#ifndef _Singleton_
#define _Singleton_

namespace lyd
{
	template<typename T>
	class Singleton
	{
	public:
		static T& Instance()
		{
			static T _instance;
			return _instance;
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator= (const Singleton&) = delete;

		virtual ~Singleton() { }

	protected:
		Singleton() { }
	};
}

#endif // !_Singleton_


