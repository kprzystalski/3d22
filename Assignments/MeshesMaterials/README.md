# Meshes and Materials

Termin oddania: 15.01.22 23:59 12pkt

W tym ćwiczeniu "uporządkujemy" trochę kod wprowadzając klasę `Mesh` i prosty system materiałów. Cwiczenie wymaga
ściągnięcia najnowszej wersji kodu z mojego repozytorium. Może się przy tym okazać, że Państwa kod przestanie działać :(
Proszę o zgłaszanie mi wszelkich problemów.

## Meshes

W katalogu `src/Engine` powinny się pojawić pliki `Mesh.h` i `Mesh.cpp` w których zdefiniowana jest klasa `Mesh`. Jeśli
wszystko poszło dobrze to pliki w tej kartotece powinny zostać skompilowane do biblioteki `
engine`. Bibliotekę można podłączyć w pliku `CMakeLists.txt` poleceniem `
target_link_libraries`. Jeśli stworzyli Państwo nowy katalog `
MeshesMaterials` i przekopiowali Państwo do niego zadanie `CameraMovement` to w pliku `
CMakeLists.txt` w tym katalogu należy dodać po poleceniu `add_executable` polecenie

```cmake
target_link_libraries(${PROJECT_NAME} PUBLIC engine)
```

Klasa `Mesh` odpowiada jednemu `Vertex Array Object` (VAO)  zawierającemu jeden bufor wierzchołów i jeden bufor
indeksów. Każdy obiekt tej klasy __musi__ posiadać jeden lub więcej obiektów klasy `SubMesh`. Klasa `SubMesh` odpowiada
jednemu poleceniu `gLDrawElements` i zawiera początek i koniec zakresu indeksów przekazywanych do tego polecenia.
Podsiatki dodajemy za pomocą metody  `add_submesh`.

Żeby skorzystać z instancji klasy `Mesh` musimy zapewnić dostęp do nich zarówno w metodzie `init` klasy `SimpleShapeApplication` jak i w metodzie `frame`.  W tym celu w tej klasie (`SimpleShapeApplication) proszę dodać pole 
```c++
std::vector<Mesh*> meshes_; 
```
i odpowiednią metodę dodającą siatki
```c++
void add_submesh(Mesh *mesh) {
    meshes_.push_back(mesh);
}
```

Nastwpnie w wmetodzie `init` musimy stworzyć siatkę odpowiadającą piramidce. Najpierw tworzymy nową siatkę poleceniem
```c++
auto pyramid = new Mesh; 
```
Potem wykorzystując metody tej klasy alokujemy pamieć dla bufora wierzchołków (`allocate_vertex_buffer`), załadować   dane (`load_vertices`) i ustalić sposób odczytu tych danych (`vertex_attrib_pointer`). I podobnie dla bufora indeksów. 

Na koniec w metodzie `frame` musimy dodać wywołanie metody `draw` na każdej siatce z tablicy (wektora) `messhes_`
```c++
 for (auto m: meshes_)
        m->draw();
```


## Materials

Nastepnym krokiem jest dodanie materiałów. W tym celu w katalogu `src/Engine` znajdują się dwa pliki `Material.h`
i `Material.cpp` w których zaimplementowana jest bazowa klasa `Material` i klasa `ColorMaterial`. Dodane zostały również
odpowiednie kody szaderów.

Klasa `Material` jest klasą bazową dla wszystkich materiałów. Posiada abstrakcyjną wirtualną metodę `bind()` którą
należy wywołać przed rysowaniem podsiatki która ma być pokryta danym materiałem. Ta metoda musi być zaimplementowana w
podklasach klasy `Material`. Celem metody `bind()` jest załadowanie odpowiedniego dla danego materiału programu, a
następnie przesłanie do niego koniecznych danych.

Klasa `ColorMaterial` implementuje chyba najprostszy możliwy materiał polegający na kolorowaniu podsiatki jednym
kolorem. Kody szaderów są praktycznie identyczne z tymi które Państwo napisali na potrzeby zadania `Uniforms`. Różnica
jest taka, że zamiast modyfikować kolor, dane z bufora uniform są wykorzystywane do przypisania koloru danemu pikselowi.
Metoda `init()` z klasy `ColorMaterial` kompiluje te shadery i tworzy program który jest zapisany w zmiennej `shader_`.
Zarówno metoda `init` jak i zmienna `shader_` są statyczne. Oznacza to, że są związane z klasą, a nie obiektem. Jest tak
ponieważ chcemy, aby wszystkie obiekty typu `ColorMaterial` miały ten sam program. Podobnie chcemy, aby wszystkie
obiekty tej klasy dzieliły ten sam bufor uniform służący do przekazywania koloru do szadera. Ten bufor jest również
tworzony w metodzie `init()`. Ta metoda musi być więc wywołana raz(!) zanim użyjemy obiektów `ColorMaterial` po raz
pierwszy. Ponieważ metoda ta nie jest związana z żadnym obiektem wywołujemy ją poleceniem

```c++
xe::ColorMaterial::init();
```

które najlepiej dodać na początku metody  `init()` klasy `SimpleShapeApplication`.

Klasa `ColorMaterial` posiada (niestatyczne) pole

```c++
glm::vec4 color_; 
```

w którym każdy obiekt tej klasy będzie przechowywał kolor danego materiału. Pole to ustawiane jest w konstruktorze
klasy.

Metoda `bind()` zaimplementowana w tej klasie ładuje program, podpina bufor uniform i przesyła do niego kolor.

Żeby zacząć korzystać z materiałów należy trochę zmodyfikować klasę `Mesh`. Po pierwsze będzie potrzebna tablica

```c++
std::vector<Material*> materialas_;
```

przechowująca wskażniki `Material*` odpowiadające kolejnym podsiatkom w tablicy `submeshes_`. Po dodaniu tablicy należy
zmodyfikować odpowiednio metodę `add_submesh` tak, żeby móc podać do niej wskaźnik na materiał dla danej podsiatki. W
przypadku nie podania materiału powinienm być ustawiany wskaźnik `nullptr`.

```c++
void add_submesh(Gluint start, GLuint end, Material* mat) } {...}
void add_submesh(Gluint start, GLuint end) } {add_submesh(start, end, nullptr);}
```

Teraz w metodzie `draw` należy przed wydaniem polecenia `glDrawElements` wykonać polecenie `bind` na wskaźniku do
materiału danej podsiatki. Oczywiście należy najpierw sprawdzić czy dany wskaźnik nie jest równy `nullptr`. Tak samo po
wydaniu tego polecenia należy wywołać metodę `unbind`.

Teraz proszę wykorzystać materiały do pokolorowania piramidy w ten sam sposób w jaki już była pokolorowana. W tym celu
należy podzielić piramidkę na pięć podsiatek i do każdej dodać odpowiedni materiał. Jak to się uda to proszę usunąć
informację o kolorze z bufora wierzchołków.   