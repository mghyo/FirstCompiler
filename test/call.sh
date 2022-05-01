#!/bin/bash

set -e

./phase2 test/call.ir $1

diff out.s test/call.$1.s

spim -f out.s > tmp

diff tmp test/call.out

