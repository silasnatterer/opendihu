#include "solver/nonlinear.h"

#include "utility/python_utility.h"

namespace Solver
{
  
Nonlinear::Nonlinear(PyObject *specificSettings) : Solver(specificSettings)
{
  // parse options
  relativeTolerance_ = PythonUtility::getOptionDouble(specificSettings, "relativeTolerance", 1e-5, PythonUtility::Positive);

  // set up SNES object
  snes_ = std::make_shared<SNES>();
  PetscErrorCode ierr = SNESCreate (PETSC_COMM_WORLD, snes_.get()); CHKERRV(ierr);
  
  // set options from command line as specified by PETSc
  SNESSetFromOptions(*snes_);
  
  // extract linear solver context
  ksp_ = std::make_shared<KSP>();
  ierr = SNESGetKSP (*snes_, ksp_.get()); CHKERRV(ierr);
  
  // set solver type
  ierr = KSPSetType(*ksp_, KSPGMRES); CHKERRV(ierr);
  
  //                             relative tol,       absolute tol,  diverg tol.,   max_iterations
  ierr = KSPSetTolerances (*ksp_, relativeTolerance_, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT); CHKERRV(ierr);
  
  // configure to exit program when the linear solver fails
  ierr = KSPSetErrorIfNotConverged(*ksp_, PETSC_TRUE); CHKERRV(ierr);
}
  
std::shared_ptr<SNES> Nonlinear::snes()
{
  return snes_;
}

std::shared_ptr<KSP> Nonlinear::ksp()
{
  return ksp_;
}

};   //namespace