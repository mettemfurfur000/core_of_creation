-- script.lua
testString = "LuaBridge works!"
number = 42

function func( a, b )
	return a + b
end

function get_dir()
	os.execute("dir /b")
end