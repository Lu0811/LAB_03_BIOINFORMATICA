# Programa de Alineamiento Local usando el Algoritmo Smith-Waterman en C++

Este programa implementa el algoritmo Smith-Waterman para encontrar la mejor subsecuencia común local entre dos cadenas de texto o secuencias biológicas (como ADN o proteínas). El algoritmo construye una matriz de puntajes para evaluar coincidencias, errores (mismatch) y espacios (gaps), y luego realiza un retroceso para recuperar la subsecuencia que maximiza el puntaje.

---

## Descripción detallada del código

### 1. Estructura `Resultado`

Esta estructura almacena toda la información relevante que el programa obtiene después de realizar el alineamiento:

* **maxScore**: Es el puntaje máximo encontrado en la matriz, que representa la mejor coincidencia parcial entre las dos secuencias.
* **subsecuenciaPrincipal**: La subsecuencia local común entre las dos cadenas que tiene el puntaje máximo.
* **pos1** y **pos2**: Son tuplas que indican las posiciones inicial y final de la subsecuencia dentro de cada secuencia original.
* **matriz**: La matriz de puntajes utilizada para calcular el alineamiento.
* **otrasSubsecuencias**: Lista que almacena otras subsecuencias locales que obtuvieron el mismo puntaje máximo.

### 2. Función `smithWaterman`

Esta función implementa el núcleo del algoritmo Smith-Waterman:

* Recibe dos secuencias de caracteres (`s1` y `s2`), y los valores numéricos para:

  * **match** (puntaje para coincidencia),
  * **mismatch** (penalización para desaciertos),
  * **gap** (penalización para espacios o huecos).

* Inicializa una matriz `H` de tamaño `(m+1) x (n+1)` donde `m` y `n` son las longitudes de las secuencias. La matriz se llena siguiendo la fórmula:

  Para cada posición `(i, j)`:

  ```
  H[i][j] = máximo entre:
    0,
    H[i-1][j-1] + match si los caracteres coinciden o mismatch si no,
    H[i-1][j] + gap,
    H[i][j-1] + gap
  ```

* El valor 0 garantiza que el alineamiento local no continúe con valores negativos.

* Mientras llena la matriz, el programa mantiene un registro de los valores máximos encontrados y sus posiciones, ya que puede haber más de una posición con el puntaje máximo.

#### Código clave de llenado de matriz:

```cpp
for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
        int diag = H[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch);
        int up = H[i - 1][j] + gap;
        int left = H[i][j - 1] + gap;
        H[i][j] = max({0, diag, up, left});
        if (H[i][j] > maxScore) {
            maxScore = H[i][j];
            coordsMax.clear();
            coordsMax.emplace_back(i, j);
        } else if (H[i][j] == maxScore) {
            coordsMax.emplace_back(i, j);
        }
    }
}
```

### 3. Retroceso (Backtracking)

Después de llenar la matriz y encontrar el puntaje máximo, el algoritmo debe reconstruir la subsecuencia local que dio lugar a ese puntaje. Esto se hace comenzando desde la posición del puntaje máximo y retrocediendo mientras los valores sean mayores que cero, escogiendo la dirección correcta según el puntaje que originó el valor actual.

La función `retroceso`:

* Construye la subsecuencia desde el final hacia el inicio.
* Elige si el paso anterior fue diagonal (coincidencia o mismatch), arriba (gap en la segunda secuencia) o izquierda (gap en la primera secuencia).
* Termina cuando encuentra un valor cero en la matriz, que indica el límite del alineamiento local.

#### Código clave para retroceso:

```cpp
while (i > 0 && j > 0 && H[i][j] > 0) {
    if (H[i][j] == H[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch)) {
        subseq = s1[i - 1] + subseq;
        --i; --j;
    } else if (H[i][j] == H[i - 1][j] + gap) {
        --i;
    } else {
        --j;
    }
}
```

### 4. Resultados y guardado

Una vez encontrado el alineamiento local principal y otras subsecuencias con puntajes máximos, el programa:

* Guarda en un archivo de texto:

  * El puntaje máximo.
  * La subsecuencia común principal.
  * Las posiciones de la subsecuencia en ambas secuencias originales.
  * La matriz completa de puntajes.
  * Otras subsecuencias que tienen el mismo puntaje máximo.

* Muestra mensajes de confirmación.

---

## Cómo compilar y ejecutar el programa

### Compilación

Abre una terminal o consola y escribe:

```
g++ -o smith_waterman smith_waterman.cpp
```

Esto compilará el archivo `smith_waterman.cpp` y generará un ejecutable llamado `smith_waterman`.

### Ejecución

Para correr el programa, simplemente ejecuta:

```
./smith_waterman
```

Luego ingresa:

* La primera secuencia (por ejemplo, una cadena de ADN: `AGCTG`)
* La segunda secuencia (por ejemplo: `GCTGA`)
* El puntaje para coincidencia (por ejemplo, `2`)
* La penalización por mismatch (por ejemplo, `-1`)
* La penalización por gap (por ejemplo, `-2`)

Después de procesar, el programa guardará el resultado en un archivo llamado `resultado_alineamiento.txt` y mostrará un mensaje indicando que la operación fue exitosa.


