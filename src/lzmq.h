///
/// Copyright (c) 2014 Niels Kooiman <niels.kooiman@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.

#ifndef __LZMQ_H_INCLUDED__
#define __LZMQ_H_INCLUDED__


// Predefenition to prevent include of lua header file here
typedef struct lua_State lua_State;


// Helper function for returning Lua errors
int return_error(lua_State* L,  char* str);
int return_zmq_error(lua_State* L, int errno);
int return_zmq_receive_error(lua_State* L);


// ZMQ functions

int Lzmq_version(lua_State* L);
int Lzmq_errno(lua_State* L);
int Lzmq_strerror(lua_State* L);

// Context functions
int Lzmq_ctx_new(lua_State* L);
int Lzmq_ctx_term(lua_State* L);
int Lzmq_ctx_shutdown(lua_State* L);
int Lzmq_ctx_set(lua_State* L);
int Lzmq_ctx_get(lua_State* L);

// Message functions
int Lzmq_msg_init(lua_State* L);
int Lzmq_msg_init_size(lua_State* L);
int Lzmq_msg_init_data(lua_State* L);
int Lzmq_msg_send(lua_State* L);
int Lzmq_msg_recv(lua_State* L);
int Lzmq_msg_close(lua_State* L);
int Lzmq_msg_move(lua_State* L);
int Lzmq_msg_copy(lua_State* L);
int Lzmq_msg_data(lua_State* L);
int Lzmq_msg_size(lua_State* L);
int Lzmq_msg_more(lua_State* L);
int Lzmq_msg_get(lua_State* L);
int Lzmq_msg_set(lua_State* L);

// NOT A ZMQ function, but a helper to receive events
int Lmsg_recv_event(lua_State* L);

// Socket functions
int Lzmq_socket(lua_State* L);
int Lzmq_close(lua_State* L);
int Lzmq_setsockopt(lua_State* L); // WANTED PRIORITY 1
int Lzmq_getsockopt(lua_State* L); // WANTED PRIORITY 1
int Lzmq_bind(lua_State* L);
int Lzmq_connect(lua_State* L);
int Lzmq_unbind(lua_State* L);
int Lzmq_disconnect(lua_State* L);
int Lzmq_send(lua_State* L);
int Lzmq_send_const(lua_State* L);
int Lzmq_recv(lua_State* L);
int Lzmq_socket_monitor(lua_State* L);
int Lzmq_sendmsg(lua_State* L);
int Lzmq_recvmsg(lua_State* L);

// Add expirimental functions?


// Other
int Lzmq_poll(lua_State* L);
int Lzmq_proxy(lua_State* L);
int Lzmq_z85_encode(lua_State* L); // WANTED PRIORITY 1
int Lzmq_z85_decode(lua_State* L); // WANTED PRIORITY 1


#endif
