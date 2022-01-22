

## Dev Environment

### MacOS

```
brew install hiredis
export LDFLAGS="${LDFLAGS:+${LDFLAGS}:}$(pkg-config --libs hiredis)"
export CFLAGS="${CFLAGS:+${CFLAGS}:}$(pkg-config --cflags hiredis)"
```
