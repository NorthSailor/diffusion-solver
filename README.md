# Diffusion solver

This program solves the Poisson equation with different boundary conditions on a rectangular structured grid.

It takes a problem input file as input and produces an output binary file containing the final converged solution field (hopefully).

## Usage

```
./diffusion <input-file> <output-file>
```

As of now, the solver has a hardcoded condition to terminate after 5 million iterations if the convergence criterion is not met. This is a temporary solution, functionality to allow the user to configure both the convergence criterion and the maximum iteration count needs to be implemented.

### Input file format

The first line of the file defines the grid and has the following format:

```
WIDTH HEIGHT PHYSICAL_WIDTH PHYSICAL_HEIGHT FIELD_NAME
```

Where:
* `WIDTH`: Integer grid width.
* `HEIGHT`: Integer grid height.
* `PHYSICAL_WIDTH`: Physical width of the domain.
* `PHYSICAL_HEIGHT`: Physical height of the domain.
* `FIELD_NAME`: The name of the field (e.g. Temperature).

After these options have been given, the boundary conditions are defined in separate lines with the following format:

```
BOUNDARY TYPE START END VALUE
```

All grid indices are zero-indexed and the end values are not inclusive.

`BOUNDARY` can be one of: `X-`, `X+`, `Y-`, `Y+`.

`TYPE` can be either `DIRICHLET` or `NEUMANN`. For Neumann boundary conditions the value represents the outflow thus an inlet's flowrate would be negative.

The parts of the grid's boundary that are not explicitly set get an implicit no-flow condition.

### Output file format
The output file is a binary file containing the values of the potential throughout the grid. A velocity field with the velocity components interlaced will be written in `velocity.bin` in the working directory.

## Building

The solver can be built like any CMake C project. Simply create a build directory, switch to it and run cmake with the options you want.

A simple release build sequence would look like:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

To build the program with double precision floating point numbers, set `USE_DOUBLES=1`.