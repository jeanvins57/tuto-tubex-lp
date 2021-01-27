#include <tubex.h>
#include <math.h>
#include "vibes.h"

using namespace std;
using namespace tubex;

// int main()
// {
//   Tube x(Interval(0,10), 0.01, TFunction("cos(t)+abs(t-5)*[-0.1,0.1]"));

//   vibes::beginDrawing();
//   VIBesFigTube fig("My first tube");
//   fig.add_tube(&x, "x");
//   fig.show();
//   vibes::endDrawing();

//   cout << x << endl;
// }

int main()
{
  // A1
    Interval x0(8,10);
    Interval y0(1,2);
    cout << x0/y0 << endl;

  // A2 - operators example
    cout << Interval(-2,4)*Interval(1,3) << endl;
    cout << Interval(8,10)/Interval(-1,10) << endl;
    // cout << Interval(-2,4)|Interval(6,7) << endl;
    cout << max(Interval(2,7),Interval(1,9)) << endl;
    cout << max(Interval::EMPTY_SET,Interval(1,2)) << endl;
    cout << cos(Interval()) << endl;
    cout << sqr(Interval(-1,4)) << endl;
    cout << (Interval(1,2)*Interval(-1,3))+max(Interval(1,3)&Interval(6,7),Interval(1,2)) << endl;

  // A3 - create 2d box
    IntervalVector y3{{0,M_PI},{-M_PI/6,M_PI/6}};
    cout << abs(y3) << endl;

  // A4 - distance function
    IntervalVector x{{0,0},{0,0}};
    IntervalVector b{{3,4},{2,3}};
    Function g("x[2]","b[2]","sqr((x[0]-b[0])^2+(x[1]-b[1])^2)");
    IntervalVector c = cart_prod(x,b);
    Interval d = g.eval(c);
    cout << d << endl;

  // A5 + A6 - creates view
    vibes::beginDrawing();
    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 400, 400); // position and size

    fig.draw_box(x,"red");
    fig.draw_box(b,"red");
    fig.draw_ring(0,0,d);
    // fig.draw_circle(0, 0, d.lb() );
    // fig.draw_circle(0, 0, u.lb() );

  // A7. 
    x.inflate(0.1);
    fig.draw_box(x, "green");
    fig.draw_box(b, "green");
    fig.draw_ring(0,0,d, "green");

    fig.draw_box(x+b,"yellow");

    fig.show(); // display all items of the figure
    vibes::endDrawing();

  // A8. Contractors.
    CtcFunction ctc_add(Function("x", "y", "a", "x+y-a"));


}