#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

class MyCtc : public ibex::Ctc
{
  public:
    MyCtc(const std::vector<ibex::IntervalVector>& M_)
      : ibex::Ctc(2), // the contractor acts on 2d boxes
        M(M_)         // attribute needed later on for the contraction
    {
    }
    void contract(ibex::IntervalVector& a)
    {
      // Insert contraction formula here (question D.2)
        IntervalVector b = IntervalVector::empty(2);
        for(auto& Mi : M)
        b |= (a & Mi);
        a = b;
    }
  protected:
    const std::vector<ibex::IntervalVector> M;
};

int main() {
    
  // Creating set M. 
  vector<IntervalVector> M;
	M.push_back(Vector({1.5,2.5}));
	M.push_back(Vector({3,1}));
	M.push_back(Vector({2,2}));
	M.push_back(Vector({2.5,3}));
	M.push_back(Vector({3.5,2}));
	M.push_back(Vector({4,1}));
	M.push_back(Vector({1.5,0.5}));

  for(auto& Mi : M) // inflate all boxes by [-0.05;0.05]
      Mi.inflate(0.05);

  // creating boxes to be contracted
  IntervalVector a1{{1.25,3},{1.6,2.75}};
  IntervalVector a2{{2,3.5},{0.6,1.2}};
  IntervalVector a3{{1.1,3.25},{0.2,1.4}};

  // draw landmarks and initial intervals
  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100,100,500,500);
  fig_map.axis_limits(0,4.5,0,3.5);

  for(auto& Mi : M)   // drawing landmarks
      fig_map.draw_box(Mi, "black[orange]");

  fig_map.draw_box(a1, "blue");
  fig_map.draw_box(a2, "green");
  fig_map.draw_box(a3, "red");

  MyCtc ctc_constell(M);
  ContractorNetwork cn;

  cn.add(ctc_constell, {a1});
  cn.add(ctc_constell, {a2});
  cn.add(ctc_constell, {a3});

  cn.contract();

  cout << "Numerical Results:" << endl;
  cout << "[a1]: " << a1 << endl;
  cout << "[a2]: " << a2 << endl;
  cout << "[a3]: " << a3 << endl;

  // fig_map.draw_box(a1, "#0074A0[#0074A077]");
  // fig_map.draw_box(a2, "#69F04B[#69F04B77]");
  fig_map.draw_box(a3, "#A02400[#A0240077]");

  /*
  Non-filled boxes are initial domains before contraction. Filled boxes are contracted domains. 
  The green one [a3] is identified since it contains only one item of the constellation M
  There is no ambiguity about the constellation point represented by the box [a3].
  */
  fig_map.show();
  vibes::endDrawing();
  
  return 0;
}
