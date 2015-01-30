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

#include "lzmq.h"


#include "zmq.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <stdlib.h>
#include <string.h>


int Lzmq_socket(lua_State* L)
{
    // void *zmq_socket (void *, int type);

    int argc = lua_gettop(L);
    if (argc > 1)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isnumber(L, 2))
        {
            return return_error(L, "arg[2]: number expected");
        }

        void *ctx = lua_touserdata(L, 1);
        int socket_type = lua_tointeger(L, 2);
        //printf("%p\n", ctx);
        void *skt = zmq_socket(ctx, socket_type);
        lua_pushlightuserdata(L, skt);
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }

    return 0;
}

int Lzmq_close(lua_State* L)
{
    // int zmq_close (void *s);

    int argc = lua_gettop(L);
    if (argc > 0 && lua_islightuserdata(L, 1))
    {
        void *skt = lua_touserdata(L, 1);

        int res = zmq_close(skt);
        if (res == -1)
        {
            return return_zmq_receive_error(L);
        }
        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L, "arg[1]: userdata expected");
    }

    return 0;
}

const int TYPE_INT = 1;
const int TYPE_INT64 = 2;
const int TYPE_UINT64 = 3;
const int TYPE_BINARY = 4;

int Lzmq_setsockopt(lua_State* L)
{
    //int zmq_setsockopt (void *s, int option, const void *optval, size_t optvallen);

    // setsockopt(SOCKET, OPTION, OPTIONVAL, OPTIONTYPE)

    int argc = lua_gettop(L);
    if (argc == 4)
    {
        if (!lua_islightuserdata(L, 1)) //socket
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isnumber(L, 2)) // option
        {
            return return_error(L, "arg[2]: number expected");
        }

        if (!lua_isnumber(L, 4)) //
        {
            return return_error(L, "arg[4]: number expected");
        }

        void *skt = lua_touserdata(L, 1);
        int option = lua_tointeger(L, 2);
        int type = lua_tointeger(L, 4);
        const void *optval = NULL;
        size_t optvallen = 0;

        if (type == TYPE_INT)
        {
            if (lua_isnumber(L, 3))
            {
                int v = lua_tointeger(L, 3);
                optval = &v;
                optvallen = sizeof(int);
            }
            else if (lua_isboolean(L, 3))
            {
                int v = lua_toboolean(L, 3);
                optval = &v;
                optvallen = sizeof(int);
            }
        }
        else if (type == TYPE_INT64)
        {
            if (lua_isnumber(L, 3))
            {
                int64_t v = (int64_t)lua_tonumber(L, 3);
                optval = &v;
                optvallen = sizeof(int64_t);
            }
            else if (lua_isboolean(L, 3))
            {
                int64_t v = (int64_t)lua_toboolean(L, 3);
                optval = &v;
                optvallen = sizeof(int64_t);
            }
        }
        else if (type == TYPE_UINT64)
        {
            if (lua_isnumber(L, 3))
            {
                uint64_t v = (uint64_t)lua_tonumber(L, 3);
                optval = &v;
                optvallen = sizeof(uint64_t);
            }
            else if (lua_isboolean(L, 3))
            {
                uint64_t v = (uint64_t)lua_toboolean(L, 3);
                optval = &v;
                optvallen = sizeof(uint64_t);
            }
        }
        else if (type == TYPE_BINARY)
        {
            if (!lua_isstring(L, 3)) return_error(L, "arg[3]: string expected");

            const char *str = lua_tostring(L, 3);
            optval = str;
            optvallen = strlen(str);
        }
        else
        {
            return return_error(L, "arg[2]: option type number expected");
        }

        int res = zmq_setsockopt(skt, option, optval, optvallen);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }
        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "4 arguments expected");
    }

    return 0;
}

int Lzmq_getsockopt(lua_State* L)
{
    //int zmq_getsockopt (void *s, int option, void *optval, size_t *optvallen);
    // getsockopt(SOCKET, OPTION, OPTION_TYPE, [BUFFER_SIZE])

    int argc = lua_gettop(L);
    if (argc >= 3)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isnumber(L, 2))
        {
            return return_error(L, "arg[2]: number expected");
        }

        if (!lua_isnumber(L, 3))
        {
            return return_error(L, "arg[3]: number expected");
        }

        int buffer_size = 50;// luaL_optint(L, 4, 50);

        void *skt = lua_touserdata(L, 1);
        int option = lua_tointeger(L, 2);
        int type = lua_tointeger(L, 3);

        size_t optvallen = buffer_size;
        if (type == TYPE_INT) optvallen = sizeof(int);
        if (type == TYPE_INT64) optvallen = sizeof(int64_t);
        if (type == TYPE_UINT64) optvallen = sizeof(uint64_t);

        char buffer[optvallen];
        void *optval = buffer;

        //printf("%p; %d; (val); %ld\n", skt, option, optvallen);

        int res = zmq_getsockopt(skt, option, optval, &optvallen);
        if (res == -1)
        {
            return return_zmq_receive_error(L);
        }

        if (optvallen > 0)
        {
            if (type == TYPE_INT && optvallen == sizeof(int))
            {
                int v = *((int*)optval);
                lua_pushinteger(L, v);
                return 1;
            }
            else if (type == TYPE_INT64 && optvallen == sizeof(int64_t))
            {
                int64_t v = *((int64_t*)optval);
                lua_pushinteger(L, v);
                return 1;
            }
            else if (type == TYPE_UINT64 && optvallen == sizeof(uint64_t))
            {
                uint64_t v = *((uint64_t*)optval);
                lua_pushinteger(L, v);
                return 1;
            }
            else if (type == TYPE_BINARY)
            {
                lua_pushlstring(L, optval, optvallen);
                return 1;
            }
            else
            {
                return return_error(L, "arg[2]: unknown data type");
            }
        }

        return return_error(L, "option returned no value");
    }
    else
    {
        return return_error(L,  "3 arguments expected");
    }

    return 0;
}

int Lzmq_bind(lua_State* L)
{
    // int zmq_bind (void *s, const char *addr);
    int argc = lua_gettop(L);
    if (argc > 1)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isstring(L, 2))
        {
            return return_error(L, "arg[2]: string expected");
        }

        void *skt = lua_touserdata(L, 1);
        const char *addr = lua_tostring(L, 2);

        int res = zmq_bind(skt, addr);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }
    return 0;
}

int Lzmq_connect(lua_State* L)
{
    // int zmq_connect (void *s, const char *addr);
    int argc = lua_gettop(L);
    if (argc > 1)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isstring(L, 2))
        {
            return return_error(L, "arg[2]: string expected");
        }

        void *skt = lua_touserdata(L, 1);
        const char *addr = lua_tostring(L, 2);

        int res = zmq_connect(skt, addr);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }
    return 0;
}

int Lzmq_unbind(lua_State* L)
{
    // int zmq_unbind (void *s, const char *addr);
    int argc = lua_gettop(L);
    if (argc > 1)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isstring(L, 2))
        {
            return return_error(L, "arg[2]: string expected");
        }

        void *skt = lua_touserdata(L, 1);
        const char *addr = lua_tostring(L, 2);

        int res = zmq_unbind(skt, addr);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }
    return 0;
}

int Lzmq_disconnect(lua_State* L)
{
    // int zmq_disconnect (void *s, const char *addr);
    int argc = lua_gettop(L);
    if (argc > 1)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isstring(L, 2))
        {
            return return_error(L, "arg[2]: string expected");
        }

        void *skt = lua_touserdata(L, 1);
        const char *addr = lua_tostring(L, 2);

        int res = zmq_disconnect(skt, addr);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }
    return 0;
}

int Lzmq_send(lua_State* L)
{
    // int zmq_send (void *s, const void *buf, size_t len, int flags);

    // redirect propably TEMPORARILY to zmq_msg_send
    return Lzmq_msg_send(L);

    //return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_send_const(lua_State* L)
{
    // int zmq_send_const (void *s, const void *buf, size_t len, int flags);

    // redirect propably TEMPORARILY to zmq_msg_send
    return Lzmq_msg_send(L);

    //return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_recv(lua_State* L)
{
    // int zmq_recv (void *s, void *buf, size_t len, int flags);

    // redirect propably TEMPORARILY to zmq_msg_rcv
    return Lzmq_msg_recv(L);

    //return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_socket_monitor(lua_State* L)
{
    // int zmq_socket_monitor (void *s, const char *addr, int events);

    int argc = lua_gettop(L);
    if (argc > 2)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_isstring(L, 2))
        {
            return return_error(L, "arg[2]: string expected");
        }

        if (!lua_isnumber(L, 3))
        {
            return return_error(L, "arg[3]: number expected");
        }

        void *skt = lua_touserdata(L, 1);
        const char *addr = lua_tostring(L, 2);
        int events = lua_tointeger(L, 3);

        printf("socketmonitor: %p, %s, %d", skt, addr, events);
        int res = zmq_socket_monitor(skt, addr, events);
        if (res == -1)
        {
            return return_zmq_receive_error(L);
        }

        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "3 arguments expected");
    }

    return 0;
}

int Lzmq_sendmsg(lua_State* L)
{
    //return return_error(L, "not implemented (yet)"); // TODO

    // redirect propably TEMPORARILY to zmq_msg_send
    return Lzmq_msg_send(L);
}

int Lzmq_recvmsg(lua_State* L)
{
    //return return_error(L, "not implemented (yet)"); // TODO

    // redirect propably TEMPORARILY to zmq_msg_rcv
    return Lzmq_msg_recv(L);
}

int Lzmq_poll(lua_State* L)
{
    // int zmq_poll (zmq_pollitem_t *items, int nitems, long timeout);

    int argc = lua_gettop(L);
    if (argc > 2)
    {
        if (!lua_istable(L, 1))
        {
            return return_error(L, "arg[1]: table expected");
        }

        if (!lua_isnumber(L, 2))
        {
            return return_error(L, "arg[2]: number expected");
        }

        if (!lua_isfunction(L, 3))
        {
            return return_error(L, "arg[3]: function expected");
        }

        zmq_pollitem_t *items;
        int nitems = lua_rawlen(L, 1);
        //printf("item count: %d\n", nitems);

        items = malloc(sizeof(zmq_pollitem_t) * nitems);

        int i;

        for(i=0;i<nitems;i+=1)
        {
            lua_rawgeti(L, 1, i+1); // pushes
            if (lua_istable(L, -1))
            {
                int c = lua_rawlen(L, -1);
                if (c > 1)
                {
                    items[i].fd = 0;
                    items[i].revents = 0;

                    lua_rawgeti(L, -1, 1);
                    if (lua_islightuserdata(L, -1))
                    {
                        items[i].socket = lua_touserdata(L, -1);
                    }
                    lua_pop(L, 1);

                    lua_rawgeti(L, -1, 2);
                    if (lua_isnumber(L, -1))
                    {
                        items[i].events = lua_tointeger(L, -1);
                    }

                    lua_pop(L, 1);

                    //printf("zmq_pollitem_t: %d: %p\t%d\n", i, items[i].socket, items[i].events);
                }
            }

            lua_pop(L, 1);
        }

        long timeout = lua_tointeger(L,2);
        //printf("timeout: %ld\n", timeout);
        //lua_pushvalue(L, 3);

        //printf("zmq_poll: %d\t%ld\n", nitems, timeout);

        int res = zmq_poll(items, nitems, timeout);
        if (res == -1)
        {
            return return_zmq_receive_error(L);
        }

        for(i=0;i<nitems;i+=1)
        {
            lua_pushvalue(L, 3);
            lua_pushlightuserdata(L, items[i].socket);
            lua_pushinteger(L, items[i].revents);
            lua_call(L, 2, 0);
        }

        free(items);

        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "3 arguments expected");
    }

    return 0;
}

int Lzmq_proxy(lua_State* L)
{
    // int zmq_proxy (void *frontend, void *backend, void *capture);

    int argc = lua_gettop(L);
    if (argc > 1)
    {
        if (!lua_islightuserdata(L, 1))
        {
            return return_error(L, "arg[1]: userdata expected");
        }

        if (!lua_islightuserdata(L, 2))
        {
            return return_error(L, "arg[2]: userdata expected");
        }

        void *skt_front = lua_touserdata(L, 1);
        void *skt_back = lua_touserdata(L, 2);
        void *skt_capture = NULL;

        if (argc > 2)
        {
            if (!lua_islightuserdata(L, 3))
            {
                return return_error(L, "arg[3]: userdata expected");
            }
            skt_capture = lua_touserdata(L, 3);
        }

        int res = zmq_proxy(skt_front, skt_back, skt_capture);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }
        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        return return_error(L,  "3 arguments expected");
    }

    return 0;
}

int Lzmq_z85_encode(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_z85_decode(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}
