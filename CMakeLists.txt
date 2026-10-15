cmake_minimum_required(VERSION 3.27)
project(AiZO_Projekt_2)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        AiZO_Projekt_2 main.cpp
        Menu/Menu.cpp
        Menu/Menu.h
        Menu/Menu.h
        Tablica/Tablica.h
        Tablica/Sciezka.cpp
        Tablica/Sciezka.h
        Tablica/KrawedzSciezki.h
        Tablica/KopiecKrawedzi.cpp
        Tablica/KopiecKrawedzi.h
        Tablica/MacierzIncydencji.cpp
        Tablica/MacierzIncydencji.h
        Tablica/ListaSasiedztwa.cpp
        Tablica/ListaSasiedztwa.h
        Utils/GeneratorGrafow.cpp
        Utils/GeneratorGrafow.h
        Utils/CzytnikGrafow.cpp
        Utils/CzytnikGrafow.h
        Algorytmy/MST/Prim.cpp
        Algorytmy/MST/Prim.h
        Algorytmy/MST/Kruskal.cpp
        Algorytmy/MST/Kruskal.h
        Algorytmy/NajkrotszaSciezka/Dijkstra.cpp
        Algorytmy/NajkrotszaSciezka/Dijkstra.h
        Algorytmy/NajkrotszaSciezka/BellmanFord.cpp
        Algorytmy/NajkrotszaSciezka/BellmanFord.h
        Algorytmy/MaksymalnyPrzeplyw/FordFulkerson.cpp
        Algorytmy/MaksymalnyPrzeplyw/FordFulkerson.h
        Tablica/KrawedzSciezki.cpp
        Menu/GrafManager.cpp
        Menu/GrafManager.h
)

# Pomiar czasu wszystkich algorytmow dla obu reprezentacji (wynik CSV). Zawsze z optymalizacja.
add_executable(benchmark
        tools/benchmark.cpp
        Tablica/MacierzIncydencji.cpp
        Tablica/ListaSasiedztwa.cpp
        Algorytmy/MST/Kruskal.cpp
        Algorytmy/MST/Prim.cpp
        Algorytmy/NajkrotszaSciezka/Dijkstra.cpp
        Algorytmy/NajkrotszaSciezka/BellmanFord.cpp
        Algorytmy/MaksymalnyPrzeplyw/FordFulkerson.cpp
)
if (MSVC)
    target_compile_options(benchmark PRIVATE /O2)
else ()
    target_compile_options(benchmark PRIVATE -O2)
endif ()
