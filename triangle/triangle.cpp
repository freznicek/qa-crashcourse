// Triangle validity tester

// Compile: g++ -std=c++11 triangle.cpp -o triangle

// includes
// --------------------------------------------------------------------------
#include <iostream>
#include <cmath>

using namespace std;

// functions
// --------------------------------------------------------------------------
int triangle_validator(double a, float b, short c) {
  int failure = 1;
  if ((a == 0)||(b == 0)||(c == 0)) {
    return(failure);
  } else if ((a < 0)||(b < 0)) {
    return(failure);
  } else {
    if ( (abs(a) + abs(b) > abs(c)) && \
         (abs(a) + abs(c) >= abs(b)) && \
         (abs(c) + abs(b) > abs(a)) ) {
      return(0);
    } else {
      return(failure);
    }
  }
}

int usage() {
  cerr << "triangle <a-side-length> <b-side-length> <c-side-length> usage:" << endl;
  cerr << "  triangle validity tester, where <?-side-length> is length of a triangle side." << endl << endl;
  cerr << "  returns ecode of 0 / 1 / 2 with meaning: valid / invalid / input error" << endl;
}


int main(int argc, char* argv[]) {
  double a_length;
  float b_length;
  short c_length;

  if( !(argc >= 4) ) {
    usage();
    return(2);
  }

  // parse the input triangle side lengths
  a_length = stod(argv[1]);
  b_length = stof(argv[2]);
  c_length = stol(argv[3]);

  // determine whether the triangle is valid (can be constructed)
  return(triangle_validator(a_length, b_length, c_length));
}

// eof
