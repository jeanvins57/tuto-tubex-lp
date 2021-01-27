#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {
	// Truth (unknown pose)
	Vector x_truth({0.,0.,M_PI/6.}); // (x,y,heading)

	// Creating random map of landmarks
	int nb_landmarks = 3;
	IntervalVector map_area(2, Interval(-8.,8.));
	vector<IntervalVector> v_b = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

	// The following function generates a set of [range]x[bearing] values
	vector<IntervalVector> v_obs = DataLoader::generate_static_observations(x_truth, v_b, false);

	// We keep range-only observations from v_obs, and add uncertainties
	vector<Interval> v_d;
	for(auto& obs : v_obs)
		v_d.push_back(obs[0].inflate(0.1)); // adding uncertainties: [-0.1,0.1]

	// Set of feasible positions for x: x ϵ [-∞,∞]×[-∞,∞]
	IntervalVector x(2);

	CtcFunction ctc_dist(Function("x[2]", "b[2]", "r", "sqrt((x[0]-b[0])^2+(x[1]-b[1])^2)-r")); // B1.

	ContractorNetwork c;
	for(int i = 0 ; i < v_b.size() ; i++)
        c.add(ctc_dist, {x, v_b[i], v_d[i]});
	c.contract();

	vibes::beginDrawing();

	VIBesFigMap fig("Map");
	fig.set_properties(50, 50, 600, 600);

	for(const auto& b : v_b)
		fig.add_beacon(b.mid(), 0.2);

	for(int i = 0 ; i < nb_landmarks ; i++)
		fig.draw_ring(v_b[i][0].mid(), v_b[i][1].mid(), v_d[i], "gray");

	fig.draw_box(x); // estimated position
	fig.draw_vehicle(x_truth, 0.7); // last param: vehicle size
	fig.show();

	vibes::endDrawing();
}
