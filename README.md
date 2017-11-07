# Dataflow plugin for V-REP

### Compiling

1. Install required packages for [v_repStubsGen](https://github.com/CoppeliaRobotics/v_repStubsGen): see v_repStubsGen's [README](external/v_repStubsGen/README.md)
2. Download and install Qt (same version as V-REP, i.e. 5.5.0)
3. Checkout and compile
```
$ git clone --recursive https://github.com/CoppeliaRobotics/v_repExtDataflow.git
$ cmake .
$ cmake --build .
```
