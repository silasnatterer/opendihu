#pragma once

#include "utility/type_utility.h"
#include "mesh/type_traits.h"

#include <cstdlib>

/** The functions in this file model a loop over the elements of a tuple, as it occurs as FieldVariablesForOutputWriterType in all data_management classes.
 *  (Because the types inside the tuple are static and fixed at compile-time, a simple for loop c not work here.)
 *  The two functions starting with loop recursively emulate the loop. One method is the break condition and does nothing, the other method does the work and calls the method without loop in the name.
 *  FieldVariablesForOutputWriterType is assumed to be of type std::tuple<...>> where the types can be (mixed) std::shared_ptr<FieldVariable> or std::vector<std::shared_ptr<FieldVariable>>.
 * 
 *  Call ExfileWriter::outputExnode on the mesh with meshName. This outputs all field variables of the mesh to an exnode file.
 */

namespace OutputWriter
{

namespace ExfileLoopOverTuple
{
 
 /** Static recursive loop from 0 to number of entries in the tuple
 *  Stopping criterion
 */
template<typename FieldVariablesForOutputWriterType, typename AllFieldVariablesForOutputWriterType, int i=0>
inline typename std::enable_if<i == std::tuple_size<FieldVariablesForOutputWriterType>::value, void>::type
loopOutputExnode(const FieldVariablesForOutputWriterType &fieldVariables, const AllFieldVariablesForOutputWriterType &allFieldVariables, std::string meshName,
                 std::ofstream &file
)
{}

 /** Static recursive loop from 0 to number of entries in the tuple
 * Loop body
 */
template<typename FieldVariablesForOutputWriterType, typename AllFieldVariablesForOutputWriterType, int i=0>
inline typename std::enable_if<i < std::tuple_size<FieldVariablesForOutputWriterType>::value, void>::type
loopOutputExnode(const FieldVariablesForOutputWriterType &fieldVariables, const AllFieldVariablesForOutputWriterType &allFieldVariables, std::string meshName, 
                 std::ofstream &file);

/** Loop body for a tuple element
 */
template<typename VectorType, typename FieldVariablesForOutputWriterType>
typename std::enable_if<TypeUtility::isTuple<VectorType>::value, bool>::type
outputExnode(VectorType currentFieldVariableGradient, const FieldVariablesForOutputWriterType &fieldVariables, std::string meshName, 
             std::ofstream &file);

/** Loop body for a vector element
 */
template<typename VectorType, typename FieldVariablesForOutputWriterType>
typename std::enable_if<TypeUtility::isVector<VectorType>::value, bool>::type
outputExnode(VectorType currentFieldVariableGradient, const FieldVariablesForOutputWriterType &fieldVariables, std::string meshName, 
             std::ofstream &file);

 /**  Loop body for a pointer element
 */
template<typename CurrentFieldVariableType, typename FieldVariablesForOutputWriterType>
typename std::enable_if<!TypeUtility::isTuple<CurrentFieldVariableType>::value && !TypeUtility::isVector<CurrentFieldVariableType>::value
  && !Mesh::isComposite<CurrentFieldVariableType>::value, bool>::type
outputExnode(CurrentFieldVariableType currentFieldVariable, const FieldVariablesForOutputWriterType &fieldVariables, std::string meshName, 
             std::ofstream &file);

/** Loop body for a field variables with Mesh::CompositeOfDimension<D>
 */
template<typename CurrentFieldVariableType, typename FieldVariablesForOutputWriterType>
typename std::enable_if<Mesh::isComposite<CurrentFieldVariableType>::value, bool>::type
outputExnode(CurrentFieldVariableType currentFieldVariable, const FieldVariablesForOutputWriterType &fieldVariables, std::string meshName,
             std::ofstream &file);

}  // namespace ExfileLoopOverTuple

}  // namespace OutputWriter

#include "output_writer/exfile/loop_output_exnode.tpp"
