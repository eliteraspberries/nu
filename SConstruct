from __future__ import print_function

import functools
import os
import re
import string
import subprocess
import sys


def filename(x):
    return [str(f.abspath) for f in x]


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

libs = ['m']

system_libs = {
    'gnu': ['rt'],
}

functions = [
    (
        ('clock_gettime', 'clock_gettime(0, NULL);', ['time.h'], []),
        ('CLOCK_GETTIME', 1),
    ),
]

default_flags = {
    'CPPDEFINES': [
        ('_POSIX_C_SOURCE', '200112L'),
        ('_XOPEN_SOURCE', '600'),
    ],
    'CFLAGS': [
        '-ffp-contract=on',
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
    ctx.env['CFLAGS'].remove(flag)
    ctx.Result(result)
    return result


def CheckFunction(ctx, function, statements, headers, libs):
    includes = '\n'.join(['#include <{}>'.format(h) for h in headers])
    main = '''
        $includes
        int main(void) {
            $statements
            return 0;
        }
    '''
    src = string.Template(main).substitute({
        'includes': includes,
        'statements': statements,
    })
    ctx.Message('Checking function ' + function + '()... ')
    ctx.env.Append(LIBS=libs)
    result = ctx.TryLink(src, '.c')
    for lib in libs:
        ctx.env['LIBS'].remove(lib)
    ctx.Result(result)
    return result


def run(target, source, env):
    cwd = os.getcwd()
    if system == 'darwin':
        penv = {'DYLD_FALLBACK_LIBRARY_PATH': cwd}
    else:
        penv = {'LD_LIBRARY_PATH': cwd}
    ok = True
    tgt = filename(target).pop()
    for src in filename(source):
        print(os.path.basename(src), end='... ')
        p = subprocess.Popen(
            src,
            env=penv,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        out, err = p.communicate()
        if p.returncode == 0:
            print('pass')
        else:
            with open(tgt, 'w+') as log:
                log.write(out)
                log.write(err)
            print('fail')
            ok = False
    if not ok:
        Exit(1)


def gettarget(env):
    opts = [
        '-print-target-triple',
        '-print-multiarch',
        '-dumpmachine',
    ]
    for opt in opts:
        try:
            output = subprocess.check_output(
                env.get('CC', 'cc').split() +
                env.get('CFLAGS', '').split() +
                [opt]
            )
            target = output.rstrip('\n').split('-')
            cpu = target[0]
            vendor = target[1]
            system = re.match(r'([a-z]*)(.*)', target[-1]).group(1)
            return (cpu, vendor, system)
        except:
            pass
    return (None, None, None)


def generate(target, source, env):
    tgt = filename(target).pop()
    src = filename(source).pop()
    if os.path.exists(tgt) and os.path.getmtime(tgt) >= os.path.getmtime(src):
        return None
    name, ext = os.path.splitext(os.path.basename(tgt))
    with open(tgt, 'w') as t:
        with open(src, 'r') as s:
            p = subprocess.Popen(
                [sys.executable, name + '.py'],
                stderr=sys.stderr,
                stdin=s,
                stdout=t,
            )
            p.wait()
    return None


generator = Builder(action=generate)

_, _, system = gettarget(os.environ)
try:
    platform = Platform(system)
except:
    platform = Platform('posix')
environment = {
    'ENV': os.environ,
    'platform': platform,
}
env = Environment(**environment)
env.Append(BUILDERS={'Generate': generator})
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
conf = Configure(
    env,
    custom_tests={
        'CheckCCFlag': CheckCCFlag,
        'CheckFunction': CheckFunction,
    }
)
conf.env.MergeFlags(system_flags.get(system, {}))
for cflag in default_flags['CFLAGS']:
    if conf.CheckCCFlag(cflag):
        conf.env.Append(CFLAGS=[cflag])
if not all(map(conf.CheckCHeader, headers)):
    Exit(1)
if not all(map(conf.CheckCHeader, system_headers.get(system, []))):
    Exit(1)
if system == 'android':
    libs = [x for x in libs if not x in ['pthread', 'rt']]
for i, lib in enumerate(libs):
    if not conf.CheckLib(lib):
        del libs[i]
for lib in system_libs.get(system, []):
    if conf.CheckLib(lib):
        libs.append(lib)
conf.env.Append(LIBS=libs)
for check, define in functions:
    if conf.CheckFunction(*check):
        conf.env.MergeFlags({'CPPDEFINES': define})
env = conf.Finish()
debug_env = env.Clone()
debug_env.MergeFlags(debug_flags)

env.Depends('diff.c', ['diff.py', 'diff.c.in'])

src = [
    'amath.c',
    'array.c',
    'clock.c',
    env.Generate('diff.c', 'diff.c.in'),
    'random.c',
    'sum.c',
]

libnu = env.SharedLibrary('nu', src)

tests = [
    'test-amath',
    'test-array',
    'test-array-complex',
    'test-clock',
    'test-diff',
    'test-random',
    'test-sum',
]

for i, test in enumerate(tests):
    tests[i] = debug_env.Program(
        test,
        [test + '.c'],
        LIBPATH='.',
        LIBS=libs + ['nu'],
    )

Default(libnu)

all = [libnu] + tests
env.Alias('all', all)

check = Command('check.log', tests, run)
AlwaysBuild(check)
env.Alias('check', check)
