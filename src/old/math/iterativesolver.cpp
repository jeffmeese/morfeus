#include "math/iterativesolver.h"
#include "math/functions.h"

#include <iostream>
#include <fstream>
#include <ctime>

using namespace math;

IterativeSolver::IterativeSolver()
{
  mShowStatus = false;
  mMinIterations = 2;
  mMaxIterations = 5000;
  mTolerance = 0.0001;
  mSolutionTolerance = -1.0;
}

bool IterativeSolver::converged() const
{
  return mConverged;
}

size_t IterativeSolver::getMinIterations() const
{
  return mMinIterations;
}

size_t IterativeSolver::getMaxIterations() const
{
  return mMaxIterations;
}

double IterativeSolver::getTolerance() const
{
  return mTolerance;
}

void IterativeSolver::setMinIterations(size_t iterations)
{
  mMinIterations = iterations;
}

void IterativeSolver::setMaxIterations(size_t iterations)
{
  mMaxIterations = iterations;
}

void IterativeSolver::setTolerance(double tolerance)
{
  mTolerance = tolerance;
}

void IterativeSolver::showStatus(bool show)
{
  mShowStatus = show;
}

std::valarray<IterativeSolver::dcomplex> IterativeSolver::bicg(const dcomplex_array & rhs, const dcomplex_array & initial, const MatrixVectorMultiply<dcomplex> & mvm)
{
  static const dcomplex zero(0.0,0.0);

  // Resize the local arrays
  size_t unknowns = rhs.size();
  std::valarray<dcomplex> residual(zero, unknowns);
  std::valarray<dcomplex> search(zero, unknowns);
  std::valarray<dcomplex> temp(zero, unknowns);
  std::valarray<dcomplex> precond(zero, unknowns);
  std::valarray<dcomplex> best_guess(zero, unknowns);
  std::valarray<dcomplex> solution(zero, unknowns);

  // Do the initial multiplication
  best_guess = initial;
  temp = mvm.compute(best_guess);

  // Compute the initial residual and search vectors
  residual = rhs - temp;
  search = mvm.precondition(residual);
  precond = search;

  // Compute the zeroth order iterates
  dcomplex old_residual_norm = math::innerProduct(residual, precond);

  // Compute the inner product of the excitation
  dcomplex inner_prod = math::innerProduct(rhs, math::conj(rhs));
  double f_norm = sqrt(inner_prod.real());

  // Initialize the solver
  mConverged = false;
  size_t counter = 0;
  double min_error = pow(9.9,9.0);
  double error = pow(1.0,10.0);

    // Begin the iterations
  while (!mConverged && (counter < mMaxIterations || counter < mMinIterations)) {
    counter++;

    // Compute the temporary vector product y*search for the FEM system
    temp = mvm.compute(search);

    // Compute the solution weight function
    dcomplex an = old_residual_norm / math::innerProduct(temp, search);

    // Update the result
    solution += an*search;
    residual -= an*temp;
    precond = mvm.precondition(residual);

    // Compute the search vector weight function
    dcomplex new_residual_norm = math::innerProduct(residual, precond);
    dcomplex cn = new_residual_norm / old_residual_norm;

    // Update the search vector
    search = precond + cn*search;

    // Update old weight and error measure for the next iteration
    old_residual_norm = new_residual_norm;

    // Calculate the error
    inner_prod = math::innerProduct(residual, math::conj(residual));
    error = sqrt(inner_prod.real()) / f_norm;

    // Update best guess
    if (error < min_error && counter > mMinIterations) {
      min_error = error;
      best_guess = solution;
    }

    if (mShowStatus && (counter == 1 || counter % 100 == 0) ) {
      std::cout << "\t" << counter << " " << error << " " << min_error << std::endl;
    }

    // Check error termination condition
    if (error <= mTolerance) {
        mConverged = true;
        mSolutionTolerance = error;
    }
  }
    
  if (!mConverged) {
    std::cout << "System not converged" << std::endl;
		solution = best_guess + initial;
	}

	if (mConverged) {
		solution = solution + initial;

    if (mShowStatus)
      std::cout << "System converged to tolerance " << error << " at " << counter << " iterations" << std::endl;
	}

	return solution;
}

IterativeSolver::dcomplex_array IterativeSolver::bicgstab(const dcomplex_array & rhs, const dcomplex_array & initial, const MatrixVectorMultiply<dcomplex> & mvm)
{
  static const dcomplex zero(0.0,0.0);
  static const dcomplex one(1.0,0.0);

  dcomplex innerProd = math::innerProduct(rhs, math::conj(rhs));
  double fNorm = sqrt(innerProd.real());

  dcomplex alpha = one, beta = zero, rho = one, rhop = one, omega = one;

  dcomplex_array s(zero, rhs.size());
  dcomplex_array t(zero, rhs.size());
  dcomplex_array p(zero, rhs.size());
  dcomplex_array v(zero, rhs.size());
  dcomplex_array solution(zero, rhs.size());
  dcomplex_array bestGuess(initial);

  //dcomplex_array r = rhs - mvm.compute(bestGuess);
  dcomplex_array r(rhs);
  dcomplex_array rhat(r);

  double minError = -1;
  double error = -1;
  int counter = 0;
  mConverged = false;
  while (!mConverged && (++counter < mMaxIterations || counter < mMinIterations)) {
    rhop = rho;
    rho = math::innerProduct(rhat, r);
    beta = (rho/rhop)*(alpha/omega);
    p = r + beta*(p - omega*v);
    v = mvm.compute(p);
    alpha = rho / math::innerProduct(rhat, v);
    s = r - alpha*v;
    t = mvm.compute(s);
    omega = math::innerProduct(t, s) / math::innerProduct(t, t);
    solution = solution + alpha*p + omega*s;
    r = s - omega*t;

    innerProd = math::innerProduct(r, math::conj(r));
    error = sqrt(innerProd.real()) / fNorm;
    if (counter == 1 || error < minError) {
      minError = error;
      bestGuess = solution;
    }

    if (error <= mTolerance) {
      mConverged = true;
    }

    if (mShowStatus && (counter == 1 || counter % 100 == 0) ) {
      std::cout << "\t" << counter << " " << error << " " << minError << std::endl;
    }
  }

  if (!mConverged) {
    std::cout << "System not converged" << std::endl;
    solution = bestGuess;
  }

	if (mConverged) {
		solution = solution + initial;

    if (mShowStatus)
      std::cout << "System converged to tolerance " << error << " at " << counter << " iterations" << std::endl;
  }

  mSolutionTolerance = minError;

  return solution;
}

std::valarray<IterativeSolver::dcomplex> IterativeSolver::cgs(const std::valarray<dcomplex> & rhs, const MatrixVectorMultiply<dcomplex> & mvm)
{
  static const dcomplex zero(0.0,0.0);
  static const dcomplex one(1.0,0.0);
    
  std::valarray<dcomplex> rc, rhatc, vc, qc, wc, pc, uc, best_guess;
  dcomplex alphac, betac, rhoc, old_rhoc;
  dcomplex inner_prod;

  size_t rows = rhs.size();
  rc.resize(rows, zero);
  rhatc.resize(rows, zero);
  vc.resize(rows, zero);
  qc.resize(rows, zero);
  wc.resize(rows, zero);
  pc.resize(rows, zero);
  uc.resize(rows, zero);
  best_guess.resize(rows, zero);

  srand(static_cast<unsigned int>(time(NULL)));
  for (size_t i = 0; i < rows; i++) {
    double ran = static_cast<double>(rand());
    ran /= RAND_MAX;
    rhatc[i] = dcomplex(ran, 0.0);
  }
    
  // Clear the result vector
  std::valarray<dcomplex> solution(rows);
  solution = zero;

  rc = rhs;
  rhoc = one;
  alphac = one;

  inner_prod = zero;
  for (size_t i = 0; i < rows; i++) {
    inner_prod += rhs[i] * conj(rhs[i]);
  }
  double f_norm = sqrt(inner_prod.real());
    
  double error = 1e10, min_error = 9e9;
  size_t counter = 0;
  mConverged = false;
  while (!mConverged && (counter < mMaxIterations || counter < mMinIterations)) {
    counter++;

    old_rhoc = rhoc;
    inner_prod = zero;
    for (size_t i = 0; i < rows; i++)
      inner_prod += rhatc[i]*rc[i];

    rhoc = inner_prod;
    betac = rhoc / old_rhoc;

    for (size_t i = 0; i < rows; i++) {
      uc[i] = rc[i] + betac*qc[i];
      pc[i] = uc[i] + betac*(qc[i] + betac*pc[i]);
    }

    vc = mvm.compute(pc);

    inner_prod = zero;
    for (size_t i = 0; i < rows; i++)
      inner_prod += rhatc[i]*vc[i];

    alphac = rhoc / inner_prod;

    for (size_t i = 0; i < rows; i++) {
      qc[i] = uc[i] - alphac*vc[i];
      wc[i] = uc[i] + qc[i];
      solution[i] += alphac*wc[i];
    }

    uc = mvm.compute(wc);

    for (size_t i = 0; i < rows; i++) {
      rc[i] -= alphac*uc[i];
    }

    inner_prod = zero;
    for (size_t i = 0; i < rows; i++) {
      inner_prod += rc[i]*conj(rc[i]);
    }
    error = sqrt(inner_prod.real()) / f_norm;

    // Update best guess
    if (error < min_error && counter > mMinIterations) {
      min_error = error;
      best_guess = solution;
    }

    if (mShowStatus) {
      if (counter == 1 || counter % 100 == 0) {
        std::cout << "\t" << counter << " " << error << " " << min_error << std::endl;
      }
    }

    // Check error termination condition
    if (error <= mTolerance)
        mConverged = true;
  }
    
  if (!mConverged) {
    std::cout << "System not converged" << std::endl;
		solution = best_guess;
	}

	if (mConverged && mShowStatus) {
		if (mShowStatus)
			std::cout << "System converged to tolerance " << error << " at " << counter << " iterations" << std::endl;
	}

	return solution;
}
