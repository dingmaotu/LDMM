import os
envDef = DefaultEnvironment(CXX="icpc", ENV=os.environ)
envDef.Append(CCFLAGS = ['-Wall', '-openmp'])
envDef.Append(CPPPATH = 'include')
envDef.Append(LINKFLAGS = ['-intel-static', '-openmp'])


if 'debug' in COMMAND_LINE_TARGETS:
    envDef.AppendUnique(CCFLAGS = ['-g'])
else:
    envDef.Prepend(CCFLAGS = ['-O2', '-DNDEBUG'])

sources = Glob('src/*.cpp')

ldmm_exe = envDef.Program("ldmm", sources)
envDef.NoClean(ldmm_exe)

Help("""
LDMM Build Tool:
Type "scons debug" to build debug version of LDMM
Type "scons release" to build release version of LDMM
Type "scons -c" to clean build (except the LDMM executables)
""")
