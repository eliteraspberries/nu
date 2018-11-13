from __future__ import print_function

import os
import subprocess


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
    'CPPDEFINES': [
        ('_XOPEN_SOURCE', '500'),
    ],
    'CFLAGS': [
        '-fno-fast-math',
        '-fpic',
        '-std=c99',
        '-Wall',
        '-Wno-float-equal',
    ],
}

debug_flags = {
    'CFLAGS': [
        '-g',
        '-O0',
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


def run(target, source, env):
    tgt = str(target.pop().abspath)
    for src in source:
        src = str(src.abspath)
        print(os.path.basename(src), end='... ')
        with open(tgt, 'w+') as log:
            p = subprocess.Popen(src, stdout=log, stderr=log, shell=True)
            p.wait()
        if p.returncode == 0:
            print('pass')
        else:
            print('fail')
            Exit(1)


env = Environment(ENV=os.environ)
conf = Configure(env, custom_tests={'CheckCCFlag': CheckCCFlag})
conf.env.Append(ENV={'PATH': os.environ['PATH']})
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
if not conf.CheckLib('m'):
    Exit(1)
conf.env.MergeFlags({'CPPDEFINES': default_flags['CPPDEFINES']})
for cflag in default_flags['CFLAGS']:
    if conf.CheckCCFlag(cflag):
        conf.env.Append(CFLAGS=[cflag])
env = conf.Finish()
debug_env = env.Clone()
debug_env.MergeFlags(debug_flags)

src = [
    'amath.c',
    'random.c',
    'sum.c',
]

libnu = env.SharedLibrary('nu', src, LIBS=[])

tests = [
    'test-random',
    'test-sum',
]

for i, test in enumerate(tests):
    tests[i] = debug_env.Program(test, [test + '.c'], LIBS=[libnu, 'm'])

all = [libnu] + tests

Default(all)
env.Alias('all', all)

check = Command('check.log', tests, run)
AlwaysBuild(check)
env.Alias('check', check)
