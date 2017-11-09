# SConstruct file for testing
#
# Usage: `scons BUILD_TYPE=debug` will build debug version, `scons` will build release version.
import os
import sys

sys.path.append("../dependencies/scons-config/dist/scons_config-0.1-py2.7.egg")     # load scons_config

import sconsconfig
from sconsconfig import packages, tools

#
# Select the packages we want to use in the configuration.
#
sconsconfig.select(
    packages.MPI(required=True),
    packages.LAPACK(required=True),
    packages.PETSc(required=True),
    packages.Python(required=True),
    packages.Base64(required=True),
    packages.Cython(required=True),
    packages.NumpyC(required=True),
    packages.googletest(required=True),
    packages.EasyLoggingPP(required=True),      # this has to be the last package to be included
)

# By default set the packages' location to the dependency directory. This can be overridden in 'user-variables.scons.py'.
# Note that an absolute path has to be given, otherwise there are problems finding the libraries
preset_variables = {'LAPACK_DIR': os.path.join(Dir('.').srcnode().abspath, '../dependencies/lapack/install'),
                    'PETSC_DIR':  os.path.join(Dir('.').srcnode().abspath, '../dependencies/petsc/install')}

# read variables from user file 'user-variables.scons.py' and from command line arguments, e.g. `scons BUILD_TYPE=release`
variables = ARGUMENTS.copy()        # command line arguments are in the global ARGUMENTS dictionary
variables.update(preset_variables)  # merge in preset_variables dictionary
vars = Variables('user-variables.scons.py', variables)

# specify type and default value for recognized variables
#vars.Add(BoolVariable('DUMMY', 'This is left in the code to see how bools work', 0))
vars.Add(EnumVariable('BUILD_TYPE', 'The build type, according to that different compiler options will be set. '\
                      '(Shortcuts: ''d'',''r'')', 'release', allowed_values=('debug', 'release'), ignorecase = 2, 
                      map={'d':'debug', 'r':'release'}))

    
# Add options from any packages we want to use.
sconsconfig.add_options(vars)

# initialize environment object containing build system settings such as compilers and flags
# command line options are considered
env = Environment(tools = ['default'], toolpath = ['config/tools'], variables = vars, ENV = os.environ)

# check for unknown variables and output warning
unknown = vars.UnknownVariables()
if unknown:
  print "WARNING: Unknown variables from command line arguments or config file 'user-variables.scons.py':", unknown.keys()
    
# generate help text for the page `scons -h`
Help(vars.GenerateHelpText(env))

# Create our configuration environment, passing the set of custom tests.
sconf = env.Configure(custom_tests=sconsconfig.custom_tests)

# Run our custom tests with any options needed.
sconsconfig.check(sconf)

# Finish the configuration and save it to file.
sconf.Finish()

# -----------------------------------------------
# depending on the variable BUILD_TYPE either build with debug or release settings and link corresponding version of core
if env["BUILD_TYPE"] == "debug":
  
  # debug build
  variant_dir = "build_debug"           # folder of build
  env.MergeFlags('-DDEBUG -ggdb -O0 -std=c++1z -Wunused-variable')   # gcc flags, will be sorted automatically into linker and compiler flags
  env.Append(LIBPATH = [os.path.join(Dir('.').srcnode().abspath, '../core/build/debug')])   # add debug version of opendihu library
  env.Prepend(LIBS = ['opendihud'])
  
else:
  # release build
  variant_dir = 'build_release'         # folder of build
  env.MergeFlags('-O3 -std=c++1z -Wunused-variable')   # gcc flags, will be sorted automatically into linker and compiler flags
  env.Append(LIBPATH = [os.path.join(Dir('.').srcnode().abspath, '../core/build/release')])   # add release version of opendihu library
  env.Prepend(LIBS = ['opendihu'])

env.MergeFlags('-Wunused-variable')

# Set the include path for header files
env.Append(CPPPATH = [os.path.join(Dir('.').srcnode().abspath, '../core/src')])
  
# define the source files
src_files = Glob('src/*.cpp')

program = env.Program('run_tests', source=src_files)
test = env.Command(target = 'test', source = './run_tests', action = './run_tests')
#Depends(test, program)
AlwaysBuild(test)

