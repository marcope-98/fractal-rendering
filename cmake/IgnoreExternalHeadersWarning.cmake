function(ignore_external_headers_warning target external_library_target)
  get_target_property(lib_include_dirs ${external_library_target} INTERFACE_INCLUDE_DIRECTORIES)
  target_include_directories(${target} SYSTEM PRIVATE ${lib_include_dirs})
endfunction(ignore_external_headers_warning)