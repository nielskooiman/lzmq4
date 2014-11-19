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

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "zmq.h"

#include "lzmq.h"

#include <stdlib.h>
#include <string.h>

/* TODO: include keypair function */
/*
#include <sodium.h>
uint8_t public_key [32];
uint8_t secret_key [32];

int test()
{
    int rc = crypto_box_keypair(public_key, secret_key);
    return rc;
}
*/



static const struct luaL_Reg lzmq_functions[] = {
    { "version", Lzmq_version },
    { "errno", Lzmq_errno },
    { "strerror", Lzmq_strerror },

// Context functions
    { "ctx_new", Lzmq_ctx_new },
    { "ctx_term", Lzmq_ctx_term },
    { "ctx_shutdown", Lzmq_ctx_shutdown },
    { "ctx_set", Lzmq_ctx_set },
    { "ctx_get", Lzmq_ctx_get },

// Message functions
    { "msg_init", Lzmq_msg_init },
    { "msg_init_size", Lzmq_msg_init_size },
    { "msg_init_data", Lzmq_msg_init_data },
    { "msg_send", Lzmq_msg_send },
    { "msg_recv", Lzmq_msg_recv },
    { "msg_close", Lzmq_msg_close },
    { "msg_move", Lzmq_msg_move },
    { "msg_copy", Lzmq_msg_copy },
    { "msg_data", Lzmq_msg_data },
    { "msg_size", Lzmq_msg_size },
    { "msg_more", Lzmq_msg_more },
    { "msg_get", Lzmq_msg_get },
    { "msg_set", Lzmq_msg_set },

    // CAUTION: originally unexisting function, added to receive 'event' messages
    { "msg_recv_event", Lmsg_recv_event },


// Socket functions
    { "socket", Lzmq_socket },
    { "close", Lzmq_close },
    { "setsockopt", Lzmq_setsockopt },
    { "getsockopt", Lzmq_getsockopt },
    { "bind", Lzmq_bind },
    { "connect", Lzmq_connect },
    { "unbind", Lzmq_unbind },
    { "disconnect", Lzmq_disconnect },
    { "send", Lzmq_send },
    { "send_const", Lzmq_send_const },
    { "recv", Lzmq_recv },
    { "socket_monitor", Lzmq_socket_monitor },
    { "sendmsg", Lzmq_sendmsg },
    { "recvmsg", Lzmq_recvmsg },

// Other functions
    { "poll", Lzmq_poll },
    { "proxy", Lzmq_proxy },
    { "z85_encode", Lzmq_z85_encode },
    { "z85_decode", Lzmq_z85_decode },
    { NULL, NULL}
};

typedef struct lua_IntConst {
  const char *name;
  int val;
} lua_IntConst;

static const struct lua_IntConst lzmq_constants[] = {

// Compile time const, retreivable at runtime via function
    { "VERSION_MAJOR", ZMQ_VERSION_MAJOR },
    { "VERSION_MINOR", ZMQ_VERSION_MINOR },
    { "VERSION_PATCH", ZMQ_VERSION_PATCH },

// Error codes
    { "ENOTSUP", ENOTSUP },
    { "EPROTONOSUPPORT", EPROTONOSUPPORT },
    { "ENOBUFS", ENOBUFS },
    { "ENETDOWN", ENETDOWN },
    { "EADDRINUSE", EADDRINUSE },
    { "EADDRNOTAVAIL", EADDRNOTAVAIL },
    { "ECONNREFUSED", ECONNREFUSED },
    { "EINPROGRESS", EINPROGRESS },
    { "ENOTSOCK", ENOTSOCK },
    { "EMSGSIZE", EMSGSIZE },
    { "EAFNOSUPPORT", EAFNOSUPPORT },
    { "ENETUNREACH", ENETUNREACH },
    { "ECONNABORTED", ECONNABORTED },
    { "ECONNRESET", ECONNRESET },
    { "ETIMEDOUT", ETIMEDOUT },
    { "EHOSTUNREACH", EHOSTUNREACH },
    { "ENETRESET", ENETRESET },

//  Native error codes.
    { "EFSM", EFSM },
    { "ENOCOMPATPROTO", ENOCOMPATPROTO },
    { "ETERM", ETERM },
    { "EMTHREAD", EMTHREAD },

//Context options
    { "IO_THREADS", ZMQ_IO_THREADS },
    { "MAX_SOCKETS", ZMQ_MAX_SOCKETS },

// Default for new contexts
    { "IO_THREADS_DFLT", ZMQ_IO_THREADS_DFLT },
    { "MAX_SOCKETS_DFLT", ZMQ_MAX_SOCKETS_DFLT },

// Socket types
    { "PAIR", ZMQ_PAIR },
    { "PUB", ZMQ_PUB },
    { "SUB", ZMQ_SUB },
    { "REQ", ZMQ_REQ },
    { "REP", ZMQ_REP },
    { "DEALER", ZMQ_DEALER },
    { "ROUTER", ZMQ_ROUTER },
    { "PULL", ZMQ_PULL },
    { "PUSH", ZMQ_PUSH },
    { "XPUB", ZMQ_XPUB },
    { "XSUB", ZMQ_XSUB },
    { "STREAM", ZMQ_STREAM },

// Socket options
    { "AFFINITY", ZMQ_AFFINITY },
    { "IDENTITY", ZMQ_IDENTITY },
    { "SUBSCRIBE", ZMQ_SUBSCRIBE },
    { "UNSUBSCRIBE", ZMQ_UNSUBSCRIBE },
    { "RATE", ZMQ_RATE },
    { "RECOVERY_IVL", ZMQ_RECOVERY_IVL },
    { "SNDBUF", ZMQ_SNDBUF },
    { "RCVBUF", ZMQ_RCVBUF },
    { "RCVMORE", ZMQ_RCVMORE },
    { "FD", ZMQ_FD },
    { "EVENTS", ZMQ_EVENTS },
    { "TYPE", ZMQ_TYPE },
    { "LINGER", ZMQ_LINGER },
    { "RECONNECT_IVL", ZMQ_RECONNECT_IVL },
    { "BACKLOG", ZMQ_BACKLOG },
    { "RECONNECT_IVL_MAX", ZMQ_RECONNECT_IVL_MAX },
    { "MAXMSGSIZE", ZMQ_MAXMSGSIZE },
    { "SNDHWM", ZMQ_SNDHWM },
    { "RCVHWM", ZMQ_RCVHWM },
    { "MULTICAST_HOPS", ZMQ_MULTICAST_HOPS },
    { "RCVTIMEO", ZMQ_RCVTIMEO },
    { "SNDTIMEO", ZMQ_SNDTIMEO },
    { "LAST_ENDPOINT", ZMQ_LAST_ENDPOINT },
    { "ROUTER_MANDATORY", ZMQ_ROUTER_MANDATORY },
    { "TCP_KEEPALIVE", ZMQ_TCP_KEEPALIVE },
    { "TCP_KEEPALIVE_CNT", ZMQ_TCP_KEEPALIVE_CNT },
    { "TCP_KEEPALIVE_IDLE", ZMQ_TCP_KEEPALIVE_IDLE },
    { "TCP_KEEPALIVE_INTVL", ZMQ_TCP_KEEPALIVE_INTVL },
    { "TCP_ACCEPT_FILTER", ZMQ_TCP_ACCEPT_FILTER },
    { "IMMEDIATE", ZMQ_IMMEDIATE },
    { "XPUB_VERBOSE", ZMQ_XPUB_VERBOSE },
    { "ROUTER_RAW", ZMQ_ROUTER_RAW },
    { "IPV6", ZMQ_IPV6 },
    { "MECHANISM", ZMQ_MECHANISM },
    { "PLAIN_SERVER", ZMQ_PLAIN_SERVER },
    { "PLAIN_USERNAME", ZMQ_PLAIN_USERNAME },
    { "PLAIN_PASSWORD", ZMQ_PLAIN_PASSWORD },
    { "CURVE_SERVER", ZMQ_CURVE_SERVER },
    { "CURVE_PUBLICKEY", ZMQ_CURVE_PUBLICKEY },
    { "CURVE_SECRETKEY", ZMQ_CURVE_SECRETKEY },
    { "CURVE_SERVERKEY", ZMQ_CURVE_SERVERKEY },
    { "PROBE_ROUTER", ZMQ_PROBE_ROUTER },
    { "REQ_CORRELATE", ZMQ_REQ_CORRELATE },
    { "REQ_RELAXED", ZMQ_REQ_RELAXED },
    { "CONFLATE", ZMQ_CONFLATE },
    { "ZAP_DOMAIN", ZMQ_ZAP_DOMAIN },

// Message options
    { "MORE", ZMQ_MORE },

// Send receive options
    { "DONTWAIT", ZMQ_DONTWAIT },
    { "SNDMORE", ZMQ_SNDMORE },

// Security mechanisms
    { "NULL", ZMQ_NULL },
    { "PLAIN", ZMQ_PLAIN },
    { "CURVE", ZMQ_CURVE },

// Socket transport events (tcp and ipc only)
    { "EVENT_CONNECTED", ZMQ_EVENT_CONNECTED },
    { "EVENT_CONNECT_DELAYED", ZMQ_EVENT_CONNECT_DELAYED },
    { "EVENT_CONNECT_RETRIED", ZMQ_EVENT_CONNECT_RETRIED },
    { "EVENT_LISTENING", ZMQ_EVENT_LISTENING },
    { "EVENT_BIND_FAILED", ZMQ_EVENT_BIND_FAILED },
    { "EVENT_ACCEPTED", ZMQ_EVENT_ACCEPTED },
    { "EVENT_ACCEPT_FAILED", ZMQ_EVENT_ACCEPT_FAILED },
    { "EVENT_CLOSED", ZMQ_EVENT_CLOSED },
    { "EVENT_CLOSE_FAILED", ZMQ_EVENT_CLOSE_FAILED },
    { "EVENT_DISCONNECTED", ZMQ_EVENT_DISCONNECTED },
    { "EVENT_MONITOR_STOPPED", ZMQ_EVENT_MONITOR_STOPPED },
    { "EVENT_ALL", ZMQ_EVENT_ALL },

// I/O multiplexing
    { "POLLIN", ZMQ_POLLIN },
    { "POLLOUT", ZMQ_POLLOUT },
    { "POLLERR", ZMQ_POLLERR },


// CUSTOM
    { "OPT_INT", 1 },
    { "OPT_INT64", 2 },
    { "OPT_UINT64", 3 },
    { "OPT_BINARY", 4 },

    { NULL, 0 }
};

void register_contants(lua_State *L, const char* name, const lua_IntConst constants[])
{
    lua_newtable(L);
    lua_setfield(L, -2, name);

    // retrieve the const table
    lua_pushlstring(L, name, strlen(name));
    lua_gettable(L, -2);

    // register constants
    int i = 0;
    while(1)
    {
        if (constants[i].name != NULL)
        {
            lua_pushnumber(L, constants[i].val);
            lua_setfield(L, -2, constants[i].name);
        }
        else
        {
            break;
        }
        i += 1;
    }
}

int luaopen_lzmq4(lua_State *L)
{
    // register functions
    luaL_newlib (L, lzmq_functions);

    int pos = lua_gettop(L); // remember the table position
    register_contants(L, "ZMQ", lzmq_constants);
    lua_settop(L, pos); // restore table position

    return 1;
}
