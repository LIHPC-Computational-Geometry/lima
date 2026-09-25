message (STATUS "===============> Compilation du noyau Lima")
add_subdirectory (Lima)

if (BUILD_SCRIPTING)
	message (STATUS "===============> Compilation de l'API scripting de Lima")
	add_subdirectory (LimaScripting)
endif ( )

if (BUILD_TESTS)
	message (STATUS "===============> Compilation des executables tests de Lima")
	add_subdirectory (Comparaison_exe)
endif ( )

if (BUILD_XLMLIMA)	# and other tools
	message (STATUS "===============> Compilation des utilitairess xlmlima, symetrise et split-tetra")
	add_subdirectory (Xlmlima_exe)
	add_subdirectory (Symetrise)
	add_subdirectory (SplitTetra)
endif ( )

enable_testing ( )
