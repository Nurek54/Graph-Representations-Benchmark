// Pomiar czasu wszystkich algorytmow dla obu reprezentacji grafu.
// Grafy sa generowane tym samym schematem co GeneratorGrafow (cykl 0 -> 1 -> ... -> V-1 -> 0,
// potem losowe pary wierzcholkow do zadanej gestosci), ale z wlasnym ziarnem dla kazdej instancji,
// zeby kolejne instancje w serii byly rozne. Czas to ten, ktory algorytm sam wypisuje (w mikrosekundach),
// usredniony z serii instancji.
//
// Uzycie: benchmark [instancje] [maks_wierzcholki]   (wynik w formacie CSV na stdout)

#include "../Tablica/MacierzIncydencji.h"
#include "../Tablica/ListaSasiedztwa.h"
#include "../Algorytmy/MST/Kruskal.h"
#include "../Algorytmy/MST/Prim.h"
#include "../Algorytmy/NajkrotszaSciezka/Dijkstra.h"
#include "../Algorytmy/NajkrotszaSciezka/BellmanFord.h"
#include "../Algorytmy/MaksymalnyPrzeplyw/FordFulkerson.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace {

struct Graf {
    size_t wierzcholki = 0, krawedzie = 0;
    std::vector<size_t> dane;  // (poczatek, koniec, waga) dla kazdej krawedzi
};

Graf generuj(size_t v, double gestosc, size_t maxWaga, unsigned ziarno) {
    std::mt19937 rng(ziarno);
    auto los = [&](size_t n) { return static_cast<size_t>(rng() % n); };
    Graf g;
    g.wierzcholki = v;
    g.krawedzie = std::max(static_cast<size_t>(gestosc * v * (v - 1) / 2), v);
    std::vector<char> jest(v * v, 0);
    auto dodaj = [&](size_t a, size_t b) {
        g.dane.insert(g.dane.end(), {a, b, los(maxWaga) + 1});
        jest[a * v + b] = jest[b * v + a] = 1;
    };
    for (size_t i = 0; i + 1 < v; ++i) dodaj(i, i + 1);
    dodaj(v - 1, 0);
    while (g.dane.size() / 3 < g.krawedzie) {
        size_t a = los(v), b = los(v);
        if (a != b && !jest[a * v + b]) dodaj(a, b);
    }
    return g;
}

// Algorytmy wypisuja swoj czas i wynik na cout; przechwytujemy to i odczytujemy zmierzony czas.
long long zmierz(const std::function<void()> &f) {
    std::ostringstream bufor;
    auto *stary = std::cout.rdbuf(bufor.rdbuf());
    f();
    std::cout.rdbuf(stary);
    static const std::regex wzor(R"((\d+) mikrosekund)");
    std::smatch m;
    const std::string tekst = bufor.str();
    return std::regex_search(tekst, m, wzor) ? std::stoll(m[1]) : -1;
}

}  // namespace

int main(int argc, char **argv) {
    const int instancje = argc > 1 ? std::atoi(argv[1]) : 10;
    const size_t maksV = argc > 2 ? std::strtoul(argv[2], nullptr, 10) : 300;
    const size_t maxWaga = 100;

    std::cout << "algorithm,representation,vertices,density,edges,mean_us,instances" << std::endl;
    for (size_t v : {25, 50, 100, 150, 200, 250, 300}) {
        if (v > maksV) continue;
        for (int procent : {25, 50, 99}) {
            std::vector<std::string> nazwy = {"kruskal", "prim", "dijkstra", "bellman_ford", "ford_fulkerson"};
            std::vector<long long> sumaM(nazwy.size(), 0), sumaL(nazwy.size(), 0);
            size_t e = 0;
            for (int k = 0; k < instancje; ++k) {
                Graf g = generuj(v, procent / 100.0, maxWaga, 1000u * static_cast<unsigned>(v) + 10u * procent + k);
                e = g.krawedzie;
                MacierzIncydencji m(g.krawedzie, g.wierzcholki, g.dane.data());
                ListaSasiedztwa l(g.krawedzie, g.wierzcholki, g.dane.data());
                const int s = 0, t = static_cast<int>(v) - 1;
                std::vector<std::function<void()>> naM = {
                    [&] { Kruskal().uruchomDlaMacierzy(m); }, [&] { Prim::uruchomDlaMacierzy(m); },
                    [&] { Dijkstra::uruchomDlaMacierzy(m, s); }, [&] { BellmanFord::uruchomDlaMacierzy(m, s); },
                    [&] { FordFulkerson::uruchomDlaMacierzy(m, s, t); }};
                std::vector<std::function<void()>> naL = {
                    [&] { Kruskal().uruchomDlaListy(l); }, [&] { Prim::uruchomDlaListy(l); },
                    [&] { Dijkstra::uruchomDlaListy(l, s); }, [&] { BellmanFord::uruchomDlaListy(l, s); },
                    [&] { FordFulkerson::uruchomDlaListy(l, s, t); }};
                for (size_t a = 0; a < nazwy.size(); ++a) {
                    sumaM[a] += zmierz(naM[a]);
                    sumaL[a] += zmierz(naL[a]);
                }
            }
            for (size_t a = 0; a < nazwy.size(); ++a) {
                std::cout << nazwy[a] << ",matrix," << v << ',' << procent << ',' << e << ','
                          << static_cast<double>(sumaM[a]) / instancje << ',' << instancje << '\n';
                std::cout << nazwy[a] << ",list," << v << ',' << procent << ',' << e << ','
                          << static_cast<double>(sumaL[a]) / instancje << ',' << instancje << std::endl;
            }
        }
    }
    return 0;
}
