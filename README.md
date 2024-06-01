# shadow-matrix

Rozwiązanie zadania dot. mnożenia macierzy z cieniem.

Przygotowano 2 programy: 
* implementujący testy jednostkowe zaproponowanego roziązania,
* demonstracyjny, przedstawiający mnożenie oraz zawartość cienia przykładowego zestawu macierzy.

## Sposób rozwiązania

Cień macierzy opisano za pomocą list, w których znajdują się
indeksy kolumn jej niezerowych elementów. Jedna lista odpowiada jednemu wierszowi macierzy, tzn.
lista 0 zawiera kolumny niezerowych elementów z wiersza 0 macierzy.

Jako strukturę danych wykorzystano listę jednokierunkową - każdy wiersz macierzy
posiada odpowiadającą mu listę jednokierunkową.

**Przykład przedstawienia w tej konwencji:**
```
A = [1, 2, 3.
     0, 0, 1,
     0, 4, 0
     0, 0, 0]
     
shdA:

row0: 0, 1, 2
row1: 2
row2: 1    
row3:  
```

### Algorytm mnożenia z wykorzystaniem cienia

TODO

W tym podejściu w zasadzie wykonuje się dokładnie te same mnożenia, które wynikają ze wzoru.
Różnica polega na tym, że wykonywane są jedynie te niezbędne, czyli te, które nie dają w wyniku 0.
Pozwala to znacząco zminimalizować liczbę mnożeń.

### Algorytm mnożenia bez cienia

W tym przypadku zaimplementowano standardowe mnożenie macierzy na podstawie wzoru.

## Kompilacja

Uwaga, wymagana `cmocka`, dla Ubuntu/Debiana można znależć jako pakiet `libcmocka-dev`

```
cmake -B ./build
cmake --build ./build
```

## Uruchomienie
### Testy

```
./bin/shadow_matrix_test
```
### Program demo
```
./bin/shadow_matrix
```
## Testy 

W celu przetestowania zaproponowanego rozwiązania przygotowano następujące testy:
* **check_coverage_and_shd** - test wyliczania pokrycia macierzy oraz zawartości wyznaczonego cienia.
* **check_multiplication_shd** - test mnożenia z wykorzystaniem cieni macierzy. Przeprowadza mnożenie
  (jeśli się da) dla danego zestawu danych.
* **check_multiplication_naive** - test standardowego mnożenia, ze wzoru, tak zwanego "naiwnego". Przeprowadza mnożenie
  (jeśli się da) dla danego zestawu danych.
* **check_multiplication_consiscency** - sprawdza czy wynik mnożenia naiwnego pokrywa się z wynikiem mnożenia z cieniem.

Każdy z testów wykonano dla 6 różnych zestawów danych. Dane testowe znajdują się w pliku nagłówkowym `./test/inc/test_data.h`.