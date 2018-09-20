# Diffusion solver

This program solves the Poisson equation with different boundary conditions.

It takes a problem input file as input and produces an output binary file containing the final converged solution field (hopefully).

## Usage

```
./diffusion <input-file> <output-file>
```

## Input file format

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
