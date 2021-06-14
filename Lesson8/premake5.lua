COMPUTER_NAME       = os.getenv("COMPUTERNAME") 
LOCATION_DIR        = "Build/" ..  COMPUTER_NAME .. "/" .. _ACTION

-- Solution
solution ("Lesson8")
    location(LOCATION_DIR)
    configurations { "Release", "Debug" }
    platforms { "x64" }
    cppdialect("C++latest")
    language "C++"
    startproject("Lesson8")

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

project "Lesson8"
    targetdir "Bin"
    debugdir "$(TargetDir)"
    targetname "$(ProjectName)$(PlatformArchitecture)$(Configuration)"
    kind "WindowedApp"
    pchheader("Lesson8PCH.h") 
    pchsource("Sources/Lesson8PCH.cpp") 
    links {   }
    defines {  }
    files { 
        "Sources/Lesson8PCH.cpp",
        "Sources/Lesson8PCH.h",
        "Sources/**.h", 
        "Sources/**.c", 
        "Sources/**.hpp", 
        "Sources/**.cpp", 
        "Sources/**.inl", 
        "Sources/**.rc", 
    }

