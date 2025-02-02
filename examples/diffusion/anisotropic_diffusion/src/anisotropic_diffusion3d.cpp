#include <iostream>
#include <cstdlib>

#include "opendihu.h"

int main(int argc, char *argv[])
{
  // 1D diffusion equation du/dt = c du^2/dx^2
  
  // initialize everything, handle arguments and parse settings from input file
  DihuContext settings(argc, argv);
  
  TimeSteppingScheme::ImplicitEuler<
    SpatialDiscretization::FiniteElementMethod<
      Mesh::StructuredRegularFixedOfDimension<3>,
      BasisFunction::LagrangeOfOrder<1>,
      Quadrature::Gauss<3>,
      Equation::Dynamic::AnisotropicDiffusion
    >
  > problem(settings);
  
  problem.run();
  
  return EXIT_SUCCESS;
}
