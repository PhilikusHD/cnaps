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

-- C# Frontend Project
project "ChatbotFrontend"
    kind "WindowedApp"
    language "C#"
    dotnetframework "net8.0-windows"
    targetdir "bin/%{cfg.buildcfg}/x64"
    objdir "bin-int/%{cfg.buildcfg}/x64"
    location "ChatbotFrontend"

    files { "ChatbotFrontend/**.cs" }
    files { "ChatbotFrontend/assets/**" }
    
    -- Avoid generating duplicate AssemblyInfo attributes
    clr "Off"
    flags {"ShadowedVariables", "WPF"}
    linktimeoptimization "On"
    defines { "WINDOWS" }

    -- Ensure the C++ backend builds first
    dependson { "ChatbotCore" }

    postbuildcommands {
        -- Copy the C++ backend DLL to the C# output directory for dynamic linking
        "{COPY} %{wks.location}bin/%{cfg.buildcfg}/x64/ChatbotCore.dll %{wks.location}bin/%{cfg.buildcfg}/x64/net8.0-windows"
    }

    filter { "files:ChatbotFrontend/assets/**" }
        buildaction "Resource"
    filter {}

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
    includedirs { "ChatbotCore/src" }

    filter "system:windows"
        systemversion "latest"
        architecture "x86_64"

    filter {}