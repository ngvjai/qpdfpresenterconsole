find_library(OSX_CORE_SERVICES_LIBRARY NAMES CoreServices)

if(OSX_CORE_SERVICES_LIBRARY)
	message(STATUS "Found OSX CoreServices Framework: ${OSX_CORE_SERVICES_LIBRARY}")
else(OSX_CORE_SERVICES_LIBRARY)
	message(FATAL_ERROR "Could NOT find OSX CoreServices Framework")
endif(OSX_CORE_SERVICES_LIBRARY)

mark_as_advanced(OSX_CORE_SERVICES_LIBRARY)
