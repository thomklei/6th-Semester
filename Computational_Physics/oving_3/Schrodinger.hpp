#ifndef SCHRODINGER
#define SCHRODINGER
#include <armadillo>
#include <cmath>
#include <complex>
typedef std::complex<double> cdouble;

enum class InitialConditions_t{
  EIGENMODE, DRIAC
};

class Schrodinger{
public:
  Schrodinger(int nx);
  Schrodinger(){};
  template<class V>
  void initDiagonals(const V &potential);
  void eigenvalueSolver();
  template<class Function>
  void project(const Function &condition);
  void normalizeEigenvectors();
  template<class V>
  void euler(const V &potential);
  double getMinEigenvalue();
  double getMaxEigenvalue();
  double dx;
  arma::vec alpha_coeff;
  arma::vec diagonal;
  arma::mat eigenvectors;
  arma::cx_vec& getEigenvectorWithTime(arma::vec &eigenvector, double t, int eigenvalue);

private:
  template<class V>
  void buildDiag(const V &potential);
  void save(arma::mat &eigenvectors, arma::vec &eigenvalues);
  void setInitialCondition();
  void buildSubDiag();
  double normalization_factor{1};
  double interpolation(arma::vec &eigenvector);
  double trapezoidal(const arma::vec &eigenvector);
  arma::cx_vec last_psi;
  arma::vec sub_diagonal;
  arma::vec total_solution;
  arma::cx_vec complex_eigenvector;
  arma::cx_vec getComplexEigenvector();
  arma::mat normalized_eigvecs;
  cdouble IMUNIT;

};

#include "Schrodinger.tpp"
#endif
