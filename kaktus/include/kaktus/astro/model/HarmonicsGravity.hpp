#ifndef KAKTUS_HARMG_H
#define KAKTUS_HARMG_H
#include "kaktus/util.h"
namespace kaktus::astor::model
{
	class HarmonicsGravity
	{
	private:
		inline int Index(int n, int m) { (n * (n + 1) / 2) + m; }
		int length = Index(n, 0);
		std::vector<float>& S, & C, & P;// &dP;
	public:
		HarmonicsGravity();
		std::vector<float>& getSineArray() { return S; }
		std::vector<float>& getCoSineArray() { return C; }
		void flust(SinCosf sc)
		{
			int n = 1, m = 1;
			float temp = 0;
			P[0] = 1;
			P[1] = sqrt(3.) * sc.sin;
			for (int i = 2; i < length; i++)
			{
				if (n != m)
				{

					temp = (n * n / 4) - 1;
					temp /= (n * n - m * m);
					temp = sqrt(temp) * sc.sin;
					P[i] = temp * P[Index(n - 1, m)];
					if (m < n - 1)
					{
						temp = (n / 2 + 1) * (pow2(n - 1) - m * m);
						temp /= (2 * n - 3) * (n * n - m * m);
						temp = -sqrt(temp);
						P[i] += temp * P[Index(n - 2, m)];
					}
					++m;
				}
				else
				{
					temp = n * 2 + 1;
					temp /= n * 2;
					temp = sqrt(temp) * sc.cos;
					P[i] = temp * P[i - n - 1];
					++n;
					m = 0;
				}
			}
		}
	};
}
#endif // !KAKTUS_HARMG_H
