This tool is a simple wrapper around a Redis "queue".

Invoking the binary with values will push items onto the list, and invoking it without any args will pop items off the queue.

Examples of how to use this tool in bash scripts to distribute work items are in the examples directory.

# Usage

```sh
$ ./qtt --help
Usage: ./qtt [flags] [values]

 Flags:
  --help      
  --verbose     
  --port  | -p <arg>
  --host  | -h <arg>
  --queue | -q <arg>
  --wait  | -w <arg>


Environment variables can be used for default values:
  REDIS_HOST
  REDIS_PORT
  QTT_QUEUE       (redis key name of the queue)
  QTT_BLOCK_WAIT  (seconds to block while waiting for item in queue)


When no values are passed in ./qtt will block for the configured time, waiting for items to appear on the queue.
When values are passed, they are added to the queue.
```

## Dev Environment

### Ubuntu

```sh
#(cd <src path>/hiredis; make install)

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:+${LD_LIBRARY_PATH}:}/usr/local/lib"
export LDFLAGS="-lhiredis"
export CFLAGS="-I/usr/local/include/hiredis"
```

### MacOS

```sh
#brew install hiredis

export LDFLAGS="$(pkg-config --libs ${PACKAGES})"
export CFLAGS="$(pkg-config --cflags ${PACKAGES})"
```
