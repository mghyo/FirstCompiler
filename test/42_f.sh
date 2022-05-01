#!/bin/bash

set -e

./phase2 test/42_f.ir $1

diff out.s test/42_f.$1.s

spim -f out.s > tmp

diff tmp test/42_f.out

