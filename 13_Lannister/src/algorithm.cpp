{\rtf1\ansi\ansicpg1252\cocoartf1348\cocoasubrtf170
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural

\f0\fs24 \cf0 #include <limits>\
#include <iostream>\
#include <iomanip>\
#include <string>\
#include <cmath>\
#include <algorithm>\
#include <vector>\
\
#include <CGAL/QP_models.h>\
#include <CGAL/QP_functions.h>\
#include <CGAL/Gmpz.h>\
\
// choose input type (input coefficients must fit)\
typedef long IT;\
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)\
typedef CGAL::Gmpz ET;\
\
// program and solution types\
typedef CGAL::Quadratic_program<IT> Program;\
typedef CGAL::Quadratic_program_solution<ET> Solution;\
\
using namespace std;\
\
long round_to_long(const CGAL::Quotient<ET> & x)\{\
  double y = ceil(CGAL::to_double(x));\
  while(y < x) y++;\
  while(y-1 >= x) y--;\
  return (long) y;\
\}\
\
void testcase() \{\
  int n, m; long s;\
  cin >> n >> m >> s;\
\
  Program lp (CGAL::SMALLER, false, 0, false, 0); \
  //We use the following variables:\
  // v0 = b_s, v1 = c_s, v2 = c_w, v4 = d;\
  int bs = 0; int cs = 1; int cw = 2; int d = 4;\
  \
  //sums for final check length of all sewage pipes:\
  long sum_x = 0; long sum_y = 0;\
  int con_nr = 0;\
  \
  for (int i = 0; i < n; i++)\{\
    long x, y; std::cin >> x >> y;\
    lp.set_a(bs, con_nr, -y); lp.set_a(cs, con_nr, -1); lp.set_a(cw, con_nr, 0); lp.set_a(d, con_nr, 0); lp.set_b(con_nr, -x); con_nr++;\
    lp.set_a(bs, con_nr, x); lp.set_a(cs, con_nr, 0); lp.set_a(cw, con_nr, 1); lp.set_a(d, con_nr, -1); lp.set_b(con_nr, -y); con_nr++; \
    lp.set_a(bs, con_nr, -x); lp.set_a(cs, con_nr, 0); lp.set_a(cw, con_nr, -1); lp.set_a(d, con_nr, -1); lp.set_b(con_nr, y); con_nr++;\
    sum_x -= x;\
    sum_y += y;\
  \}\
  \
  for (int i = 0; i < m; i++)\{\
    long x, y; std::cin >> x >> y;    \
    lp.set_a(bs, con_nr, y); lp.set_a(cs, con_nr, 1); lp.set_a(cw, con_nr, 0); lp.set_a(d, con_nr, 0); lp.set_b(con_nr, x); con_nr++;\
    lp.set_a(bs, con_nr, x); lp.set_a(cs, con_nr, 0); lp.set_a(cw, con_nr, 1); lp.set_a(d, con_nr, -1); lp.set_b(con_nr, -y); con_nr++; \
    lp.set_a(bs, con_nr, -x); lp.set_a(cs, con_nr, 0); lp.set_a(cw, con_nr, -1); lp.set_a(d, con_nr, -1); lp.set_b(con_nr, y); con_nr++;\
    sum_x += x;\
    sum_y -= y;\
  \}\
  \
  lp.set_l(d, true);\
  lp.set_c(d, 1);\
  \
  Solution sol = CGAL::solve_linear_program(lp, ET());\
  if (sol.is_infeasible()) \{\
    std::cout << "Yuck!" << std::endl;\
  \} else \{\
    if (s == -1)\{\
      std::cout << round_to_long(sol.objective_value()) << std::endl;\
    \} else \{\
      //This is the case where we have to check if there is enough money for the sewage pipes\
      //we have to add the additional cost constraint and recalculate the lp\
      lp.set_a(bs, con_nr, sum_y); lp.set_a(cs, con_nr, n-m); lp.set_a(cw, con_nr, 0); lp.set_a(d, con_nr, 0); lp.set_b(con_nr, s-sum_x);\
      sol = CGAL::solve_linear_program(lp, ET());\
      if (sol.is_infeasible())\{\
        std::cout << "Bankrupt!" << std::endl;\
      \} else \{\
        std::cout << round_to_long(sol.objective_value()) << std::endl;\
      \}\
    \}\
  \}\
  return;\
\}\
\
int main() \{\
  std::ios_base::sync_with_stdio(false);\
  int t;\
  std::cin >> t;\
  for (int i = 0; i < t; ++i) testcase();\
\}}