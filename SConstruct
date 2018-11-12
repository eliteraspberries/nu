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

default_flags = {
    'CPPFLAGS': [
        '-D_XOPEN_SOURCE=500',
    ],
    'CFLAGS': [
        '-std=c99',
        '-fno-fast-math',
        '-fpic',
    ],
}


def CheckCCFlag(ctx, flag):
    src = '''int main(void) {return 0;}'''
    ctx.Message('Checking if compiler supports ' + flag + '... ')
    cflags = ctx.env['CFLAGS']
    ctx.env.Append(CFLAGS=[flag])
    result = ctx.TryCompile(src, '.c')
    ctx.env.Replace(CFLAGS=cflags)
    ctx.Result(result)
    return result


env = Environment(ENV=os.environ)
conf = Configure(env, custom_tests={'CheckCCFlag': CheckCCFlag})
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
conf.env.MergeFlags({'CPPFLAGS': default_flags['CPPFLAGS']})
for cflag in default_flags['CFLAGS']:
    if conf.CheckCCFlag(cflag):
        conf.env.Append(CFLAGS=[cflag])
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
