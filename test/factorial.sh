#!/bin/bash

set -e

./phase2 test/factorial.ir $1

diff out.s test/factorial.$1.s

spim -f out.s > tmp

diff tmp test/factorial.out

