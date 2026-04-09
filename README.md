# Using General Disjunction

This repository keeps only the local instructions needed to build and use the
general disjunction setup.

For all standard SCIP documentation, installation details, and general build
instructions, see the upstream SCIP repository:

https://github.com/scipopt/scip

## Build scipmip with general disjunction support

If you want to use the general disjunction branching rule in SCIP, build
`scipmip` in this source tree.

For a CMake build:

```bash
mkdir build
cd build
cmake ..
make scipmip
```

For a Makefile build:

```bash
make scipmip
```

The resulting `scipmip` binary is created in the corresponding `bin`
directory and includes the general disjunction branching rule.

If you also want to run the shared-memory parallel version, build `fscip` as
well:

```bash
make fscip
```

## Run one instance sequentially

Use the default non-sandbox general disjunction setting file:

```bash
./build/bin/examples/scipmip \
	/path/to/instance.mps \
	./settings/scipmip_settings/allon_gendj.set
```

## Run one instance in parallel

For the shared-memory parallel run, use `fscip` together with the default
non-sandbox general disjunction parameter file and setting file:

```bash
./build/bin/fscip \
	./settings/fscip_settings/default_mps_gendj_allon.prm \
	/path/to/instance.mps \
	-s ./settings/fscip_settings/allon_gendj.set \
	-sr ./settings/fscip_settings/allon_gendj.set \
	-sth 8
```

For parameter details and all other SCIP usage documentation, refer to the
upstream SCIP repository linked above.
