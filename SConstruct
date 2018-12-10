from __future__ import print_function

import functools
import os
import re
import subprocess


headers = [
    'assert.h',
    'errno.h',
    'float.h',
    'inttypes.h',
    'math.h',
    'stddef.h',
    'stdint.h',
    'stdio.h',
    'stdlib.h',
    'sys/types.h',
    'time.h',
    'unistd.h',
]

system_headers = {
    'darwin': ['mach/mach_time.h', 'sys/sysctl.h'],
}

libs = []

system_libs = {
}

default_flags = {
    'CPPDEFINES': [
        ('_POSIX_C_SOURCE', '200112L'),
        ('_XOPEN_SOURCE', '600'),
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
    'darwin': {
        'CPPDEFINES': [
            ('_DARWIN_C_SOURCE', '1'),
        ],
    },
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


def dumpmachine(env):
    output = subprocess.check_output([env['CC'], '-dumpmachine'])
    target = output.rstrip('\n')
    cpu, vendor, os = target.split('-', 2)
    system = re.compile(r'([a-z]*)[-]?(.*)').match(os).group(1)
    return (cpu, vendor, system)


_, _, sys = dumpmachine(os.environ)
try:
    platform = Platform(sys)
except:
    platform = Platform('posix')
env = Environment(ENV=os.environ, platform=platform)
env.Append(ENV={'PATH': os.environ['PATH']})
for var in ['AR', 'AS', 'CC', 'CPP', 'CXX', 'LD', 'RANLIB']:
    if var in os.environ:
        env.Replace(**{var: os.environ[var]})
for flags in ['CPPFLAGS', 'CFLAGS']:
    env.MergeFlags({flags: os.environ.get(flags, '').split()})
for flags in ['CFLAGS', 'LINKFLAGS']:
    env.MergeFlags({flags: os.environ.get('ARCHFLAGS', '').split()})
env.MergeFlags({'LINKFLAGS': os.environ.get('LDFLAGS', '').split()})
env.MergeFlags({'CPPDEFINES': default_flags['CPPDEFINES']})
conf = Configure(env, custom_tests={'CheckCCFlag': CheckCCFlag})
conf.env.MergeFlags(system_flags.get(sys, {}))
for cflag in default_flags['CFLAGS']:
    if conf.CheckCCFlag(cflag):
        conf.env.Append(CFLAGS=[cflag])
if not all(map(conf.CheckCHeader, headers)):
    Exit(1)
if not all(map(conf.CheckCHeader, system_headers.get(sys, []))):
    Exit(1)
if not all(map(conf.CheckLib, libs)):
    Exit(1)
if not all(map(conf.CheckLib, system_libs.get(sys, []))):
    Exit(1)
env = conf.Finish()
debug_env = env.Clone()
debug_env.MergeFlags(debug_flags)

src = [
    'amath.c',
    'array.c',
    'clock.c',
    'random.c',
    'sum.c',
]

libs = [] + system_libs.get(sys, [])
libnu = env.SharedLibrary('nu', src, LIBS=libs)

tests = [
    'test-amath',
    'test-array',
    'test-array-complex',
    'test-clock',
    'test-random',
    'test-sum',
]

for i, test in enumerate(tests):
    tests[i] = debug_env.Program(test, [test + '.c'], LIBS=[libnu, 'm'])

Default(libnu)

all = [libnu] + tests
env.Alias('all', all)

dir = os.path.abspath('.')
if sys == 'darwin':
    penv = {'DYLD_FALLBACK_LIBRARY_PATH': dir}
else:
    penv = {'LD_LIBRARY_PATH': dir}
run = functools.partial(run, penv)
check = Command('check.log', tests, run)
AlwaysBuild(check)
env.Alias('check', check)
