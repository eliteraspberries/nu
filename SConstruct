import os


headers = [
    'assert.h',
    'float.h',
    'inttypes.h',
    'math.h',
    'stddef.h',
    'stdint.h',
    'stdio.h',
    'stdlib.h',
]


env = Environment(ENV=os.environ)
conf = Configure(env)
conf.env.Append(ENV={'PATH' : os.environ['PATH']})
for var in ['AR', 'AS', 'CC', 'CPP', 'CXX', 'LD', 'RANLIB']:
    if var in os.environ:
        conf.env.Replace(**{var: os.environ[var]})
for flags in ['CPPFLAGS', 'CFLAGS']:
    conf.env.MergeFlags({flags: os.environ.get(flags, '').split()})
for flags in ['CFLAGS', 'LINKFLAGS']:
    conf.env.MergeFlags({flags: os.environ.get('ARCHFLAGS', '').split()})
conf.env.MergeFlags({'LINKFLAGS': os.environ.get('LDFLAGS', '').split()})
if not all(map(conf.CheckCHeader, headers)):
    Exit(1)
env = conf.Finish()

src = [
    'amath.c',
    'random.c',
    'sum.c',
]

libnu = env.SharedLibrary('nu', src)

all = [libnu]

Default(all)
env.Alias('all', all)
