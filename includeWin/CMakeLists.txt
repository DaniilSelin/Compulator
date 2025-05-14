cmake_minimum_required(VERSION 3.16)

project(Compuler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Compuler main.cpp
    Structs.h
    Lexeme.h
    EnumClasses.h
    LexemeReader.h LexemeReader.cpp
    GenOPS.h GenOPS.cpp
    AdditionalFunctions.h
    include.txt

)

include(GNUInstallDirs)
install(TARGETS Compuler
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
