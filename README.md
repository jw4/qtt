

## Dev Environment

### Ubuntu

```
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:+${LD_LIBRARY_PATH}:}/usr/local/lib"
export LDFLAGS="-lhiredis"
export CFLAGS="-I/usr/local/include/hiredis"
```

### MacOS

```
brew install hiredis
export LDFLAGS="${LDFLAGS:+${LDFLAGS}:}$(pkg-config --libs hiredis)"
export CFLAGS="${CFLAGS:+${CFLAGS}:}$(pkg-config --cflags hiredis)"
```
