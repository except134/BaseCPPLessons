COMPUTER_NAME       = os.getenv("COMPUTERNAME") 
LOCATION_DIR        = "Build/" ..  COMPUTER_NAME .. "/" .. _ACTION

-- Solution
solution ("Lesson6")
    location(LOCATION_DIR)
    configurations { "Release", "Debug" }
    platforms { "x64" }
    cppdialect("C++latest")
    language "C++"
    startproject("Lesson3")

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

project "Lesson6"
    targetdir "Bin"
    debugdir "$(TargetDir)"
    targetname "$(ProjectName)$(PlatformArchitecture)$(Configuration)"
    kind "ConsoleApp"
    pchheader("Lesson6PCH.h") 
    pchsource("Sources/Lesson6PCH.cpp") 
    links {   }
    defines {  }
    files { 
        "Sources/Lesson6PCH.cpp",
        "Sources/Lesson6PCH.h",
        "Sources/**.h", 
        "Sources/**.c", 
        "Sources/**.hpp", 
        "Sources/**.cpp", 
        "Sources/**.inl", 
    }

