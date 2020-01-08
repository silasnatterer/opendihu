# isotropic Mooney Rivlin
import numpy as np
import sys, os

# number of elements
nx = 1    # 2
ny = 1    # 2
nz = 3    # 5

# boundary conditions (for quadratic elements)
dirichlet_bc = {}

xpos = 0.0
ypos = 0.0
zpos = 0.0

# fix bottom plane in z direction, displacements are quadratic
for j in range(0,2*ny+1):
  for i in range(0,2*nx+1):
    dirichlet_bc[j*(2*nx+1) + i] = [None,None,zpos,None,None,None]   # displacements and velocity

if False:
  # left plane
  for k in range(0,2*nz+1):
    for j in range(0,2*ny+1):
      dirichlet_bc[k*(2*nx+1)*(2*ny+1) + j*(2*nx+1)] = [xpos,None,None]

  # front plane
  for k in range(0,2*nz+1):
    for i in range(0,2*nx+1):
      dirichlet_bc[k*(2*nx+1)*(2*ny+1) + i] = [None,ypos,None]

  # vertical edge
  for k in range(0,2*nz+1):
    dirichlet_bc[k*(2*nx+1)*(2*ny+1)] = [xpos,ypos,None]

if True:
  # fix points on bottom horizontal edge in y and z direction
  for i in range(0,2*nx+1):
    dirichlet_bc[i] = [None,ypos,zpos]

if True:
  # horizontal edge
  for j in range(0,2*ny+1):
    dirichlet_bc[j*(2*nx+1)] = [xpos,None,zpos]

# fix corner completely
dirichlet_bc[0] = [xpos,ypos,zpos]

# Neumann boundary conditions, specify upward force for top elements, slightly in y-direction
neumann_bc = [{"element": (nz-1)*nx*ny + j*nx + i, "constantVector": [0,1e-1,5e-1], "face": "2+"} for j in range(ny) for i in range(nx)]

dt = 1e-1
end_time = 100
output_interval = dt

#dirichlet_bc = {}
#neumann_bc = []

config = {
  "scenarioName": "3d_box",
  "DynamicHyperelasticitySolver": {
    #"numberTimeSteps": 1,
    "endTime": end_time,
    "timeStepWidth": dt,    
    "durationLogKey": "nonlinear",
    "timeStepOutputInterval": 1,
    
    #"materialParameters": [1.5,2.0],
    "materialParameters": [0.0,1.0],
    "density": 1.0,
    "displacementsScalingFactor": 1.0,   # scaling factor for displacements, set to sth. other than 1 only to increase visual appearance for very small displacements
    "residualNormLogFilename": "log_residual_norm.txt",
    "useAnalyticJacobian": True,
    "useNumericJacobian": False,   # only works with non-nested matrices, if both numeric and analytic are enable, it uses the analytic for the preconditioner and the numeric as normal jacobian
      
    "dumpDenseMatlabVariables": False,   # extra output of matlab vectors, x,r, jacobian matrix, very slow
    # if useAnalyticJacobian,useNumericJacobian and dumpDenseMatlabVariables all all three true, the analytic and numeric jacobian matrices will get compared to see if there are programming errors for the analytic jacobian
    
    # mesh
    "nElements": [nx, ny, nz],
    "inputMeshIsGlobal": True,
    "physicalExtent": [nx, ny, nz],
    
    # nonlinear solver
    "relativeTolerance": 1e-10,         # 1e-10 relative tolerance of the linear solver
    "solverType": "preonly",            # type of the linear solver: cg groppcg pipecg pipecgrr cgne nash stcg gltr richardson chebyshev gmres tcqmr fcg pipefcg bcgs ibcgs fbcgs fbcgsr bcgsl cgs tfqmr cr pipecr lsqr preonly qcg bicg fgmres pipefgmres minres symmlq lgmres lcd gcr pipegcr pgmres dgmres tsirm cgls
    "preconditionerType": "lu",         # type of the preconditioner
    "maxIterations": 1e4,               # maximum number of iterations in the linear solver
    "snesMaxFunctionEvaluations": 1e8,  # maximum number of function iterations
    "snesMaxIterations": 50,            # maximum number of iterations in the nonlinear solver
    "snesRelativeTolerance": 1e-10,     # tolerance of the nonlinear solver
    
    #"dumpFilename": "out/r{}/m".format(sys.argv[-1]),   # dump system matrix and right hand side after every solve
    "dumpFilename": "",         # dump disabled
    "dumpFormat": "matlab",   # default, ascii, matlab
    
    # boundary and initial conditions
    "dirichletBoundaryConditions": dirichlet_bc,
    "neumannBoundaryConditions": neumann_bc,
    "initialValuesDisplacements": [],
    #"initialValuesDisplacements": [[0.0,0.0,0.0] for i in range((2*nx+1)*(2*ny+1)*(2*nz+1-1))] + [[1.0,0.0,0.0] for i in range((2*nx+1)*(2*ny+1))],
    "initialValuesVelocities": [[0.1*z,0.0,0.0] for i in range((2*nx+1)*(2*ny+1)) for z in range((2*nz+1))],
    "constantBodyForce": [0,0,0],
    
    "OutputWriter" : [   # output files for displacements function space (quadratic elements), contains displacements, velocities and PK2 stresses
      {"format": "Paraview", "outputInterval": 1, "filename": "out/u", "binary": False, "fixedFormat": False, "onlyNodalValues":True, "combineFiles":True},
      #{"format": "PythonFile", "filename": "out/u", "outputInterval": 1, "binary":False, "onlyNodalValues":True},
    ],
    "pressure": {   # output files for pressure function space (linear elements), contains pressure values, as well as displacements and velocities
      "OutputWriter" : [
        {"format": "Paraview", "outputInterval": 1, "filename": "out/p", "binary": False, "fixedFormat": False, "onlyNodalValues":True, "combineFiles":True},
      ]
    },
    "dynamic": {    # output of the dynamic solver, has additional virtual work values 
      "OutputWriter" : [   # output files for displacements function space (quadratic elements)
        #{"format": "Paraview", "outputInterval": int(output_interval/dt), "filename": "out/dynamic", "binary": False, "fixedFormat": False, "onlyNodalValues":True, "combineFiles":True},
        #{"format": "Paraview", "outputInterval": 1, "filename": "out/dynamic", "binary": False, "fixedFormat": False, "onlyNodalValues":True, "combineFiles":True},
      ],
    }
  }
}
