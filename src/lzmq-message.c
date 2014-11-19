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

int Lzmq_msg_init(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_init_size(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_init_data(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_send(lua_State* L)
{
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

        size_t size = 0;

        void *skt = lua_touserdata(L, 1);
        const char *msgdata = lua_tolstring(L, 2, &size);
        int flags = lua_tointeger(L, 3);

        int res = 0;

        zmq_msg_t msg;
        res = zmq_msg_init_size(&msg, size);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        void *data = zmq_msg_data (&msg);
        memcpy(data, msgdata, size);
        res = zmq_msg_send(&msg, skt, flags);

        if (res < 0)
        {
            return return_zmq_receive_error(L);
        }

        res = zmq_msg_close(&msg);

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

int Lzmq_msg_recv(lua_State* L)
{
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

        void *skt = lua_touserdata(L, 1);
        int flags = lua_tointeger(L, 2);

        int res = 0;
        size_t size = 0;

        zmq_msg_t msg;
        res = zmq_msg_init (&msg);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        res = zmq_msg_recv(&msg, skt, flags);
        if (res == -1)
        {
            return return_zmq_receive_error(L);
        }

        size = res;

        char *data = (char*)zmq_msg_data (&msg);

        lua_pushlstring(L, data, size);

        res = zmq_msg_close (&msg);

        if (res == -1)
        {
            return return_zmq_error(L, res);
        }
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }

    return 0;
}

int Lzmq_msg_close(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_move(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_copy(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_data(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_size(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_more(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_get(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lzmq_msg_set(lua_State* L)
{
    return return_error(L, "not implemented (yet)"); // TODO
}

int Lmsg_recv_event(lua_State* L)
{
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

        void *skt = lua_touserdata(L, 1);
        int flags = lua_tointeger(L, 2);

        int res = 0;

        // ZMQ event
        zmq_event_t event;

        zmq_msg_t msg;
        res = zmq_msg_init (&msg);
        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        res = zmq_msg_recv(&msg, skt, flags);
        if (res == -1)
        {
            return return_zmq_receive_error(L);
        }
        memcpy (&event, zmq_msg_data (&msg), sizeof(event));

        lua_newtable(L);

        lua_pushnumber(L, event.event);
        lua_setfield(L, -2, "event");
        lua_pushnumber(L, event.value);
        lua_setfield(L, -2, "value");

        res = zmq_msg_close (&msg);

        if (res != 0)
        {
            return return_zmq_error(L, res);
        }

        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }

    return 0;
}
