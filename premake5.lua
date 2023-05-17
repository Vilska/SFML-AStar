workspace "SFMLAstar"
   architecture "x64"
   configurations { "Debug", "Release" }

project "SFMLAstar"
   location "SFMLAstar"
   kind "ConsoleApp"
   language "C++"
   
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int/%{cfg.buildcfg}"

   files { "SFMLAstar/src/**.*" }

   filter "configurations:*"
      defines { "SFML_STATIC" }
      includedirs { "vendor/SFML/include" }
      libdirs { "vendor/SFML/lib" }
      links {
         "opengl32",
         "freetype",
         "winmm",
         "gdi32",
         "flac",
         "vorbisenc",
         "vorbisfile",
         "vorbis",
         "ogg",
         "ws2_32"
      }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      links {   
         "sfml-graphics-s-d",
         "sfml-window-s-d",
         "sfml-system-s-d",
         "sfml-audio-s-d",
         "sfml-network-s-d"
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      links {   
         "sfml-graphics-s",
         "sfml-window-s",
         "sfml-system-s",
         "sfml-audio-s",
         "sfml-network-s"
      }