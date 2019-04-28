#include "partition/partitioned_petsc_mat/partitioned_petsc_mat.h"


//! constructor, create square sparse matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
PartitionedPetscMat(std::shared_ptr<Partition::MeshPartition<RowsFunctionSpaceType>> meshPartition,
                    int nComponents, int diagonalNonZeros, int offdiagonalNonZeros, std::string name): nComponents_(nComponents)
{
  // create nComponents matrix components by calling the constructor
  matrixComponents_.reserve(MathUtility::sqr(nComponents_));
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    //matrixComponents_.push_back(PartitionedPetscMatOneComponent<RowsFunctionSpaceType,ColumnsFunctionSpaceType>(meshPartition, diagonalNonZeros, offdiagonalNonZeros, name));
    matrixComponents_.emplace_back(meshPartition, diagonalNonZeros, offdiagonalNonZeros, name);
  }
  createMatNest();
}

//! constructor, create square dense matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
PartitionedPetscMat(std::shared_ptr<Partition::MeshPartition<RowsFunctionSpaceType>> meshPartition,
                    int nComponents, std::string name): nComponents_(nComponents)
{
  // create nComponents matrix components by calling the constructor
  matrixComponents_.reserve(MathUtility::sqr(nComponents_));
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_.emplace_back(meshPartition, name);
  }
  createMatNest();
}

//! constructor, create non-square sparse matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
  PartitionedPetscMat(std::shared_ptr<Partition::MeshPartition<RowsFunctionSpaceType>> meshPartitionRows,
                      std::shared_ptr<Partition::MeshPartition<ColumnsFunctionSpaceType>> meshPartitionColumns,
                      int nComponents, int diagonalNonZeros, int offdiagonalNonZeros, std::string name): nComponents_(nComponents)
{
  // create nComponents matrix components by calling the constructor
  matrixComponents_.reserve(MathUtility::sqr(nComponents_));
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_.emplace_back(meshPartitionRows, meshPartitionColumns, diagonalNonZeros, offdiagonalNonZeros, name);
  }
  createMatNest();
}


//! constructor, create non-square dense matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
PartitionedPetscMat(std::shared_ptr<Partition::MeshPartition<RowsFunctionSpaceType>> meshPartitionRows,
                      std::shared_ptr<Partition::MeshPartition<ColumnsFunctionSpaceType>> meshPartitionColumns,
                      int nComponents, std::string name): nComponents_(nComponents)
{
  // create nComponents matrix components by calling the constructor
  matrixComponents_.reserve(MathUtility::sqr(nComponents_));
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_.emplace_back(meshPartitionRows, meshPartitionColumns, name);
  }
  createMatNest();
}


//! constructor, use provided global matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
PartitionedPetscMat(std::shared_ptr<Partition::MeshPartition<RowsFunctionSpaceType>> meshPartition,
                    Mat &globalMatrix, std::string name): nComponents_(1)
{
  // create nComponents matrix components by calling the constructor
  matrixComponents_.reserve(MathUtility::sqr(nComponents_));
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_.emplace_back(meshPartition, globalMatrix, name);
  }
  createMatNest();
}


//! wrapper of MatSetValues for a single value, sets a local value in the matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
setValue(int componentNo, PetscInt row, PetscInt col, PetscScalar value, InsertMode mode)
{
  assert(0 <= componentNo && componentNo < matrixComponents_.size());
  matrixComponents_[componentNo].setValue(row, col, value, mode);
}

//! wrapper of MatSetValues for a single value, sets a local value in the matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
setValue(PetscInt row, PetscInt col, PetscScalar value, InsertMode mode)
{
  setValue(0, row, col, value, mode);
}

//! wrapper of MatSetValues for a vector value, sets a local value in the matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
template<int nComponentsSquared>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
setValue(PetscInt row, PetscInt col, std::array<double,nComponentsSquared> value, InsertMode mode)
{
  assert(matrixComponents_.size() >= nComponentsSquared);
  for (int i = 0; i < nComponentsSquared; i++)
  {
    matrixComponents_[i].setValue(row, col, value[i], mode);
  }
}

//! wrapper of MatSetValues for a single value, sets a local value in the matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
setValues(int componentNo, PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], const PetscScalar v[], InsertMode addv)
{
  assert(0 <= componentNo && componentNo < matrixComponents_.size());
  matrixComponents_[componentNo].setValues(m, idxm, n, idxn, v, addv);
}

//! wrapper of MatSetValues for a single value, sets a local value in the matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
setValues(PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], const PetscScalar v[], InsertMode addv)
{
  setValues(0, m, idxm, n, idxn, v, addv);
}

//! wrapper of MatSetValues for a vector value, sets a local value in the matrix
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
template<int nComponentsSquared>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
setValues(PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], const std::vector<std::array<double,nComponentsSquared>> &v, InsertMode addv)
{
  assert(matrixComponents_.size() >= nComponentsSquared);
  std::vector<double> componentValues(m*n);
  for (int i = 0; i < nComponentsSquared; i++)
  {
    for (int j = 0; j < m*n; j++)
    {
      componentValues[j] = v[j][i];
    }

    matrixComponents_[i].setValues(m, idxm, n, idxn, componentValues.data(), addv);
  }
}

//! wrapper of MatZeroRowsColumns, zeros all entries (except possibly the main diagonal) of a set of local rows and columns
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
zeroRowsColumns(PetscInt numRows, const PetscInt rows[], PetscScalar diag)
{
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_[i].zeroRowsColumns(numRows, rows, diag);
  }
}

//! wrapper of MatZeroEntries, sets all entries to 0
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
zeroEntries()
{
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_[i].zeroEntries();
  }
}

//! parallel assembly of the matrix, wraps the PETSc function MatAssemblyBegin,MatAssemblyEnd, type is MAT_FLUSH_ASSEMBLY or MAT_FINAL_ASSEMBLY
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
assembly(MatAssemblyType type)
{
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    matrixComponents_[i].assembly(type);
  }
}

//! get entries from the matrix that are locally stored
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
getValues(int componentNo, PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], PetscScalar v[]) const
{
  assert(0 <= componentNo && componentNo < matrixComponents_.size());
  matrixComponents_[componentNo].getValues(m, idxm, n, idxn, v);
}

//! get entries from the matrix that are locally stored
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
getValues(PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], PetscScalar v[]) const
{
  getValues(0, m, idxm, n, idxn, v);
}

//! get entries from the matrix that are locally stored
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
template<int nComponentsSquared>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
getValues(PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], std::vector<std::array<double,nComponentsSquared>> &v) const
{
  assert(matrixComponents_.size() >= nComponentsSquared);
  std::vector<double> componentValues(m*n);
  for (int i = 0; i < nComponentsSquared; i++)
  {
    matrixComponents_[i].getValues(m, idxm, n, idxn, componentValues.data());

    for (int j = 0; j < m*n; j++)
    {
      v[j][i] = componentValues[j];
    }
  }
}

//! get entries from the matrix that are locally stored, uses the global/Petsc indexing. This is not the global natural numbering!
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
getValuesGlobalPetscIndexing(int componentNo, PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], PetscScalar v[]) const
{
  assert(0 <= componentNo && componentNo < matrixComponents_.size());
  matrixComponents_[componentNo].getValues(m, idxm, n, idxn, v);
}

//! get entries from the matrix that are locally stored, uses the global/Petsc indexing. This is not the global natural numbering!
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
getValuesGlobalPetscIndexing(PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], PetscScalar v[]) const
{
  getValuesGlobalPetscIndexing(0, m, idxm, n, idxn, v);
}

//! get entries from the matrix that are locally stored, uses the global/Petsc indexing. This is not the global natural numbering!
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
template<int nComponentsSquared>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
getValuesGlobalPetscIndexing(PetscInt m, const PetscInt idxm[], PetscInt n, const PetscInt idxn[], std::vector<std::array<double,nComponentsSquared>> &v) const
{
  assert(matrixComponents_.size() >= nComponentsSquared);
  std::vector<double> componentValues(m*n);
  for (int i = 0; i < nComponentsSquared; i++)
  {
    matrixComponents_[i].getValuesGlobalPetscIndexing(m, idxm, n, idxn, componentValues.data());

    for (int j = 0; j < m*n; j++)
    {
      v[j][i] = componentValues[j];
    }
  }
}

//! get a reference to the PETSc matrix, because there is no parallelism with UnstructuredDeformableOfDimension meshes, this is the same as valuesGlobal
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
Mat &PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
valuesLocal(int componentNo)
{
  assert(0 <= componentNo && componentNo < matrixComponents_.size());
  return matrixComponents_[componentNo].valuesLocal();
}

template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
Mat &PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
valuesGlobal(int componentNo)
{
  assert(0 <= componentNo && componentNo < matrixComponents_.size());
  return matrixComponents_[componentNo].valuesGlobal();
}

template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
Mat &PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
valuesGlobal()
{
  if (nComponents_ == 1)
  {
    return matrixComponents_[0].valuesGlobal();
  }
  return matNest_;
}

template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
createMatNest()
{
  // create nested matrix
  PetscErrorCode ierr;
  MPI_Comm mpiCommunicator = matrixComponents_[0].meshPartitionRows()->mpiCommunicator();
  std::vector<Mat> submatrices(MathUtility::sqr(nComponents_));

  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    submatrices[i] = matrixComponents_[i].valuesGlobal();
  }
  ierr = MatCreateNest(mpiCommunicator, nComponents_, NULL, nComponents_, NULL, submatrices.data(), &this->matNest_); CHKERRV(ierr);
}

//! get the mesh partition of rows
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
std::shared_ptr<Partition::MeshPartition<RowsFunctionSpaceType>> PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
meshPartitionRows()
{
  return matrixComponents_[0].meshPartitionRows();
}

//! get the mesh partion of columns
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
std::shared_ptr<Partition::MeshPartition<ColumnsFunctionSpaceType>> PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
meshPartitionColumns()
{
  return matrixComponents_[0].meshPartitionColumns();
}

//! output matrix to stream, the operator<< is also overloaded to use this method
template<typename RowsFunctionSpaceType, typename ColumnsFunctionSpaceType>
void PartitionedPetscMat<RowsFunctionSpaceType,ColumnsFunctionSpaceType>::
output(std::ostream &stream) const
{
  for (int i = 0; i < MathUtility::sqr(nComponents_); i++)
  {
    stream << "component " << i << ": ";
    matrixComponents_[i].output(stream);
  }
}

template<typename FunctionSpaceType>
std::ostream &operator<<(std::ostream &stream, const PartitionedPetscMat<FunctionSpaceType> &matrix)
{
  matrix.output(stream);
  return stream;
}
