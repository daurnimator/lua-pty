--[[
This example uses [lua-spawn](https://github.com/daurnimator/lua-spawn)
to spawn the 'echo' program, then reads the output using
[cqueues](http://25thandclement.com/~william/projects/cqueues.html).
]]
local pty = require "pty"
local spawn_posix = require "spawn.posix"
local wait = require "spawn.wait"
local cs = require "cqueues.socket"

local master = cs.fdopen(assert(pty.open()))
assert(pty.grant(master:pollfd()))
assert(pty.unlock(master:pollfd()))
local slave_path = assert(pty.ptsname(master:pollfd()))

local actions = spawn_posix.new_file_actions()
-- hard code open flags until https://github.com/daurnimator/lua-spawn/issues/2 is resolved
assert(actions:addopen(0, slave_path, 0, 0))
assert(actions:addopen(1, slave_path, 1, 0))
assert(actions:addopen(2, slave_path, 1, 0))
local attr = spawn_posix.new_attr()
local pid = assert(spawn_posix.spawnp("echo", actions, attr, {"echo", "hi"}))
assert(wait.waitpid(pid))

print(master:read())
