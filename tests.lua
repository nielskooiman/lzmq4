#!/usr/bin/env lua

local separator = string.rep('-', 60)
print(separator)


local test_count = 0;
local tests_success = 0;

local function test(description, expression, validate, print_value)
    test_count = test_count + 1

    local result, res, err

    if(expression and type(expression) == 'function') then
        res, err = pcall(function() result = expression() end);
    else
        result = expression
    end

    if (validate(result)) then
        tests_success = tests_success + 1
        print(string.format('%-40s %s', description, 'passed'))
        if print_value then
            print(string.format('%-40s %s', 'result', tostring(result)))
        end
    else
        print(string.format('%-40s %s', description, 'failed'))
        print(string.format('%-40s %s', 'result', tostring(result)))
        print(string.format('%-40s %s', 'error', tostring(err)))
    end
    return result
end

local function is_true(r)
    return r
end

local function is_number(r)
    return r and type(r) == 'number'
end

local function is_userdata(r)
    return r and type(r) == 'userdata'
end

local function is_table(r)
    return r and type(r) == 'table'
end


local zmq = assert(require('lzmq4'))

local start_time = os.time()

local version = test('zmq version', function() return zmq.version() end, is_table)
print(separator)
print( string.format("ZeroMQ %d.%d.%d", version.major, version.minor, version.patch))
print(separator)

local ZMQ = zmq.ZMQ

print(separator)
print('CONSTANTS')
print(separator)

test('VERSION_MAJOR', ZMQ.VERSION_MAJOR, is_number, true)
test('VERSION_MINOR', ZMQ.VERSION_MINOR, is_number, true)
test('VERSION_PATCH', ZMQ.VERSION_PATCH, is_number, true)

test('ENOTSUP', ZMQ.ENOTSUP, is_number, true)
test('EPROTONOSUPPORT', ZMQ.EPROTONOSUPPORT, is_number, true)
test('ENOBUFS', ZMQ.ENOBUFS, is_number, true)
test('ENETDOWN', ZMQ.ENETDOWN, is_number, true)
test('EADDRINUSE', ZMQ.EADDRINUSE, is_number, true)
test('EADDRNOTAVAIL', ZMQ.EADDRNOTAVAIL, is_number, true)
test('ECONNREFUSED', ZMQ.ECONNREFUSED, is_number, true)
test('EINPROGRESS', ZMQ.EINPROGRESS, is_number, true)
test('ENOTSOCK', ZMQ.ENOTSOCK, is_number, true)
test('EMSGSIZE', ZMQ.EMSGSIZE, is_number, true)
test('EAFNOSUPPORT', ZMQ.EAFNOSUPPORT, is_number, true)
test('ENETUNREACH', ZMQ.ENETUNREACH, is_number, true)
test('ECONNABORTED', ZMQ.ECONNABORTED, is_number, true)
test('ECONNRESET', ZMQ.ECONNRESET, is_number, true)
test('ETIMEDOUT', ZMQ.ETIMEDOUT, is_number, true)
test('EHOSTUNREACH', ZMQ.EHOSTUNREACH, is_number, true)
test('ENETRESET', ZMQ.ENETRESET, is_number, true)
test('EFSM', ZMQ.EFSM, is_number, true)
test('ENOCOMPATPROTO', ZMQ.ENOCOMPATPROTO, is_number, true)
test('ETERM', ZMQ.ETERM, is_number, true)
test('EMTHREAD', ZMQ.EMTHREAD, is_number, true)
test('IO_THREADS', ZMQ.IO_THREADS, is_number, true)
test('MAX_SOCKETS', ZMQ.MAX_SOCKETS, is_number, true)
test('IO_THREADS_DFLT', ZMQ.IO_THREADS_DFLT, is_number, true)
test('MAX_SOCKETS_DFLT', ZMQ.MAX_SOCKETS_DFLT, is_number, true)

test('PAIR', ZMQ.PAIR, is_number, true)
test('PUB', ZMQ.PUB, is_number, true)
test('SUB', ZMQ.SUB, is_number, true)
test('REQ', ZMQ.REQ, is_number, true)
test('REP', ZMQ.REP, is_number, true)
test('DEALER', ZMQ.DEALER, is_number, true)
test('ROUTER', ZMQ.ROUTER, is_number, true)
test('PULL', ZMQ.PULL, is_number, true)
test('PUSH', ZMQ.PUSH, is_number, true)
test('XPUB', ZMQ.XPUB, is_number, true)
test('XSUB', ZMQ.XSUB, is_number, true)
test('STREAM', ZMQ.STREAM, is_number, true)

test('AFFINITY', ZMQ.AFFINITY, is_number, true)
test('IDENTITY', ZMQ.IDENTITY, is_number, true)
test('SUBSCRIBE', ZMQ.SUBSCRIBE, is_number, true)
test('UNSUBSCRIBE', ZMQ.UNSUBSCRIBE, is_number, true)
test('RATE', ZMQ.RATE, is_number, true)
test('RECOVERY_IVL', ZMQ.RECOVERY_IVL, is_number, true)
test('SNDBUF', ZMQ.SNDBUF, is_number, true)
test('RCVBUF', ZMQ.RCVBUF, is_number, true)
test('RCVMORE', ZMQ.RCVMORE, is_number, true)
test('FD', ZMQ.FD, is_number, true)
test('EVENTS', ZMQ.EVENTS, is_number, true)
test('TYPE', ZMQ.TYPE, is_number, true)
test('LINGER', ZMQ.LINGER, is_number, true)
test('RECONNECT_IVL', ZMQ.RECONNECT_IVL, is_number, true)
test('BACKLOG', ZMQ.BACKLOG, is_number, true)
test('RECONNECT_IVL_MAX', ZMQ.RECONNECT_IVL_MAX, is_number, true)
test('MAXMSGSIZE', ZMQ.MAXMSGSIZE, is_number, true)
test('SNDHWM', ZMQ.SNDHWM, is_number, true)
test('RCVHWM', ZMQ.RCVHWM, is_number, true)
test('MULTICAST_HOPS', ZMQ.MULTICAST_HOPS, is_number, true)
test('RCVTIMEO', ZMQ.RCVTIMEO, is_number, true)
test('SNDTIMEO', ZMQ.SNDTIMEO, is_number, true)
test('LAST_ENDPOINT', ZMQ.LAST_ENDPOINT, is_number, true)
test('ROUTER_MANDATORY', ZMQ.ROUTER_MANDATORY, is_number, true)
test('TCP_KEEPALIVE', ZMQ.TCP_KEEPALIVE, is_number, true)
test('TCP_KEEPALIVE_CNT', ZMQ.TCP_KEEPALIVE_CNT, is_number, true)
test('TCP_KEEPALIVE_IDLE', ZMQ.TCP_KEEPALIVE_IDLE, is_number, true)
test('TCP_KEEPALIVE_INTVL', ZMQ.TCP_KEEPALIVE_INTVL, is_number, true)
test('TCP_ACCEPT_FILTER', ZMQ.TCP_ACCEPT_FILTER, is_number, true)
test('IMMEDIATE', ZMQ.IMMEDIATE, is_number, true)
test('XPUB_VERBOSE', ZMQ.XPUB_VERBOSE, is_number, true)
test('ROUTER_RAW', ZMQ.ROUTER_RAW, is_number, true)
test('IPV6', ZMQ.IPV6, is_number, true)
test('MECHANISM', ZMQ.MECHANISM, is_number, true)
test('PLAIN_SERVER', ZMQ.PLAIN_SERVER, is_number, true)
test('PLAIN_USERNAME', ZMQ.PLAIN_USERNAME, is_number, true)
test('PLAIN_PASSWORD', ZMQ.PLAIN_PASSWORD, is_number, true)
test('CURVE_SERVER', ZMQ.CURVE_SERVER, is_number, true)
test('CURVE_PUBLICKEY', ZMQ.CURVE_PUBLICKEY, is_number, true)
test('CURVE_SECRETKEY', ZMQ.CURVE_SECRETKEY, is_number, true)
test('CURVE_SERVERKEY', ZMQ.CURVE_SERVERKEY, is_number, true)
test('PROBE_ROUTER', ZMQ.PROBE_ROUTER, is_number, true)
test('REQ_CORRELATE', ZMQ.REQ_CORRELATE, is_number, true)
test('REQ_RELAXED', ZMQ.REQ_RELAXED, is_number, true)
test('CONFLATE', ZMQ.CONFLATE, is_number, true)
test('ZAP_DOMAIN', ZMQ.ZAP_DOMAIN, is_number, true)
test('MORE', ZMQ.MORE, is_number, true)
test('DONTWAIT', ZMQ.DONTWAIT, is_number, true)
test('SNDMORE', ZMQ.SNDMORE, is_number, true)

test('NULL', ZMQ.NULL, is_number, true)
test('PLAIN', ZMQ.PLAIN, is_number, true)
test('CURVE', ZMQ.CURVE, is_number, true)

test('EVENT_CONNECTED', ZMQ.EVENT_CONNECTED, is_number, true)
test('EVENT_CONNECT_DELAYED', ZMQ.EVENT_CONNECT_DELAYED, is_number, true)
test('EVENT_CONNECT_RETRIED', ZMQ.EVENT_CONNECT_RETRIED, is_number, true)
test('EVENT_LISTENING', ZMQ.EVENT_LISTENING, is_number, true)
test('EVENT_BIND_FAILED', ZMQ.EVENT_BIND_FAILED, is_number, true)
test('EVENT_ACCEPTED', ZMQ.EVENT_ACCEPTED, is_number, true)
test('EVENT_ACCEPT_FAILED', ZMQ.EVENT_ACCEPT_FAILED, is_number, true)
test('EVENT_CLOSED', ZMQ.EVENT_CLOSED, is_number, true)
test('EVENT_CLOSE_FAILED', ZMQ.EVENT_CLOSE_FAILED, is_number, true)
test('EVENT_DISCONNECTED', ZMQ.EVENT_DISCONNECTED, is_number, true)
test('EVENT_MONITOR_STOPPED', ZMQ.EVENT_MONITOR_STOPPED, is_number, true)
test('EVENT_ALL', ZMQ.EVENT_ALL, is_number, true)

test('POLLIN', ZMQ.POLLIN, is_number, true)
test('POLLOUT', ZMQ.POLLOUT, is_number, true)
test('POLLERR', ZMQ.POLLERR, is_number, true)

-- CUSTOM CONSTANTS FOR getsockopt AND setsockopt
test('OPT_INT', ZMQ.OPT_INT, is_number, true)
test('OPT_INT64', ZMQ.OPT_INT64, is_number, true)
test('OPT_UINT64', ZMQ.OPT_UINT64, is_number, true)
test('OPT_BINARY', ZMQ.OPT_BINARY, is_number, true)


print(separator)
print('FUNCTIONS')
print(separator)

-- TESTING CONTEXTS
local context = test('create context', function() return zmq.ctx_new() end, is_userdata)

-- TESTING CONTEXT SETTINGS

local io_threads = test('context option get IO_THREADS', function() return zmq.ctx_get(context, ZMQ.IO_THREADS) end, is_number, true)
local max_sockets = test('context option get MAX_SOCKETS', function() return zmq.ctx_get(context, ZMQ.MAX_SOCKETS) end, is_number, true)
local ipv6 = test('context option get IPV6', function() return zmq.ctx_get(context, ZMQ.IPV6) end, is_number, true)

local io_threads = test('context option set IO_THREADS', function() return zmq.ctx_set(context, ZMQ.IO_THREADS, 3) end, is_number, true)
local max_sockets = test('context option set MAX_SOCKETS', function() return zmq.ctx_set(context, ZMQ.MAX_SOCKETS, 256) end, is_number, true)
local ipv6 = test('context option set IPV6', function() return zmq.ctx_set(context, ZMQ.IPV6, 1) end, is_number, true)

local io_threads = test('context option get IO_THREADS', function() return zmq.ctx_get(context, ZMQ.IO_THREADS) end, is_number, true)
local max_sockets = test('context option get MAX_SOCKETS', function() return zmq.ctx_get(context, ZMQ.MAX_SOCKETS) end, is_number, true)
local ipv6 = test('context option get IPV6', function() return zmq.ctx_get(context, ZMQ.IPV6) end, is_number, true)


-- TESTING SOCKETS

local router = test('create router socket', function() return zmq.socket(context, ZMQ.ROUTER) end, is_userdata)
local dealer = test('create dealer socket', function() return zmq.socket(context, ZMQ.DEALER) end, is_userdata)

-- TESTING SOCKET SETTINGS

local socket_type = test('socket option get TYPE', function() return zmq.getsockopt(router, ZMQ.TYPE, ZMQ.OPT_INT) end, is_number, true)
local identity = test('socket option set IDENTITY', function() return zmq.setsockopt(router, ZMQ.IDENTITY, 'super-socket', ZMQ.OPT_BINARY) end, is_true, true)
local identity = test('socket option get IDENTITY', function() return zmq.getsockopt(router, ZMQ.IDENTITY, ZMQ.OPT_BINARY) end, is_true, true)

test('close dealer socket', function() return zmq.close(dealer) end, is_true)
test('close router socket', function() return zmq.close(router) end, is_true)

test('shutdown context', function() return zmq.ctx_shutdown(context) end, function (r) return r end)

print(separator)
end_time = os.time()
elapsed_time = os.difftime(end_time-start_time)
print('time elapsed: ' .. elapsed_time .. 's')
print('successfull tests: ' .. tests_success .. ' / ' .. test_count)
print(separator)

