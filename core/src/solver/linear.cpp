#include "solver/linear.h"

#include "utility/python_utility.h"

namespace Solver
{
  
Linear::Linear(PyObject *specificSettings) : 
  Solver(specificSettings)
{
  // parse options  
  relativeTolerance_ = PythonUtility::getOptionDouble(specificSettings, "relativeTolerance", 1e-5, PythonUtility::Positive);

  // set up KSP object
  //KSP *ksp;
  ksp_ = std::make_shared<KSP>();
  PetscErrorCode ierr = KSPCreate (PETSC_COMM_WORLD, ksp_.get()); CHKERRV(ierr);
  
  // set options from command line as specified by PETSc
  KSPSetFromOptions(*ksp_);
  
  
  // extract preconditioner context
  PC pc;
  ierr = KSPGetPC (*ksp_, &pc); CHKERRV(ierr);
  
  // set preconditioner type
  ierr = PCSetType (pc, PCNONE); CHKERRV(ierr);
  
  // set solver type
  ierr = KSPSetType(*ksp_, KSPGMRES); CHKERRV(ierr);
  
  //                                    relative tol,      absolute tol,  diverg tol.,   max_iterations
  ierr = KSPSetTolerances (*ksp_, relativeTolerance_, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT); CHKERRV(ierr);
}

std::shared_ptr<KSP> Linear::ksp()
{
  return ksp_;
}
  
};   //namespace