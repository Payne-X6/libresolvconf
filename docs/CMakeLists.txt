find_package(Sphinx COMPONENTS breathe)

if(Sphinx_FOUND AND Sphinx_breathe_FOUND)
	sphinx_add_docs(doc
		BREATHE_PROJECTS libresolvconf_doxygen
		BUILDER html
		SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
else()
	message("Unable to build target 'doc': Missing dependencies!")
endif()