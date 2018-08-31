#pragma once

#include <Python.h>  // has to be the first included header
#include <petscsys.h>
#include <petscksp.h>
#include <iostream>
#include <vector>

#include "data_management/solution_vector_mapping.h"

class DiscretizableInTime
{
public:
  //! constructor
  DiscretizableInTime(SolutionVectorMapping solutionVectorMapping);

  // Classes that derive from DiscretizableInTime must define a constexpr nComponents that specifies the number of components in the solution field variable
  //typedef .. nComponents;

  //! initialize timestepping
  virtual void initialize() = 0;
  
  //! initialize timestepping
  virtual void initialize(double timeStepWidth) = 0;

  //! timestepping rhs function f of equation u_t = f(u,t), compute output = f(u,t) where u=input
  virtual void evaluateTimesteppingRightHandSideExplicit(Vec &input, Vec &output, int timeStepNo, double currentTime) = 0;
  
  //! timestepping rhs function f of equation u_t = f(u,t), computed in an implicit time step with equation Au^{t+1}=f^{t} where A=(I-dt*M^{-1}K). This is called when implicit Euler is used.
  virtual void evaluateTimesteppingRightHandSideImplicit(Vec &input, Vec &output, int timeStepNo, double currentTime) = 0;

  //! get the number of degrees of freedom per node which is 1 by default
  virtual int nComponentsNode();

  //! set initial values and return true or don't do anything and return false
  // this could use std::any (c++17)
  //template<typename FieldVariableType>
  //virtual bool setInitialValues(FieldVariableType &initialValues);

  //! get the names of components to be used for the solution variable
  virtual void getComponentNames(std::vector<std::string> &componentNames);
  
  //! set the subset of ranks that will compute the work
  virtual void setRankSubset(Partition::RankSubset rankSubset) = 0;
  
  //! return the solution vector mapping object, that contains information on if there are more internal values stored in the data_ object than may be needed for further computationo
  SolutionVectorMapping &solutionVectorMapping();

  //! return whether the object has a specified mesh type and is not independent of the mesh type
  virtual bool knowsMeshType() = 0;

  //! return the mesh
  virtual std::shared_ptr<Mesh::Mesh> mesh() = 0;

protected:
  SolutionVectorMapping solutionVectorMapping_;   ///< the solution vector mapping object that contains information if for further computation only a subset of the stored entries in the data_.solution vector will be needed
};
