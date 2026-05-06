# Fakes FindFreetype.cmake for Emscripten
set(FREETYPE_FOUND TRUE)
set(FREETYPE_VERSION_STRING "2.10.0")

if(NOT TARGET Freetype::Freetype)
  add_library(Freetype::Freetype INTERFACE IMPORTED)
  target_link_options(Freetype::Freetype INTERFACE
      -sUSE_FREETYPE=1
      -sUSE_HARFBUZZ=1
  )

  target_compile_options(Freetype::Freetype INTERFACE
      -sUSE_FREETYPE=1
      -sUSE_HARFBUZZ=1
  )
endif()
