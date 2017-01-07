
debug = ARGUMENTS.get('debug')

env = Environment(DEBUG = debug)

env.AppendUnique(LIBS = ['pthread', 'boost_system', 'boost_thread', 'boost_regex'])
env.AppendUnique(LDFLAGS = ['-pthread'])
env.AppendUnique(CCFLAGS = ['-std=c++11', '-DBOOST_SYSTEM_NO_DEPRECATED'])
env.AppendUnique(LIBPATH = ['/usr/lib/x86_64-linux-gnu'])

if debug:
    env.AppendUnique(CCFLAGS = '-g')
    env.AppendUnique(CCFLAGS = '-O0')

VariantDir('build', 'src', False)

SConscript('build/SConscript', exports=['env', 'debug'])
