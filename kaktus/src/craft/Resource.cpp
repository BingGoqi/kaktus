#include <list>
namespace kaktus::craft
{
	//caekit\craft\resource
	class KStroge
	{
	private:
		int type, id, priority;//资源种类，容器编号，优先级
		double stock, size;
	public:

	};
	class KStrogeState
	{
	private:
		std::list<KStroge> list;
	public:
	};
	class KStrogeTree
	{
	private:
		std::list<KStrogeState> list;
	public:
	};
}