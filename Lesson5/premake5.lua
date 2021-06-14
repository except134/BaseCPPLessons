COMPUTER_NAME       = os.getenv("COMPUTERNAME") 
LOCATION_DIR        = "Build/" ..  COMPUTER_NAME .. "/" .. _ACTION

-- Solution
solution ("Lesson5")
    location(LOCATION_DIR)
    configurations { "Release", "Debug" }
    platforms { "x64" }
    cppdialect("C++latest")
    language "C++"
    startproject("Lesson5")

    filter "configurations:Debug"
        optimize "Off"
        symbols "On"
        inlining "Disabled"
        flags { "MultiProcessorCompile" }
        defines { "_DEBUG" }
        libdirs {  }

    filter "configurations:Release"
        optimize "Full"
        symbols "Off"
        inlining "Auto"
        flags { "MultiProcessorCompile", "LinkTimeOptimization" }
        defines { "NDEBUG" }
        libdirs {  }

    filter "platforms:x64" 
        system "Windows"
        architecture "x64"
        systemversion("latest")
        defines { "WIN64" }
        staticruntime "On"
        buildoptions { "/Zm512 /bigobj" }
        includedirs { 
            "Sources", 
        }
        libdirs { 
             
        }

project "Lesson5"
    targetdir "Bin"
    debugdir "$(TargetDir)"
    targetname "$(ProjectName)$(PlatformArchitecture)$(Configuration)"
    kind "ConsoleApp"
    pchheader("Lesson5PCH.h") 
    pchsource("Sources/Lesson5PCH.cpp") 
    links {   }
    defines {  }
    files { 
        "Sources/Lesson5PCH.cpp",
        "Sources/Lesson5PCH.h",
        "Sources/**.h", 
        "Sources/**.c", 
        "Sources/**.hpp", 
        "Sources/**.cpp", 
        "Sources/**.inl", 
    }

