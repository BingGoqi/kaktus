#include "kaktus/astor/model/Itrf.h"
namespace kaktus::astor::model
{
	void Itrf::setNPlot(const Eigen::Vector3d& np,bool vp)
	{
		this->vp = vp;
		//Eigen::Vector3d temp = np/np.size();
		if (vp) {
			double
				a = np(0),
				b = np(1),
				c = np(2),
				d = 1 / sqrt((a * a + c * c)),
				e = -a * d;
			d *= c;
			mat.Zero();
			mat.col(2) = np;
			mat(0, 0) = d;
			mat(0, 2) = e;
			mat.col(1) = mat.col(2) * mat.col(0);
		}
		else
		{
			mat.Identity();
			mat.col(2) = np;
			mat.col(1) = mat.col(2) * mat.col(0);
			mat.col(1).normalize();
			mat.col(0) = mat.col(1) * mat.col(2);
		}
		imat = mat.transpose();
	}
	void Itrf::S2I(Eigen::Vector3d& s)
	{
		s *= mat;
	}
	void Itrf::I2S(Eigen::Vector3d& i)
	{
		i *= imat;
	}
}