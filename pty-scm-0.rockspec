package = "pty"
version = "scm-0"
source = {
	url = "git://github.com/daurnimator/lua-pty.git";
}
description = {
	summary = "A lua library to call operating system pty primitives";
	homepage = "https://github.com/daurnimator/lua-pty";
	license = "MIT/X11";
}
supported_platforms = {
	"unix";
}
dependencies = {
	"lua >= 5.1, < 5.4";
}
build = {
	type = "builtin";
	modules = {
		["pty"] = {
			defines = { "_POSIX_C_SOURCE=200809L" };
			incdirs = { "vendor/compat-5.3/c-api/" };
			sources = {
				"pty/init.c";
			};
		};
	}
}
