set (HDF5_DIR "/opt/HDF5/1.12.0_cmake" CACHE PATH "Chemin d'acces a la bibliotheque HDF5" FORCE)
set (HDF5_ROOT ${HDF5_DIR} CACHE PATH "Chemin d'acces a la bibliotheque HDF5" FORCE)
set (ZLIB_ROOT ${HDF5_DIR} CACHE PATH "Chemin d'acces a la bibliotheque ZLIB utilisée par HDF5" FORCE)
message (STATUS "======================================================== VARIABLES ENVIRONNEMENTALES DEPENDANCES MAILLAGE ========================================================")
message (STATUS "======================> HDF5_DIR		=${HDF5_DIR}")

set (HDF145_DIR "/opt22/hdf145/1.3.1" CACHE PATH "Chemin d'acces a la bibliotheque HDF 145" FORCE)
set (HDF145_ROOT ${HDF145_DIR} CACHE PATH "Chemin d'acces a la bibliotheque HDF 145" FORCE)
set (HDF145_INCLUDE_DIR "${HDF145_DIR}/include" CACHE PATH "Chemin d'acces aux entetes de la bibliotheque HDF 145" FORCE)
set (HDF145CPP_LIBRARY "${HDF145_DIR}/lib/libhdf145_cpp.so" CACHE PATH "Chemin d'acces aux entetes de la bibliotheque HDF 145" FORCE)
set (HDF145_LIBRARY "${HDF145_DIR}/lib/libhdf145.so" CACHE PATH "Chemin d'acces aux entetes de la bibliotheque HDF 145" FORCE)

