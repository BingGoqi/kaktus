#include <list>
namespace kaktus::craft
{
	//caekit\craft\resource
	class KStroge
	{
	private:
		int type, id, priority;//��Դ���࣬������ţ����ȼ�
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