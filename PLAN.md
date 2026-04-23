# PLAN DE TRABAJO

## 1. Punto de partida

### Lo que ya existe

- El proyecto ya compila con CMake y genera `pixelstorm_demo`.
- Hay una base funcional de ventana, bucle principal y carga de shaders:
  - `engine/include/pixelstorm/core/Application.h`
  - `engine/src/core/Application.cpp`
  - `engine/include/pixelstorm/core/Window.h`
  - `engine/src/core/Window.cpp`
  - `engine/include/pixelstorm/renderer/Shader.h`
  - `engine/src/renderer/Shader.cpp`
- La estructura de carpetas esta bien encaminada y ya separa `core`, `renderer`, `ecs`, `resources`, `physics`, `scene`, `systems` y `demo`.

### Lo que falta realmente

- La mayoria de sistemas definidos en `GOALS.md` estan solo como esqueleto.
- Todavia no hay un camino jugable completo de "crear entidad -> asignar componentes -> renderizar sprite -> moverlo con input -> detectar colision -> cambiar de escena".
- Hay muchas cabeceras y fuentes vacias, asi que el riesgo ahora no es la estructura, sino el orden de implementacion.

## 2. Objetivo del plan

Construir primero un MVP tecnico que cumpla los requisitos minimos de `GOALS.md`, dejando los extras para el final. La idea es evitar dispersarnos y avanzar por capas que se puedan probar en cada paso.

Ademas, la `demo` no debe quedarse en una simple prueba aislada: debe evolucionar poco a poco hacia un pequeno juego hecho sobre el motor. Por eso, a medida que las fases avancen, conviene mover la logica de uso real a `demo` y reservar `engine` para la infraestructura reutilizable.

## 3. Prioridades del proyecto

1. Tener una base estable del motor.
2. Conseguir un render 2D minimo util.
3. Introducir un ECS sencillo pero usable.
4. Conectar input, recursos y escenas.
5. Implementar fisicas/collisions basicas.
6. Completar particulas y shaders basicos.
7. Preparar una demo pequena que ensene todo junto.
8. Solo despues, abordar los editores opcionales.

Nota de enfoque:
- `engine` debe contener sistemas reutilizables del motor.
- `demo` debe actuar como primer juego construido con ese motor, y servir para validar si la API realmente resulta comoda desde fuera.

## 4. Hoja de ruta por fases

### Fase 1 - Base tecnica del motor

Objetivo: convertir el arranque actual en una base segura para crecer.

Estado actual: completada.

Tareas:
- Endurecer `Window`:
  - comprobar errores de `glfwInit`, `glfwCreateWindow` y `gladLoadGLLoader`
  - destruir la ventana correctamente antes de terminar GLFW
  - configurar viewport y callback de resize
- Limpiar `Application`:
  - evitar `new/delete` manuales si es posible
  - separar `Init`, `Run`, `Shutdown`
  - preparar `delta time`
- Implementar `core/Time` y `core/Log`
- Definir convenciones:
  - nombres
  - ownership de recursos
  - carpeta de assets y rutas

Entregable:
- El motor abre ventana, actualiza tiempo, registra logs y cierra sin fugas ni estados invalidos.

Resultado alcanzado:
- `Window` valida la inicializacion de GLFW/GLAD, destruye la ventana correctamente y actualiza el viewport al redimensionar.
- `Application` usa `std::unique_ptr` y separa `Init`, `Run` y `Shutdown`.
- `Time` centraliza `delta time` y tiempo acumulado del motor.
- `Log` centraliza mensajes `Info`, `Warning` y `Error`.
- Quedan definidas convenciones base de ownership, tiempo, logs y rutas de assets en `README.md`.

### Fase 2 - Render 2D minimo

Objetivo: poder dibujar algo util en pantalla, no solo limpiar el frame.

Estado actual: completada.

Tareas:
- Implementar `renderer/Renderer`
- Implementar `renderer/Texture`
- Implementar un primer `SpriteRenderer` component
- Crear un pipeline minimo para dibujar un quad/textura 2D
- Dejar listo un shader basico para sprites pixel art
- Anadir una camara 2D simple en `renderer/Camera2D`

Entregable:
- La demo dibuja al menos un sprite o quad con transformacion 2D.

Resultado alcanzado:
- `Renderer` dibuja quads a partir de una matriz de modelo, evitando duplicar la logica de transformacion.
- `Transform` y `SpriteRenderer` ya existen como componentes base reutilizables.
- El shader por defecto soporta tinte RGBA para sprites.
- `Application` ya renderiza la demo a traves de `Transform` + `SpriteRenderer`, con camara 2D, shader y textura conectados.

### Fase 3 - ECS minimo viable

Objetivo: introducir el corazon del motor sin sobreingenieria.

Tareas:
- Implementar `Entity`, `Component`, `Registry` y `System`
- Empezar con pocos componentes reales:
  - `Transform`
  - `SpriteRenderer`
  - `Collider`
  - `Rigidbody`
- Crear un flujo simple:
  - crear entidad
  - anadir componentes
  - consultar entidades por combinacion de componentes

Entregable:
- La demo crea entidades mediante ECS y el render usa sus componentes.

Enfoque recomendado:
- exponer la API minima de entidades/componentes desde `engine`
- empezar a crear entidades visibles desde `demo`, para comprobar pronto como se siente el motor desde el lado juego

### Fase 4 - Recursos e input

Objetivo: que el motor empiece a ser comodo de usar.

Tareas:
- Implementar `resources/ResourceManager` y `AssetLoader`
- Centralizar carga de texturas y shaders
- Implementar `input/Input` y `systems/InputSystem`
- Exponer API sencilla:
  - teclado
  - raton si hace falta
  - acciones basicas

Entregable:
- Un sprite puede cargarse desde assets y moverse con input.

### Fase 5 - Escenas y flujo de juego

Objetivo: organizar el contenido del motor y de la demo.

Tareas:
- Implementar `scene/Scene` y `scene/SceneManager`
- Definir ciclo de vida:
  - `OnEnter`
  - `OnUpdate`
  - `OnRender`
  - `OnExit`
- Crear al menos 2 escenas de ejemplo en `demo/scenes`

Entregable:
- La demo puede cambiar de una escena a otra y mantener una estructura clara.

### Fase 6 - Fisicas y colisiones 2D

Objetivo: cubrir uno de los requisitos minimos mas importantes.

Tareas:
- Implementar `physics/Physics` y `physics/Collision`
- Implementar `systems/PhysicsSystem`
- Empezar con AABB y colisiones simples
- Anadir deteccion y respuesta minima:
  - overlap
  - resolucion basica
  - velocidad basica en `Rigidbody`

Entregable:
- Dos entidades con `Collider` pueden detectar colision y reaccionar.

### Fase 7 - Particulas, animacion y shaders basicos

Objetivo: completar el bloque visual minimo del TFG.

Tareas:
- Implementar `systems/ParticleSystem`
- Implementar `systems/AnimationSystem`
- Anadir soporte minimo de sprite sheet
- Preparar 2 o 3 shaders basicos:
  - sprite normal
  - tinte/color
  - opcion de efecto simple para debug o particulas

Entregable:
- La demo muestra una entidad animada y un efecto de particulas simple.

### Fase 8 - Integracion, demo y documentacion

Objetivo: cerrar el MVP y dejarlo presentable.

Tareas:
- Montar una demo vertical pequena:
  - personaje o sprite controlable
  - colision con escenario
  - cambio de escena
  - particulas al interactuar
- Anadir README tecnico del motor
- Documentar arquitectura minima
- Crear checklist de requisitos de `GOALS.md`

Entregable:
- Demo reproducible y alineada con los requisitos minimos.

### Fase 9 - Requisitos opcionales

Objetivo: abordar extras solo si el MVP ya esta estable.

Tareas:
- Editor de particulas con ImGui
- Editor de niveles muy basico

Entregable:
- Extras funcionales sin comprometer la estabilidad del motor base.

## 5. Siguientes pasos inmediatos

Este seria el orden que yo seguiria ya mismo:

1. Construir un ECS pequeno pero funcional con `Transform` + `SpriteRenderer`.
2. Exponer una API minima del motor para crear entidades y anadir componentes desde `demo`.
3. Hacer que `demo` cree al menos una entidad visible usando esa API.
4. Conectar `ResourceManager` e `Input`.
5. Evolucionar la `demo` hacia una primera escena jugable con un sprite movible.

## 6. Reparto recomendado por bloques

Si quieres avanzar sin perder foco, trabaja asi:

- Bloque A: `core` + robustez del arranque
- Bloque B: `renderer` + recursos graficos
- Bloque C: `ecs` + componentes base
- Bloque D: `input` + `scene`
- Bloque E: `physics` + `systems`
- Bloque F: demo final + documentacion

Cada bloque deberia cerrar con una prueba visible en la demo.

## 7. Criterio de "hecho"

Una fase no deberia darse por cerrada hasta que cumpla esto:

- compila en limpio
- la demo prueba la funcionalidad
- no introduce codigo vacio sin uso inmediato
- deja una API entendible desde `demo`
- el cambio queda documentado en README o notas tecnicas
- acerca la `demo` al juego final, en vez de acumular pruebas tecnicas desconectadas

## 8. Riesgos a evitar

- Intentar implementar todos los sistemas a la vez
- Hacer un ECS demasiado complejo para el alcance del TFG
- Posponer la demo hasta el final
- Mantener demasiados archivos vacios durante mucho tiempo
- Meter ImGui antes de tener el MVP base estable

## 9. Resultado esperado del MVP

Al cerrar el MVP, deberias tener:

- ventana y renderizado
- ECS funcional
- componentes base
- gestion de recursos
- input
- particulas
- shaders basicos
- fisicas y colisiones 2D
- sistema de escenas

Eso ya te deja muy bien posicionado para cumplir el TFG y, si queda tiempo, entrar a los editores opcionales.
