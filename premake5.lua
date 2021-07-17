-- premake5.lua
workspace "Game"
    architecture "x64"
    configurations { "Debug", "Release", cppdialect "C++17" }

project "Game"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files 
    { 
    "Game/src/**.cpp",
    "Game/include/**.h",

    "../extern/lib/Glad/glad.c",
    "../extern/include/stb/**.h",
    "../extern/include/imgui-master/*.h",
    "../extern/include/imgui-master/*.cpp",
    "../extern/include/imgui-master/backends/imgui_impl_opengl3.cpp",
    "../extern/include/imgui-master/backends/imgui_impl_sdl.cpp"
    }

    links
    {
    	"SDL2",
    	"SDL2main",
    	"opengl32"
    }

    sysincludedirs { "Game/include/" }

   	includedirs
    {
        "../extern/include/SDL",
        "../extern/include/Glad",
        "../extern/include/Glad/**",
        "../extern/include/stb",
        "../extern/include/entt",
        "../extern/include/imgui-master/",
        "../extern/include/imgui-master/backends"
    }

 	libdirs
    {
        "../extern/lib/SDL",
        "../extern/lib/Glad",
        "../extern/include/imgui-master/",
        "../extern/include/imgui-master/backends"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"