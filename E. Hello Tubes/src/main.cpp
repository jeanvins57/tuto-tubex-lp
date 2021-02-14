#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main(){

	double dt = 0.01;
	Interval tdomain(0,5); // temporal domain [t0,tf]
	TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);
    TrajectoryVector actual_v(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt);

	Beacon b(0.5, 1); // creating landmark

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");
	fig_map.set_properties(100, 100, 600, 300);
	fig_map.add_trajectory(&actual_x, "x*", 0, 1);
	fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
	fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
	fig_map.show(0.5);                            // argument is robot size

    // Random values in [-0.1,0.1] at each dt=0.01
	RandTrajectory n(tdomain, dt, Interval(-0.2,0.2));

    // Distance signal 'actual_y' between the robot and the landmark as one Trajectory object.
	Trajectory actual_y = sqrt(pow(actual_x[0] - b.x(), 2) + pow(actual_x[1] - b.y(), 2)) + n;

    // Display the actual_y trajectory in another dedicated view with a VIBesFigTube object.
    VIBesFigTube fig_dist("Distance to the landmark");
    fig_dist.set_properties(100, 100, 600, 300);
    fig_dist.add_trajectory(&actual_y, "y*");
    fig_dist.show();

    // Initialize Tubes objects.
    TubeVector x(tdomain, dt,
            TFunction("(2*cos(t) ; sin(2*t))"));
	Tube y(actual_y, dt);

    // Adding uncertainties.
    x.inflate(0.2);
    y.inflate(0.2);

    // Display Tubes on VIBES.
        // On map.
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.show(); // this method must be called to display the added objects
        // On dist.
    fig_dist.add_tube(&y, "y");
    fig_dist.show();

    IntervalVector b1({{Interval(0.5)}, {Interval(1)}});
    TubeVector v(actual_v, dt);
    v.inflate(0.01);

    // Defining contractor.
	ContractorNetwork cn;
	cn.add(ctc::dist, {x, b1, y});
	cn.add(ctc::deriv, {x, v});
	cn.contract();
     
    fig_map.show(); // this method must be called to display the added objects
    fig_dist.show();

    vibes::endDrawing();
    return 0;
}