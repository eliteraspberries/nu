from __future__ import print_function

import functools
import os
import platform
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

system_headers = {
}

libs = []

system_libs = {
}

default_flags = {
    'CPPDEFINES': [
        ('_XOPEN_SOURCE', '500'),
    ],
    'CFLAGS': [
        '-fno-fast-math',
        '-fpic',
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Wno-float-equal',
    ],
}

system_flags = {
}

debug_flags = {
    'CFLAGS': [
        '-g',
        '-O0',
    ],
}

sys = platform.system()


def CheckCCFlag(ctx, flag):
    src = '''int main(void) {return 0;}'''
    ctx.Message('Checking if compiler supports ' + flag + '... ')
    cflags = ctx.env['CFLAGS']
    ctx.env.Append(CFLAGS=[flag])
    result = ctx.TryCompile(src, '.c')
    ctx.env.Replace(CFLAGS=cflags)
    ctx.Result(result)
    return result


def run(penv, target, source, env):
    tgt = str(target.pop().abspath)
    for src in source:
        src = str(src.abspath)
        print(os.path.basename(src), end='... ')
        with open(tgt, 'w+') as log:
            p = subprocess.Popen(
                src,
                env=penv,
                stdout=log,
                stderr=log,
                shell=True,
            )
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
if not all(map(conf.CheckCHeader, system_headers.get(sys, []))):
    Exit(1)
if not all(map(conf.CheckLib, libs)):
    Exit(1)
if not all(map(conf.CheckLib, system_libs.get(sys, []))):
    Exit(1)
conf.env.MergeFlags({'CPPDEFINES': default_flags['CPPDEFINES']})
for cflag in default_flags['CFLAGS']:
    if conf.CheckCCFlag(cflag):
        conf.env.Append(CFLAGS=[cflag])
conf.env.MergeFlags(system_flags.get(sys, {}))
env = conf.Finish()
debug_env = env.Clone()
debug_env.MergeFlags(debug_flags)

src = [
    'amath.c',
    'array.c',
    'random.c',
    'sum.c',
]

libs = [] + system_libs.get(sys, [])
libnu = env.SharedLibrary('nu', src, LIBS=libs)

tests = [
    'test-amath',
    'test-array',
    'test-array-complex',
    'test-random',
    'test-sum',
]

for i, test in enumerate(tests):
    tests[i] = debug_env.Program(test, [test + '.c'], LIBS=[libnu, 'm'])

all = [libnu] + tests

Default(all)
env.Alias('all', all)

dir = os.path.abspath('.')
if sys == 'Darwin':
    penv = {'DYLD_FALLBACK_LIBRARY_PATH': dir}
else:
    penv = {'LD_LIBRARY_PATH': dir}
run = functools.partial(run, penv)
check = Command('check.log', tests, run)
AlwaysBuild(check)
env.Alias('check', check)
