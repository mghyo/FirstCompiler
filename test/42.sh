#!/bin/bash

set -e

./phase2 test/42.ir $1

diff out.s test/42.$1.s

spim -f out.s > tmp

diff tmp test/42.out

