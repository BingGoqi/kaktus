#ifndef KAKTUS_BODIES_H
#define KAKTUS_BODIES_H
#include <forward_list>
#include "kaktus/astor/Body.h"
namespace kaktus::astor
{
	class Bodies
	{
	private:
		std::unordered_map<string, Body&> bodyMap;
		Body basic;// = Body("basic", ConstEphemeris(Eigen::Vector2cd::Zero()));
	public:
		//Body& getBody(string);
		Body& getPPB(const Body& b1, const Body& b2, double time);
		void addBody(Body& body, Body& supBody);
	};
	//static Bodies bodies;
}
#endif