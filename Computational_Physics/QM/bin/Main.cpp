#include "Schrodinger.hpp"
#include "Potentials.hpp"
#include "InitialConditions.hpp"
#include <stdlib.h>
#include <armadillo>
#include <cmath>

using namespace std;

//Input: executable, discretization, initial condition, potential
int main(int argc, char* argv[]){
  if(argc != 4){cout << "Wrong arguments" << endl; return 1;}

  //Potentials
  FreeParticalPotential freePartical;
  BarrierPotential barrier;

  //Initial contidions
  Dirac dirac;
  FirstEigenmode mode;
  FirstTwoEigenmode first_two_modes;
  InitialCondition* impuls = 0;

  //Activate initial condition from input
  string initial(argv[2]);
  if(initial == "dirac"){
    impuls = &dirac;
  }else if(initial == "first_mode"){
    impuls = &mode;
  }else if(initial == "first_two"){
    impuls = &first_two_modes;
  }
  else{
    cout << "Unknown initial condition" << endl; return 1;
  }

  //Create an instance
  Schrodinger schrodinger(atoi(argv[1]));

  //Activate potential from input
  string potential(argv[3]);
  if(potential == "free"){
    schrodinger.initDiagonals( freePartical );
  }else if(potential == "barrier"){
    schrodinger.initDiagonals( barrier );
  }

  //Get eigenvectors and values from system
  schrodinger.eigenvalueSolver();

  //Normalize eigenvectors
  schrodinger.normalizeEigenvectors();
  //Project initial condition onto set of eigenstates
  schrodinger.project(*impuls);

  //Final solution
  arma::cx_vec solution;
  solution.set_size( schrodinger.alpha_coeff.n_elem );

  //Time steps
  double nt = 150;
  double dt = 0.002;
  arma::mat time_evolution( solution.n_elem, nt );
  time_evolution.fill(0);

  //Normalize probability distribution function
  double sum_alpha_squared = arma::accu(arma::pow(schrodinger.alpha_coeff,2));

  //Time evolution
  for(int i = 0; i<nt; i++){
    double t = i*dt;
    solution.fill(0.0);
    for(int j = 0; j < schrodinger.alpha_coeff.n_elem; j++){
      arma::vec eigenvector = schrodinger.eigenvectors.col(j);
      solution += schrodinger.getEigenvectorWithTime(eigenvector, t, j)*schrodinger.alpha_coeff(j);
    }
    time_evolution.col(i) = arma::pow(arma::abs(solution), 2)/sum_alpha_squared;
  }

  //Save result
  time_evolution.save("time_evolution.csv", arma::csv_ascii);
  arma::vec psi = arma::pow(arma::abs(solution), 2) / sum_alpha_squared;
  psi.save("psi.csv", arma::csv_ascii);

  return 0;
};
