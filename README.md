# Symulacja rozchodzenia się fali dźwiękowej
Projekt stanowi uproszczoną symulację propagacji fali dźwiękowej w dwuwymiarowej przestrzeni, zbudowaną w celu analizy rozkładu amplitudy oraz potencjalnego znalezienia punktu o najwyższej amplitudzie.

### Opis działania
Środowisko symulacji:

Przestrzeń reprezentowana jest przez dwuwymiarową macierz komórek.
Na brzegach siatki umieszczane są „ściany” odbijające fale.
W jednym z punktów inicjowane jest źródło fali dźwiękowej.

### Model propagacji:

Symulacja przebiega poklatkowo.
Fala rozchodzi się w czterech głównych kierunkach (góra, dół, lewo, prawo).
Każda komórka reprezentowana jest przez strukturę:
```
struct Fala {
    int id[4];            // identyfikatory kierunków
    float amp[4][2];      // amplitudy: [kierunek][pasywna/aktywna]
    int state[4][2];      // stany: 0 - brak, 1 - pasywny, 2 - aktywny
};
```
### Mechanizm aktualizacji:

W każdej klatce symulacji obliczana jest suma aktywnych amplitud z sąsiednich komórek.
Wynik przekazywany jest do kolejnych komórek jako amplitudy pasywne.
Następnie amplitudy pasywne stają się aktywne i cykl się powtarza.

### Status projektu:
Projekt nie został w pełni dopracowany.

Każda klatka zostaje zapisana do pliku Frames.txt, które są następnie odczytywane przez program Symulacja.py, który zamienia je na animację

## Jeden impuls  
![Fale single](https://github.com/user-attachments/assets/2a6cccda-9ec0-4267-bd5d-325067af6d1d)  
## Sygnał ciągły  
![Fale continous](https://github.com/user-attachments/assets/ed3ffba4-a452-4d52-a049-000415ca3331)  
