import subprocess
from Package import Package

##
##  Handles Python interpreter C library
##
class Python(Package):

    def __init__(self, **kwargs):
        super(Python, self).__init__(**kwargs)

    def check(self, ctx):
        env = ctx.env
        ctx.Message('Checking for Python ... ')
        
        # remove "-Wstrict-prototypes" because it is only valid for c and not c++
        flags_to_remove = ["-Wstrict-prototypes ", '-DNDEBUG ', '-g ', '-O2 ', '-fno-strict-aliasing ', '-Wp,-D_FORTIFY_SOURCE=2 ']
        
        pythonVersion = "2.7"   # set to 2.7 or 3
        
        # remove specified flags 
        #cflags = subprocess.check_output("python-config --cflags", shell=True)+' '
        if pythonVersion == "3":
          try:
            cflags = subprocess.check_output("python3-config --includes", shell=True)
            ldflags = subprocess.check_output("python3-config --ldflags", shell=True)
          except:
            ctx.Result(False)
            return False
        else:
          try:
            cflags = subprocess.check_output("python2.7-config --includes", shell=True)
            ldflags = subprocess.check_output("python2.7-config --ldflags", shell=True)
          except:
            print "command python2.7-config failed, try python-config instead"
            try:
              cflags = subprocess.check_output("python-config --includes", shell=True)
              ldflags = subprocess.check_output("python-config --ldflags", shell=True)
            except:
              ctx.Result(False)
              return False
        
        for flag_to_remove in flags_to_remove:            
          while flag_to_remove in cflags:
            startpos = cflags.index(flag_to_remove)
            length = len(flag_to_remove)
            while cflags[startpos+length] == ' ':
              length += 1
            cflags = cflags[0:startpos] + cflags[startpos+length:]
              
            
        
        # remove trailing newline
        if cflags[-1] == '\n':
          cflags = cflags[:-1]
        if ldflags[-1] == '\n':
          ldflags = ldflags[:-1]
        
        #print "cflags: [{}]".format(cflags)
        #print "ldflags: [{}]".format(ldflags)
        
        env.MergeFlags(cflags)
        env.MergeFlags(ldflags)
          
        ctx.Result(True)
        return True
