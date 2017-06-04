# Dataflow plugin for V-REP

### Compiling

1. Install required python packages for [v_repStubsGen](https://github.com/fferri/v_repStubsGen): see v_repStubsGen's [README](external/v_repStubsGen/README.md)
2. Download and install Qt (same version as V-REP, i.e. 5.5.0)
3. Checkout and compile
```
$ git clone --recursive https://github.com/fferri/v_repExtDataflow.git
$ /path/to/Qt5.5.0/5.5/.../bin/qmake v_repExtDataflow.pro
$ make
```
