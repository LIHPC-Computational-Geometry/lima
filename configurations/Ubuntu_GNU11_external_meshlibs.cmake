set (HDF5_DIR "/opt/HDF5/1.12.0_cmake" CACHE PATH "Chemin d'acces a la bibliotheque HDF5" FORCE)
set (HDF5_ROOT ${HDF5_DIR} CACHE PATH "Chemin d'acces a la bibliotheque HDF5" FORCE)
set (ZLIB_ROOT ${HDF5_DIR} CACHE PATH "Chemin d'acces a la bibliotheque ZLIB utilisée par HDF5" FORCE)
message (STATUS "======================================================== VARIABLES ENVIRONNEMENTALES DEPENDANCES MAILLAGE ========================================================")
message (STATUS "======================> HDF5_DIR		=${HDF5_DIR}")



