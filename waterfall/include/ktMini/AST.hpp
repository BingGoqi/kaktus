#include <string>
#include<ktMini/igraph.hpp>
namespace kaktus
{
	class AST:igraph<int>
	{
	private:

	public:
		int addVal(std::wstring&& name)
		{

		}
		int addsFunc(std::wstring&& telex);
		int useFunc(int fid, std::vector<int>&& in, std::vector<int>&& out);
	};
}
int testFunc()
{
	kaktus::AST ast;
	int f = ast.addsFunc(L"+");
	int a = ast.addVal(L"a"),
		b = ast.addVal(L"b"),
		c = ast.addVal(L"c");
	ast.useFunc(f, { a,b }, { c });
	return 0;
}
/*
* 
*/