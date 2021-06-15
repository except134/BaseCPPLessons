function os.winSdkVersion()
    local reg_arch = iif(os.is64bit(), "\\Wow6432Node\\", "\\")
    local sdk_version = os.getWindowsRegistry("HKLM:SOFTWARE"..reg_arch.."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion")
    if sdk_version ~= nil then return sdk_version end
end

solution "ExceptEngine"
    configurations { "Debug", "Release" }
    compname = os.getenv("COMPUTERNAME") .. "/"
    location("Build/" ..  compname .. _ACTION)
    platforms { "x32", "x64" }
    language "C++"
    objdir("Build/" .. compname .. "Intermediate/" ..  _ACTION.. "/$(TargetName)")
    debugdir "$(TargetDir)"
    cppdialect "c++17"

project "ExceptEngine"
    filter { "system:windows", "action:vs*"}
    systemversion(os.winSdkVersion() .. ".0")    
    kind "WindowedApp"
    location("Build/" ..  compname .. _ACTION)
    includedirs {
        "Deps", 
        "Deps/Include", 
        "$(VULKAN_SDK)/Include", 
    }
    pchheader "ExceptEnginePCH.h"
    pchsource "Src/ExceptEnginePCH.cpp"
    files {
        "Src/ExceptEnginePCH.cpp",
        "Src/ExceptEnginePCH.h",
        "Src/**.h",
        "Src/**.hpp",
	"Src/**.inl", 
	"Src/**.cpp", 
	"Src/Res/**.*"
    }
    excludes
    {
    }

    targetdir "bin"
    targetname "$(ProjectName)$(PlatformArchitecture)$(Configuration)"

    libdirs {
        "Deps", 
        "Deps/Libs", 
        "Deps/Libs/$(PlatformShortName)", 
    }
    links {
    }
    buildoptions { "/wd4005 /Zm512 /MP" }

    configuration "Debug"
        flags { symbols "On", "StaticRuntime", entrypoint "wWinMainCRTStartup", "NoMinimalRebuild" }
        linkoptions { "" }
        libdirs {
            "Deps/Libs/$(PlatformShortName)/Debug", 
        }
        defines { "_DEBUG", "WIN32" }
        
    configuration "Release"
        optimize "Speed"
        flags { symbols "Off", "StaticRuntime", entrypoint "wWinMainCRTStartup", "NoFramePointer", editandcontinue "Off", "NoMinimalRebuild" }
        libdirs {
            "Deps/Libs/$(PlatformShortName)/Release", 
        }
        defines { "NDEBUG", "WIN32" }
        prebuildcommands { "$(SolutionDir)../../../Tools/VerBuild/verbuild.exe -t c -i bydate -f 10.*.+.+ -g all -p ExceptEngine_ -o $(SolutionDir)../../../Src/Res/VersionNo.h" }
