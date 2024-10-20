#ifndef KAKTUS_HARMG_H
#define KAKTUS_HARMG_H
#include <cmath>
#include <array>
namespace kaktus::astor::model
{
	template<int num>
	class HarmonicsGravity
	{
	private:
		inline int getIndex(int n, int m) { (n * (n + 1) >> 1) + m; }
		int length = getIndex(n, 0);
		std::array<float, length>& S, & C, & P, & dP;
		inline float D10(int n, float sf);
		inline float D11(int n, float sf);
		inline float D20(int n, float sf);
		inline float D21(int n, float sf);
		inline float D30(int n, float sf);
		inline float D31(int n, float sf);
		inline float D40(int n, float sf);
		inline float D41(int n, float sf);
	public:
		HarmonicsGravity()
		{
			S = std::array();
		}
		std::array<float, length>& getSineArray() { return S; }
		std::array<float, length>& getCoSineArray() { return C; }
		flust(float sf)
		{
			int n = 1, m = 1;
			for (int i = 2; i < length; i++)
			{
				m++;
				switch (n - m)
					case 0:
						flush(n, m, 2,sf);
						++i;
						if (i == length)break;
						++n;
						m = 0;
						flush(n, m, 1,sf);
						break;
					case 1:
						flush(n, m, 3,sf);
					default:
						flush(n, m, 4,sf);
			}
		}
		flush(int n, int m, int t,float sf)
		{
			switch t
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
		}
	};
}
#endif // !KAKTUS_HARMG_H
