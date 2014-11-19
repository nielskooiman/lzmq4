
## LZMQ4

Lua C bindings to ØMQ (ZeroMQ) 4.0

### Status

Alpha version, use at your own risk!

Missing functionality:
- Advanced messages: zmq_msg_* (except zmq_msg_recv and zmq_msg_send)
- Security: zmq_z85_*
(probably more..)


### Usage

```lua

local zmq = assert(require('lzmq4'))
local ZMQ = zmq.ZMQ -- Table containing the constants

local context = zmq.ctx_new() -- Just like the C version: zmq_ctx_new();
local router = zmq.socket(context, ZMQ.ROUTER) -- Just like the C version: zmq_socket(context, ZMQ_ROUTER);

-- zmq.bind(router, 'tcp://127.0.0.1:8080')
-- zmq.connect(router, 'tcp://127.0.0.1:8080')

zmq.close(router)
zmq.ctx_shutdown(context)
```

### Compiling

I'm using Code::Blocks for now, the generated commands are:

```bash

gcc -Wall -fPIC -O3  -c src/lzmq-context.c -o obj/Release/src/lzmq-context.o
gcc -Wall -fPIC -O3  -c src/lzmq-message.c -o obj/Release/src/lzmq-message.o
gcc -Wall -fPIC -O3  -c src/lzmq-socket.c -o obj/Release/src/lzmq-socket.o
gcc -Wall -fPIC -O3  -c src/lzmq.c -o obj/Release/src/lzmq.o
g++ -shared  obj/Release/src/lzmq-context.o obj/Release/src/lzmq-message.o obj/Release/src/lzmq-socket.o obj/Release/src/lzmq.o  -o lzmq4.so -s  -llua -lzmq
```
Output file is lzmq4.so with size 29.47 KB
Process terminated with status 0 (0 minute(s), 0 second(s))
0 error(s), 0 warning(s) (0 minute(s), 0 second(s))


### Documentation

<http://api.zeromq.org/>
