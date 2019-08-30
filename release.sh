#!/bin/sh
set -x
v=$1
rm -rf nu-$v
rm -f nu-$v.tar nu-$v.tar.gz
mkdir nu-$v
cp *.[ch] *.in *.py SConstruct README.md LICENSE.txt nu-$v/
tar -c -f nu-$v.tar --format ustar nu-$v
gzip nu-$v.tar
rm -rf nu-$v
