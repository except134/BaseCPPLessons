COMPUTER_NAME       = os.getenv("COMPUTERNAME") 
LOCATION_DIR        = "Build/" ..  COMPUTER_NAME .. "/" .. _ACTION

-- Solution
solution ("Lesson3")
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
        defines { "_DEBUG", "_USRDLL" }
        libdirs {  }

    filter "configurations:Release"
        optimize "Full"
        symbols "Off"
        inlining "Auto"
        flags { "MultiProcessorCompile", "LinkTimeOptimization" }
        defines { "NDEBUG", "_USRDLL" }
        libdirs {  }

    filter "platforms:x64" 
        system "Windows"
        architecture "x64"
        systemversion("latest")
        defines { "WIN64" }
        staticruntime "On"
        buildoptions { "/Zm512 /bigobj" }
        includedirs { 
            "Sources/Program", 
            "Sources/DLL" 
        }
        libdirs { 
             
        }

project "Lesson3DLL"
    targetdir("Bin")
    debugdir "$(TargetDir)"
    targetname "$(ProjectName)$(PlatformArchitecture)$(Configuration)"
    kind "SharedLib"
    pchheader("Lesson3DLLPCH.h") 
    pchsource("Sources/DLL/Lesson3DLLPCH.cpp") 
    defines { 
        "_CRT_SECURE_NO_DEPRECATE",
        "_CRT_NONSTDC_NO_DEPRECATE",
        "_WINSOCK_DEPRECATED_NO_WARNINGS",
	"DLL_EXPORT"
    }
    files { 
        "Sources/DLL/Lesson3DLLPCH.cpp",
        "Sources/DLL/Lesson3DLLPCH.h",
        "Sources/DLL/**.h", 
        "Sources/DLL/**.c", 
        "Sources/DLL/**.hpp", 
        "Sources/DLL/**.cpp", 
        "Sources/DLL/**.inl", 
    }
    excludes {
    }

project "Lesson3"
    targetdir "Bin"
    debugdir "$(TargetDir)"
    targetname "$(ProjectName)$(PlatformArchitecture)$(Configuration)"
    kind "ConsoleApp"
    --flags { "NoPCH" }
    pchheader("Lesson3PCH.h") 
    pchsource("Sources/Program/Lesson3PCH.cpp") 
    links {  "Lesson3DLL" }
    defines { "DLL_IMPORT" }
    files { 
        "Sources/Program/Lesson3PCH.cpp",
        "Sources/Program/Lesson3PCH.h",
        "Sources/Program/**.h", 
        "Sources/Program/**.c", 
        "Sources/Program/**.hpp", 
        "Sources/Program/**.cpp", 
        "Sources/Program/**.inl", 
    }

