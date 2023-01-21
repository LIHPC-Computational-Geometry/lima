set (GNU_ROOT /usr)
set(CMAKE_C_COMPILER gcc-7)
set(CMAKE_CXX_COMPILER g++-7)
set(CMAKE_CXX_FLAGS -std=c++11)
set(CMAKE_Fortran_COMPILER "${GNU_ROOT}/bin/gfortran-9")

#set (CMAKE_Fortran_FLAGS "-fdefault-real-8")	# I4/R8 insuffisant si des double precision sont déclarés car alors promus en R16
set (CMAKE_Fortran_FLAGS)
if (INT_8)
	string(APPEND CMAKE_Fortran_FLAGS " -fdefault-integer-8")
endif (INT_8)
if (REAL_8)
	string (APPEND CMAKE_Fortran_FLAGS " -fdefault-real-8 -fdefault-double-8")
endif (REAL_8)

# SWIG_EXECUTABLE : requiert d'être en cache pour fonctionner tout au long de la chaine ...
set (SWIG_EXECUTABLE "/opt/swig/3.0.12/bin/swig" CACHE FILEPATH "Swig exe" FORCE)

message (STATUS "========================================================== VARIABLES DE COMPILATION ==========================================================")
message (STATUS "======================> CMAKE_C_COMPILER		=${CMAKE_C_COMPILER}")
message (STATUS "======================> CMAKE_CXX_COMPILER		=${CMAKE_CXX_COMPILER}")
message (STATUS "======================> CMAKE_Fortran_COMPILER		=${CMAKE_Fortran_COMPILER}")
message (STATUS "======================> CMAKE_C_FLAGS			=${CMAKE_C_FLAGS}")
message (STATUS "======================> CMAKE_CXX_FLAGS		=${CMAKE_CXX_FLAGS}")
message (STATUS "======================> CMAKE_Fortran_FLAGS		=${CMAKE_Fortran_FLAGS}")
message (STATUS "======================> CMAKE_EXE_LINKER_FLAGS_INIT	=${CMAKE_EXE_LINKER_FLAGS_INIT}")
message (STATUS "======================> CMAKE_SHARED_LINKER_FLAGS_INIT	=${CMAKE_SHARED_LINKER_FLAGS_INIT}")

