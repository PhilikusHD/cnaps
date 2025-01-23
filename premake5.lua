-- Solution Configuration
workspace "ChatbotSolution"
    configurations { "Debug", "Release" }
    platforms { "Any CPU" }

    -- Global settings
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

-- C++ Backend Project
project "ChatbotCore"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}/x64"
    objdir "bin-int/%{cfg.buildcfg}/x64"
    location "ChatbotCore"

    files { "ChatbotCore/**.h", "ChatbotCore/**.cpp" }
    includedirs { "ChatbotCore/include" }

    filter "system:windows"
        systemversion "latest"
        architecture "x86_64"

    filter {}

-- C# Frontend Project
project "ChatbotFrontend"
    kind "WindowedApp"
    language "C#"
    dotnetframework "net8.0"
    targetdir "bin/%{cfg.buildcfg}/x64"
    objdir "bin-int/%{cfg.buildcfg}/x64"
    location "ChatbotFrontend"

    files { "ChatbotFrontend/**.cs" }

    -- Avoid generating duplicate AssemblyInfo attributes
    clr "Off"

    postbuildcommands {
        -- Copy the C++ backend DLL to the C# output directory for dynamic linking
        "{COPY} %{wks.location}bin/%{cfg.buildcfg}/x64/ChatbotCore.dll %{wks.location}bin/%{cfg.buildcfg}/x64/net8.0"
    }

    filter {}
