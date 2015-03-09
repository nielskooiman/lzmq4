package = "lzmq4"
version = "0.1-1"
source = {
	url= "git://github.com/Cogibucks/lzmq4",
	tag = "v0.1"
}
description = {
   summary = "Lua C bindings to ØMQ (ZeroMQ) 4.0",
   detailed = [[
		Lua C bindings to ØMQ (ZeroMQ) 4.0
   ]],
   homepage = "https://github.com/Cogibucks/lzmq4",
   license = "MIT/X11"
}
dependencies = {
   "lua >= 5.2"
   -- If you depend on other rocks, add them here
}
build = {
	type = "builtin",
	external_dependencies = {
		ZMQ = {
			header = "zmq.h"
		}
	},
	modules = {
		lzmq4 = {
			sources = { 
				"src/lzmq.c",
				"src/lzmq-context.c",
				"src/lzmq-message.c",	
				"src/lzmq-socket.c",
			},
			defines = {},
			libraries = { "zmq" },
			incdirs = { },
			libdirs = { }
		}
	},
	copy_directories = { } -- "doc", "test"
}

