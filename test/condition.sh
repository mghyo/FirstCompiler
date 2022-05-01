#!/bin/bash

set -e

./phase2 test/condition.ir $1

diff out.s test/condition.$1.s

spim -f out.s > tmp

diff tmp test/condition.out

