#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

    Vector x_truth({2.,1.,M_PI/6}); // actual state vector (pose)
    Vector y_truth({6.,M_PI/6});
    Vector m_truth({5.,6.2});

    // Creating bounded states 
    IntervalVector x(3);
    IntervalVector y(y_truth);
    IntervalVector m=m_truth;
    x[2] = Interval(x_truth[2]); // actual heading is precisely know (compass)
	
    y[0].inflate(0.3); // uncertainties related to the measurement
	y[1].inflate(0.1);
	m.inflate(0.2); // uncertainties related to the landmark

	// Intermediate variables
	Interval theta;
	IntervalVector d(2);

	// Functions.
	CtcFunction ctc_add(Function("x1", "x2", "x3", "x1+x2-x3"));
	CtcFunction ctc_sub(Function("x1", "x2", "x3", "x1-x2-x3"));

    // Adding contractor.
    ContractorNetwork cn;
	cn.add(ctc_add, {y[1], x[2], theta});
	cn.add(ctc_sub, {m[0], x[0], d[0]});
	cn.add(ctc_sub, {m[1], x[1], d[1]});
	cn.add(ctc::polar, {d, y[0], theta});

    // Solve the problem.
	cn.contract();

    vibes::beginDrawing();

    // Display vehicle and landmark with VIBES.
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(100,100,500,500);
    fig_map.axis_limits(0,7,0,7);
    fig_map.draw_vehicle(x_truth, 1);
    fig_map.draw_box(m, "red");
    fig_map.draw_box(x.subvector(0,1)); // does not display anything if unbounded
    fig_map.draw_pie(x_truth[0],x_truth[1],(Interval(0.1)|y[0]),y[1]+x_truth[2],"lightGray"); // display the range-and-bearing measurement with its uncertainties.
  	fig_map.draw_pie(x_truth[0],x_truth[1],y[0],y[1]+x_truth[2]);
    fig_map.show();

    vibes::endDrawing();
    return EXIT_SUCCESS;
}