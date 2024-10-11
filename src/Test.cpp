//#include "tudat/simulation/environment_setup.h"
#include "tudat/simulation/simulation.h"
#include "tudat/astro/orbit_determination.h"
#define Auto main_
using namespace tudat::simulation_setup;
using namespace tudat::orbital_element_conversions;
using namespace tudat::propagators;
namespace test1 {
	void test()
	{
		BodyListSettings bls = getDefaultBodySettings({ "a","b" }, 300, 3000);
		auto blsa = bls.get("a");
		blsa->constantMass = 500;
		blsa->rotationModelSettings = simpleRotationModelSettings("SSB", "J2000", Eigen::Quaterniond(1, 2, 3, 4), 0, 1);
		blsa->gravityFieldSettings = centralGravitySettings(398601.22e9);
		blsa->ephemerisSettings = customEphemerisSettings(SimpleCustomGuidanceModel::fun);

		SystemOfBodies sob = createSystemOfBodies(bls);
		auto soba = sob.getBody("a");
		auto n = sob.getBody("a")->getGravitationalParameter();
		auto k = convertCartesianToKeplerianElements(soba->getState(), n);

		//PropagatorSettings ps = setInitialStateVectorFromParameterSet();
		AccelerationSettings as = AccelerationSettings(tudat::basic_astrodynamics::AvailableAcceleration::point_mass_gravity);
		//auto am = createAccelerationModel(soba);
		TranslationalStatePropagatorSettings tsp = translationalStatePropagatorSettings();
	}
	class SimpleCustomGuidanceModel
	{
	public:
		static Eigen::Vector6d fun(float t) {
			return Eigen::Vector6d(0, 0, t, 0, 0, 1);
		}
	};
}
auto Auto(auto Auto)
{
	return Auto;
}