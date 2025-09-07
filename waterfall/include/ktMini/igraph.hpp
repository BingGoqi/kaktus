#include <vector>
#include <forward_list>
namespace kaktus {
	template<typename Index> class igraph
	{
	public:
		enum nodeType
		{
			val = 0, func, ptr, fptr
		};
		typedef struct Node {
			std::forward_list<Index> perv, next;
		}Node;
	private:
		//std::vector<std::tuple<std::forward_list<int>,std::forward_list<int>>> list;
		std::vector<Node> list;
	private:
		Index addVertex()
		{
			int i = list.size();
			list.push_back(Node());
			return i;
		}
		void addEdge(Index const vout, Index const vin)
		{
			list[vout].next.push_front(vin);
			list[vin].perv.push_front(vout);
		}
	};
}