# ALGOL-eqsue Compiler from School
This was the first compiler I wrote in school.

## Building

```
$ cmake . && make
```

You may then run the tests with `$ make test`.

## Usage

```
Usage   : ./phase2 <filename> <reg_alloc_scheme>
Example : ./phase2 test/42.ir naive
```

If successful, the compiler writes the generated assembly to the current working
directory in `out.s`. Note that `make test` will clobber `out.s`.

## Structure

The compiler has these major files:

* CFG.cpp - Builds the control flow graph from the parsed IR.
* CodeGen.cpp - Generates most of the asm from IR (instruction selection) -
  except for the parts delegated out to the various strategies.
* Global.cpp - Whole function register allocation strategy. Performs the
  liveness analysis, web building, inference graph building, coloring, any
  necessary spilling, etc.
* IntraBlock.cpp - Intra-block allocation strategy. Performs the block liveness
  analysis and adds load/store instructions before/after each block.
* IR.cpp - Code used to parse IR
* Naive.cpp - Naive strategy. Fairly simple and just loads and stores directly
  from/to stack.
* phase2.cpp - Entrypoint. A lot of the IR parsing code is also here.

## Design internals

Each strategy is implemented completely in its own source file. The strategy
overrides the methods declared in Strategy.h - for loading/storing, enter/exit
blocks, as required. This way most of the code generation logic can stay
centralized and only the strategy specific code generation parts must be specialized.
