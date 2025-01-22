-- Solution Configuration
workspace "ChatbotSolution"
    configurations { "Debug", "Release" }
    platforms { "x64" } -- Ensure we are targeting x64

    -- Global settings
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

-- C# Frontend Project
project "ChatbotFrontend"
    kind "WindowedApp"
    language "C#"
    dotnetframework "net8.0"
    location "ChatbotFrontend"
    architecture "x64"

    files { "ChatbotFrontend/**.cs" }
    links { "ChatbotCore" }

    filter "platforms:x64"
        architecture "x64"

    -- Post-build command to copy the DLL from ChatbotCore to the frontend's directory
    postbuildcommands {
        "{COPY} ../bin/%{cfg.buildcfg}/%{cfg.platform}/ChatbotCore.dll %{cfg.targetdir}"
    }

    filter {}

-- C++ Backend Project
project "ChatbotCore"
    kind "SharedLib" -- Compiling the backend as a DLL
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.platform}" -- DLL output directory, same as frontend
    objdir "bin-int/%{cfg.buildcfg}/%{cfg.platform}" -- Intermediate directory
    location "ChatbotCore"
    architecture "x64"


    files { "ChatbotCore/**.h", "ChatbotCore/**.cpp" }
    includedirs { "ChatbotCore/include" }

    filter "system:windows"
        systemversion "latest"
    filter "platforms:x64"
        architecture "x64"

    filter {}

