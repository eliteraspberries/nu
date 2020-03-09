# Copyright 2019, 2020, Mansour Moufid <mansourmoufid@gmail.com>

from __future__ import print_function

try:
    from StringIO import StringIO
except:
    from io import StringIO
import string
import sys


def ks(n):
    return map(float, range(-((n + 1) // 2) + 1, n // 2 + 1))


cos = {
    3: [[x - 0.5, -2 * x, x + 0.5] for x in ks(3)],
    4: [[-(3 * x**2 - 6 * x + 2) / 6,
          (3 * x**2 - 4 * x - 1) / 2,
         -(3 * x**2 - 2 * x - 2) / 2,
          (3 * x**2 - 1) / 6] for x in ks(4)],
    5: [[ (2 * x**3 - 3 * x**2 - x + 1) / 12,
         -(4 * x**3 - 3 * x**2 - 8 * x + 4) / 6,
          (2 * x**3 - 5 * x) / 2,
         -(4 * x**3 + 3 * x**2 - 8 * x - 4) / 6,
          (2 * x**3 + 3 * x**2 - x - 1) / 12] for x in ks(5)],
}

indent = ' ' * 4

co = StringIO()
nco = max(cos.keys())

print('{', file=co)
for k in cos:
    print('{}[{}] = {{'.format(indent, k), file=co)
    for x in cos[k]:
        co.write(2 * indent + '{')
        for f in x:
            co.write('{:10.6f}f, '.format(f))
        print('},', file=co)
    print(indent + '},', file=co)
co.write('}')

for line in sys.stdin:
    template = string.Template(line)
    out = template.substitute({
        'co': co.getvalue(),
        'nco': nco,
    })
    sys.stdout.write(out)
