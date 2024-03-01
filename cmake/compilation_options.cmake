# -------------------------------------------------------------------------------------------------------------------------
# Le contenu de ce fichier permet de gérer des options de compilation indispensables à une bonne utilisation de Lima.
# Ces options de compilation définissent la taille d'encodage des entiers et réels.
# C'est par exemple indispensable à des codes fortran dont les arguments transmis lors des appels Lima sont de type
# INTEGER ou REAL.
# -------------------------------------------------------------------------------------------------------------------------

# ---------------------------------
# manage integer    
# ---------------------------------
#
# I4 
#    C and C++
#    Fortran if Intel
#    Fortran if GNU
#    Fortran if ARM
#    Fortran if FLANG
#    Fortran if IBM
#    Fortran if PGI
# I8 
#    C and C++
#    Fortran if Intel
#    Fortran if GNU
#    Fortran if ARM
#    Fortran if FLANG
#    Fortran if IBM
#    Fortran if PGI
# ---------------------------------
if (NOT INT_8)
  set (LIMA_INT_SIZE "INTEGER_32")
else() # I8
  set (LIMA_INT_SIZE "INTEGER_64")
endif()


# ---------------------------------
# manage real
# ---------------------------------
#
# R4 
#    C and C++
#    Fortran if Intel
#    Fortran if GNU
#    Fortran if ARM
#    Fortran if FLANG
#    Fortran if IBM
#    Fortran if PGI
# R8 
#    C and C++
#    Fortran if Intel
#    Fortran if GNU
#    Fortran if ARM
#    Fortran if FLANG
#    Fortran if IBM
#    Fortran if PGI
# ---------------------------------
if (NOT REAL_8)
  set (LIMA_REAL_SIZE "REAL_32")
else() # R8
  set (LIMA_REAL_SIZE "REAL_64")
endif()



if (DEFINED CMAKE_Fortran_COMPILER_ID)
        if(CMAKE_Fortran_COMPILER_ID STREQUAL "ARM" OR CMAKE_Fortran_COMPILER MATCHES "armflang") # cmake does always have proper COMPILER_ID for arm compiler
          set(i4_opt "") # -i4 does not exists, but it's the default
          set(i8_opt -i8)
          set(r4_opt "") # -r4 does not exists, but it's the default
          set(r8_opt -r8)
          set(r16_opt -r16) # future ?
        elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU") # gnu (gcc) gfortran
          set(i4_opt "") # -fdefault-integer-4 does not exists, but it's the default
          set(i8_opt -fdefault-integer-8)
          set(r4_opt "") # -fdefault-real-4 does not exists, but it's default
          set(r8_opt -fdefault-real-8)
          set(r16_opt -fdefault-real-16) # future ?
        elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "Intel" OR CMAKE_Fortran_COMPILER_ID STREQUAL "IntelLLVM") # intel (icc/icx) ifort/ifx
          set(i4_opt -i4)
          set(i8_opt -i8)
          set(r4_opt "") # -r4 does not exists, but it's the default
          set(r8_opt -r8)
          set(r16_opt -r16)
        elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "Flang") # llvm (clang) flang
          set(i4_opt "")
          set(i8_opt -i8)
          set(r4_opt "")
          set(r8_opt -r8)
          set(r16_opt -r16) # future ?
        elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "XL" OR CMAKE_Fortran_COMPILER_NAME MATCHES "xlf*") # VisualAge (ibm) xlfortran
          # set(i2_opt  -qintsize=2)   # available but not used
          set(i4_opt  -qintsize=4)   # (default)
          set(i8_opt  -qintsize=8)
          set(r4_opt  -qrealsize=4)  # (default)
          set(r8_opt  -qrealsize=8)
          set(r16_opt -qrealsize=16) # not present in xlf (xlf_r) 16.1
        elseif(CMAKE_Fortran_COMPILER_ID STREQUAL "PGI" OR CMAKE_Fortran_COMPILER_ID STREQUAL "NVHPC") # PGI (portland group) pgf[77,90,95] = pgfortran
          # set(i2_opt  -i2)           # available but not used
          set(i4_opt  -i4)           # (default)
          set(i8_opt  -i8)
          set(r4_opt  -r4)           # (default)
          set(r8_opt  -r8)
          set(r16_opt -r16)          # not present in pgfortran 20.9-0
        else()
          message(FATAL_ERROR
            "Unknown Fortran compiler '${CMAKE_Fortran_COMPILER_ID}' ('${CMAKE_Fortran_COMPILER}'), "
            "IR flags not implemented.")
        endif()

        set(FORTRAN_FLAGS "")
        set(FORTRAN_FLAGS_LIST "")
        if (NOT INT_8)
            string (APPEND FORTRAN_FLAGS " ${i4_opt}")
            list (APPEND FORTRAN_FLAGS_LIST "${i4_opt}")
        else()
            string (APPEND FORTRAN_FLAGS " ${i8_opt}")
            list (APPEND FORTRAN_FLAGS_LIST "${i8_opt}")
        endif()

        if (NOT REAL_8)
            string (APPEND FORTRAN_FLAGS " ${r4_opt}")
            list (APPEND FORTRAN_FLAGS_LIST "${r4_opt}")
        else()
            string (APPEND FORTRAN_FLAGS " ${r8_opt}")
            list (APPEND FORTRAN_FLAGS_LIST "${r8_opt}")
        endif()
		set(FORTRAN_FLAGS "")
		string (REPLACE ";" " " FORTRAN_FLAGS "${FORTRAN_FLAGS_LIST}")
        # set compile flags
        
        set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${FORTRAN_FLAGS}" CACHE FILEPATH "" FORCE)

endif()
