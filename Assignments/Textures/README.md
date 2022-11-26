# Textures

Termin oddania: 07.01.22 23:59 10kt

W tym zadaniu rozszerzymy klasę `ColorMaterial`, tak aby kolor mógł być pobierany z tekstury. W tym celu musimny dodać
do niej dodatkowe pola:

```c++
static GLint uniform_map_Kd_location_;
GLuint texture_;
GLuint texture_unit_;
```

Pole statyczne `uniform_map_Kd_location_` będzie zawierać "adres" zmniennej uniform w shaderze fragmentów
odpowiedzialnej za teksturowanie. Jest to pole `static` bo chcemy, żeby było wspolne dla wszystkich obiektów tej klasy.
Pola typu `static` wymagają dodatkowej definicji. W pliku `Material.cpp`należy więc dodać wewnątrz przestrzeni nazw `xe`
linijkę:

```c++
GLint  ColorMaterial::uniform_map_Kd_location_ = 0;
```

To pole będzie inicjalizowane w funkcji `init` klasy `ColorMaterial`, za pomocą polecenia `glGetUniformLocation`.

Pola `texture_` i `texture_unit_` będą zawierać "uchwyt" do tekstury oraz numer jednostki teksturującej. Należy dodać
odpowienie "getery" i "setery" czyli metody nadające im i zwracające ich wartość, a w konstruktorze klasy nadać obu
wartość zero.

W metodzie `bind` sprawdzamy czy pole `texture_` ma wartość większą od zera. Jeśli tak, to poleceniem `glUniform1i`
przesyłamy pod adres wskazany polem `uniform_map_Kd_location_` wartość z pola `texture_unit_`. Następnie ustawiamy tą
jednostkę jako aktywną poleceniem

```c++
glActiveTexture(GL_TEXTURE0 + texture_unit_); 
```

Potem podpinamy teksturę z pola `texture_` poleceniem `glBindTexture`.

Ponieważ chcemy, aby klasa `ColorMaterial` obsługiwała zarówno tekstury jak i stałe kolory, to musimy przesłać do
szadera informację o tym, czy używamy tekstury czy nie. W tym celu w metodzie `bind` deklarujemy zmienna `use_map_Kd` i
nadajemy jej wartość zero jeśli nie używamy tekstury i jeden jeśli używamy. Potem wyślemy tą zmienną do bufora
uniform `Color`. Tak więc dodajemy kolejne pole do bloku uniform w szaderze fragmentów:

```glsl
layout(std140, binding=0) uniform Color {
    vec4  Kd;
    bool use_map_Kd;
};
```

W kodzie metody `bind` dodajemy odpowienie polecenie przesyłające zmienną `use_map_Kd` do bufora uniform.

Teraz musimy dodać współrzędne tekstury do bufora wierzchołków. Po poprzednim ćwiczeniu powinni mieć państwo w nim już
tylko współrzędne wierzchołków. Dodanie dodatkowego atrybutu czyli współrzędnych tekstury będzie przebiegać tak samo jak
w ćwiczeniu `Colors`, tyle że współrzedne tekstury mają tylko dwie składowe. Wartości tych współrzędnych można odczytać
z uv mapy w pliku `uv.png`.

Koniec końców powinniśmy mieć w szaderze fragmentów zmienną

```glsl
in vec2 vertex_texcoords; 
```

Zawierającą współrzędne tekstury dla danego piksela. Teraz będziemy mieli tylko jedną podsiatkę ponieważ tesktura
pokryje wszystkie ścianki.

Klasa `ColorMaterial` odpowiada za podpięcie tektstury, ale nie za jej stworzenie i wczytanie. To dokonamy w
metodzie `init` klasy `SimpleShapeApplication`. Najpierw proszę w głownym katalogu stworzyć pod katalog `Models` i
przekopiować do niego plik `multicolor.png`. Obrazek tekstury wczytamy za pomocą polecenia `stb_image`. W tym celu w
pliku `app.cpp` dodajemy

```c++
#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"
```

na samym końcu listy includów. Obrazek wczytujemy za pomocą poleceń

```c++
stbi_set_flip_vertically_on_load(true);
GLint width, height, channels;
auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png"
auto img = stbi_load(texture_file, &width, &height, &channels, 0);
if (!img) {
    spdlog::warn("Could not read image from file `{}'", texture_file);
}
```

Jeśli obrazek załaduje się, to w zmiennych `height`, `width` i `channels` zostaną zapisane odpowiednie wartości.
Zmnienna `channels` będzie zawierać liczbę kanałów w obrazku. Teraz trzeba stworzyć teksturę poleceniem  `glGenTeksture`
, zbindować ją, załadować do niej obrazek polecenien `glTexImage2D` i ustawić odpowiednie parametry. Jak to się uda, to
możemy tą teksturę przekazać do obiektu `ColorMaterial`.

I na konieć zmodyfikujemy szader fragmentów. Dodamy do niego zmienna

```glsl
uniform sampler2D map_Kd;
```

W funkcji `main` sprawdzamy czy zmienna `use_map_Kd` z bloku uniform `Color` ma wartość zero. Jeśli tak to
do `vFragColor` przypisujemy wartość `Kd`. Jeśli nie to przypisujemy
`Kd*texture(map_Kd, vertex_texcoords)`.

Musimy jeszcze tylko zaincjalizować pole  `uniform_map_Kd_location` klasy `ColorMaterial`. W tym celu należy dodać kod

```c++
uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
if (uniform_map_Kd_location_ == -1) {
    spdlog::warn("Cannot get uniform {} location", "map_Kd");
}
```

w metodzie `init` tej klasy. 





