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


int return_error(lua_State* L,  char* str)
{
    // passive errors
    lua_pushnil(L);
    lua_pushlstring(L, str, strlen(str));
    return 2;

    // or throw lua error

//    lua_pushliteral(L, "arg[2]: number expected");
//    lua_error(L); // breaks function
//    return 0;
}

int return_zmq_error(lua_State* L, int errnumber)
{
    const char* strerror = zmq_strerror(errnumber);
    int strerrorlen = strlen(strerror);

    lua_pushnil(L);
    lua_pushlstring(L, strerror, strerrorlen);

    errnumber = zmq_errno();
    if (errnumber != 0)
    {
        strerror = zmq_strerror(errnumber);
        strerrorlen = strlen(strerror);

        lua_pushlstring(L, strerror, strerrorlen);
        lua_pushnumber(L, errnumber);
        return 4;
    }
    return 2;
}

int return_zmq_receive_error(lua_State* L)
{
    int errnumber = zmq_errno();
    const char* strerror = zmq_strerror(errnumber);
    int strerrorlen = strlen(strerror);

    lua_pushnil(L);
    lua_pushlstring(L, strerror, strerrorlen);

    lua_pushnumber(L, errnumber);
    return 3;
}


int Lzmq_version(lua_State* L)
{
    // void zmq_version (int *major, int *minor, int *patch);
    int major, minor, patch;
    zmq_version (&major, &minor, &patch);

    lua_newtable(L);
    //lua_settable(L, -2)
    lua_pushnumber(L, major);
    lua_setfield(L, -2, "major");
    lua_pushnumber(L, minor);
    lua_setfield(L, -2, "minor");
    lua_pushnumber(L, patch);
    lua_setfield(L, -2, "patch");

    return 1;
}

int Lzmq_errno(lua_State* L)
{
    //int zmq_errno (void);
    int errnumber = zmq_errno();
    lua_pushnumber(L, errnumber);
    return 1;
}

int Lzmq_strerror(lua_State* L)
{
    //const char *zmq_strerror (int errnum);
    int argc = lua_gettop(L);
    int errnumber = 0;
    if (argc > 0)
    {
        if( lua_isnumber(L, 1) )
        {
            errnumber = lua_tointeger(L, 1);
        }
        else
        {
            return return_error(L, "arg[1]: number expected");
        }
    }
    const char* strerror = zmq_strerror(errnumber);
    int strerrorlen = strlen(strerror);
    lua_pushlstring(L, strerror, strerrorlen);
    return 1;
}

int Lzmq_ctx_new(lua_State* L)
{
    // void *zmq_ctx_new (void);
    void* p = zmq_ctx_new();
    lua_pushlightuserdata(L, p);
    return 1;
}

int Lzmq_ctx_term(lua_State* L)
{
    // int zmq_ctx_term (void *context);
    int argc = lua_gettop(L);
    if (argc > 0 && lua_islightuserdata(L, 1))
    {
        void *ctx = lua_touserdata(L, 1);

        int res = zmq_ctx_term(ctx);
        if (res != 0)
        {
            return return_zmq_error(L, res);
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

int Lzmq_ctx_shutdown(lua_State* L)
{
    // int zmq_ctx_shutdown (void *ctx_);
    int argc = lua_gettop(L);
    if (argc > 0 && lua_islightuserdata(L, 1))
    {
        void *ctx = lua_touserdata(L, 1);
        //printf("%p\n", ctx);
        int res = zmq_ctx_shutdown(ctx);

        if (res != 0)
        {
            return return_zmq_error(L, res);
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

int Lzmq_ctx_set(lua_State* L)
{
    //int zmq_ctx_set (void *context, int option, int optval);

    int argc = lua_gettop(L);
    if (argc > 2)
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


        void *ctx = lua_touserdata(L, 1);
        int option = lua_tointeger(L, 2);
        int optval = lua_tointeger(L, 3);
        //printf("%p\n", ctx);
        int res = zmq_ctx_set(ctx, option, optval);
        if (res == -1)
        {
            return return_zmq_error(L, res);
        }
        lua_pushnumber(L, res);
        return 1;
    }
    else
    {
        return return_error(L,  "3 arguments expected");
    }

    return 0;
}

int Lzmq_ctx_get(lua_State* L)
{
    //int zmq_ctx_get (void *context, int option);

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
        int option = lua_tointeger(L, 2);
        int res = zmq_ctx_get(ctx, option);
        if (res == -1)
        {
            return return_zmq_error(L, res);
        }
        lua_pushnumber(L, res);
        return 1;
    }
    else
    {
        return return_error(L,  "2 arguments expected");
    }

    return 0;
}

