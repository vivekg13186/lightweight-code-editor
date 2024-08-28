solution "cle"
location("build")
configurations { "Debug", "Release" }
platforms { "native", "x64", "x32" }

project "lce"
kind "ConsoleApp"
language "C++"
files { "main.cpp", "editor.cpp" }
targetdir("build")

configuration { "linux" }
linkoptions { "`pkg-config --libs glfw3`" }
links { "GL", "GLU", "m", "GLEW" }

configuration { "windows" }
links { "glfw3", "gdi32", "winmm", "user32", "GLEW", "glu32", "opengl32", "kernel32" }
defines { "NANOVG_GLEW", "_CRT_SECURE_NO_WARNINGS" }

configuration { "macosx" }
links { "glfw3" }
linkoptions { "-framework OpenGL", "-framework Cocoa", "-framework IOKit", "-framework CoreVideo", "-framework Carbon" }

configuration "Debug"
defines { "DEBUG" }
flags { "Symbols", "ExtraWarnings" }

configuration "Release"
defines { "NDEBUG" }
flags { "Optimize", "ExtraWarnings" }
