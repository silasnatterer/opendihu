#include "time_stepping_scheme/explicit_euler.h"

#include <Python.h>  // has to be the first included header

#include "utility/python_utility.h"
#include "utility/petsc_utility.h"

namespace TimeSteppingScheme
{

template<typename DiscretizableInTime>
ExplicitEuler<DiscretizableInTime>::ExplicitEuler(DihuContext context) :
  TimeSteppingSchemeOde<DiscretizableInTime>(context, "ExplicitEuler")
{
  this->data_ = std::make_shared <Data::TimeStepping<typename DiscretizableInTime::BasisOnMesh, DiscretizableInTime::nComponents()>>(context); // create data object for explicit euler
  PyObject *topLevelSettings = this->context_.getPythonConfig();
  this->specificSettings_ = PythonUtility::getOptionPyObject(topLevelSettings, "ExplicitEuler");
  this->outputWriterManager_.initialize(this->specificSettings_);
}

template<typename DiscretizableInTime>
void ExplicitEuler<DiscretizableInTime>::advanceTimeSpan()
{
  // compute timestep width
  double timeSpan = this->endTime_ - this->startTime_;
  double timeStepWidth = timeSpan / this->numberTimeSteps_;

  LOG(DEBUG) << "ExplicitEuler::advanceTimeSpan, timeSpan="<<timeSpan<<", timeStepWidth="<<timeStepWidth
    <<" n steps: "<<this->numberTimeSteps_;

  // loop over time steps
  double currentTime = this->startTime_;
  for(int timeStepNo = 0; timeStepNo < this->numberTimeSteps_;)
  {
    if (timeStepNo % this->timeStepOutputInterval_ == 0)
     LOG(INFO) << "Timestep "<<timeStepNo<<"/"<<this->numberTimeSteps_<<", t="<<currentTime;

    //LOG(DEBUG) << "solution before integration: " << PetscUtility::getStringVector(this->data_->solution().values());

    // advance computed value
    // compute next delta_u = f(u)
    this->discretizableInTime_.evaluateTimesteppingRightHandSide(
      this->data_->solution().values(), this->data_->increment().values(), timeStepNo, currentTime);

    // integrate, y += dt * delta_u
    VecAXPY(this->data_->solution().values(), timeStepWidth, this->data_->increment().values());

    LOG_EVERY_N(1000,DEBUG) << "dt=" << timeStepWidth << ", delta_u=" << PetscUtility::getStringVector(this->data_->increment().values())
      << ", new y=" << PetscUtility::getStringVector(this->data_->solution().values());
    
    // advance simulation time
    timeStepNo++;
    currentTime = this->startTime_ + double(timeStepNo) / this->numberTimeSteps_ * timeSpan;

    //LOG(DEBUG) << "solution after integration: " << PetscUtility::getStringVector(this->data_->solution().values());
    // write current output values
    this->outputWriterManager_.writeOutput(*this->data_, timeStepNo, currentTime);

    //this->data_->print();
  }
}

template<typename DiscretizableInTime>
void ExplicitEuler<DiscretizableInTime>::run()
{
  TimeSteppingSchemeOde<DiscretizableInTime>::run();
}
} // namespace TimeSteppingScheme