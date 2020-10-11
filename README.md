# Base de Datos 2: Proyecto 1

Integrantes:
* Mosquera, Raúl
* Nicho, Jorge
* Vilchez, Osman

## Introducción
La emergencia sanitaria en la que nos encontramos ha traído varios desafíos en múltiples ámbitos, siendo la tecnología una parte crucial en la solución de estos. En consecuencia, uno de los ámbitos que han incrementado exponencialmente su impacto y su promoción en la sociedad, considerado como una "alternativa segura" de diversión, son los videojuegos. Tanto es así, que gran cantidad de jugadores casuales y targets imprevistos se han unido a esta forma de entretenimiento. Prueba de ello, es que a mediados del presente año 2020, 2 juegos casuales "FallGuys" y "AmongUs" (desarrollado el 2016 para plataformas móviles) han dado mantenimiento a sus plataformas por fallas en los servidores por la alta concurrencia de jugadores no contemplada. Esto ha promovido que plataformas de distribución de juegos como STEAM presenten catálogos extendidos y requieran cuantificar a los jugadores por juego para exigir la robustez de los servidores tanto propios como de los juegos de su catálogo.

El objetivo de este proyecto es realizar un gestor de base de datos simple con interfaz gráfica de usuario, limitado a las operaciones de búsqueda, inserción y eliminación, usando dos técnicas de organización de archivos. Además, se va a simular un planificador de transacciones concurrentes con las operaciones de lectura y escritura.

El dominio de datos esta basado en la plataforma digital de videojuegos, STEAM. Los campos son: Código, Nombre, Descripción, Precio y valoración.

Los resultados esperados para este proyecto es poder realizar satisfactoriamente un gestor de base de datos eficiente con una interfaz amigable para el usuario. Este será puesto a prueba para medir su desempeño en las diferentes operaciones, y posteriormente ser analizado.

## Técnicas utilizadas

### Sequential FIle
Lo definimos como un archivo que mantiene los registros ordenados fisicamente en base al valor de alguno de sus campos. Este nos permite tener búsquedas eficientes. Sin embargo, son dificiles de mantener y generan un costo extra a la hora de reorganizar el archivo.
  * Inserción
  Localizamos la posición en donde será insertado el nuevo registro. Si el espacio esta libre, lo insertamos. De lo contrario, insertamos un registro en un espacio auxiliar y los actualizamos los punteros.
  * Eliminación
  Utilizamos los punteros para saltar las tuplas eliminadas hasta llegar al registro que queremos eliminar.
  * Búsqueda
  Usamos un algoritmo de busqueda binaria para llegar al registro que queremos buscar.
  
### Extendible Hashing
Lo definimos como un hash dinámico para gestionar grandes base de datos y que reducen su tamaño en el tiempo (transaccionales). Estos indexan los registros en una tabla de direcciones de buckets usando un prefijo/sufijo.
  * Inserción
  Lozalizamos el bucket usando la secuencia D-bit. Si no encuentra el bucket, procede a buscar uno con la con la profundidad local mínima. Si no lo encuentra, procede a crear el bucket, y si lo encuentra y no esta lleno, procedemos a insertar. Por el contrario, si el bucket se encuentra lleno, lo dividimos y reinsertamos todos los registros. Entonces se crean nuevo buckets con una nueva profundidad local y el índice es modificado.
  En caso no se pueda incrementar la profundidad, ocurre un desbordamiento.
  * Eliminación
  Localizar el bucket respectivo mediante el índice y remover el registro. Si el bucket queda vacío, puede sr liberado, lo que implica actualizar el índice. Por otro lado, si dos buckets quedan con pocos elementos y tienen el mismo prefijo en la profundida local anterior, procedemos a mezclarlos, lo que implica actualizar el índice nuevamente.
  * Búsqueda
  Hacemos coincidir la secuencia D-bit con una entrada del directorio y nos dirigimos al bucket correspondiente para encontrar el registro.
### Transacciones
Lo definimos como un conjunto de operaciones de acceso a base de datos que conforman una unidad lógica de trabajo.
## Resultados experimentales

### Comparación entre técnicas de indexación
  * Inserción
  * Búsqueda

### Análisis

## Pruebas de uso

## Video


