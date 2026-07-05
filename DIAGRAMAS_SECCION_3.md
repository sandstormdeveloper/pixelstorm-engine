# Diagramas PlantUML para la seccion 3

Version simplificada para que los diagramas se vean bien en la memoria.
Cada bloque solo incluye lo importante y mantiene una lectura limpia.

## 3.1 Vision general de la arquitectura

```plantuml
@startuml
skinparam componentStyle rectangle
skinparam shadowing false
skinparam linetype ortho
left to right direction

package "PixelStorm" {
  [Application] as App

  package "core" {
    [Window] as Window
    [Time] as Time
    [Log] as Log
  }

  package "ecs" {
    [Entity] as Entity
    [Registry] as Registry
    [World] as World
  }

  package "renderer" {
    [Camera2D] as Camera
    [Renderer] as Renderer
    [Shader] as Shader
    [Texture] as Texture
  }

  package "resources" {
    [ResourceManager] as Resources
  }

  package "input" {
    [Input] as Input
  }

  package "scene" {
    [SceneManager] as SceneMgr
    [Scene] as Scene
  }

  package "systems" {
    [PhysicsSystem] as Phys
    [RenderSystem] as RenderSys
    [AnimationSystem] as Anim
    [ParticleSystem] as Part
  }
}

App --> Window
App --> Time
App --> Log
App --> SceneMgr
App --> Resources
App --> Phys
App --> RenderSys
App --> Anim
App --> Part

Input --> Window
SceneMgr --> World
SceneMgr --> Phys
World --> Registry
Registry --> Entity
RenderSys --> Renderer
RenderSys --> Shader
RenderSys --> Camera
RenderSys --> Resources
RenderSys --> Registry
Phys --> Registry
Resources --> Texture
SceneMgr --> Scene

@enduml
```

## 3.2 Decisiones de diseno

```plantuml
@startuml
skinparam componentStyle rectangle
skinparam shadowing false

[Infraestructura] as Infra
[Contenido / demo] as Demo
[API publica] as API

API --> Infra
Demo --> Infra

note right of Infra
  Motor separado de la demo.
  Modulos claros y reutilizables.
end note

@enduml
```

## 3.3 Core del motor

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false
skinparam linetype ortho

class Application {
  +Run()
  +OnUpdate(callback)
  +GetWorld()
  +GetScenes()
}

class Window
class Time
class Log
class SceneManager
class Input
class Renderer
class Camera2D
class Texture
class ResourceManager
class PhysicsSystem
class RenderSystem
class AnimationSystem
class ParticleSystem
class World
class Registry
class Entity
class Shader

Application *-- Window
Application *-- Time
Application *-- Log
Application *-- SceneManager
Application *-- ResourceManager
Application *-- PhysicsSystem
Application *-- RenderSystem
Application *-- AnimationSystem
Application *-- ParticleSystem
Application *-- World
Application *-- Registry
Application ..> Input

Input --> Window
SceneManager --> World
SceneManager --> PhysicsSystem
RenderSystem --> Renderer
RenderSystem --> Shader
RenderSystem --> Camera2D
RenderSystem --> ResourceManager
RenderSystem --> Registry
PhysicsSystem --> Registry
PhysicsSystem --> Time
World --> Registry
Registry --> Entity
ResourceManager --> Texture

@enduml
```

## 3.4 Pipeline del motor

```plantuml
@startuml
skinparam shadowing false
start

while (¿La ventana sigue abierta?) is (si)
  :Actualizar Time;

  if (¿Hay callback de juego?) then (si)
    :Ejecutar OnUpdate(deltaTime);
  else (no)
    :Omitir logica externa;
  endif

  if (¿Hay escena activa?) then (si)
    :SceneManager.Update(deltaTime);
  else (no)
    :Esperar nueva escena;
  endif

  if (¿Hay sistema fisico activo?) then (si)
    :PhysicsSystem.Update(deltaTime);
  endif

  if (¿Hay sistemas visuales activos?) then (si)
    :AnimationSystem.Update(deltaTime);
    :ParticleSystem.Update(deltaTime);
  endif

  :RenderSystem.Render();
  :Window.Update();
  :Input.Update();
endwhile (no)

stop

@enduml
```

## 3.5 ECS y entidades

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

class Entity
class Registry
class World
class Transform
class SpriteRenderer
class Collider
class Rigidbody

Entity --> Registry
World --> Registry
Registry o-- Transform
Registry o-- SpriteRenderer
Registry o-- Collider
Registry o-- Rigidbody

@enduml
```

## 3.6 Sistema de renderizado

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

class RenderSystem
class Renderer
class Shader
class Texture
class Camera2D
class ResourceManager
class Transform
class SpriteRenderer

RenderSystem --> Renderer
RenderSystem --> Shader
RenderSystem --> ResourceManager
RenderSystem --> Camera2D
RenderSystem --> Transform
RenderSystem --> SpriteRenderer
ResourceManager o-- Texture

@enduml
```

## 3.7 Gestion de recursos

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

class ResourceManager
class Texture
class Application

Application --> ResourceManager
ResourceManager o-- Texture

@enduml
```

## 3.8 Sistema de input

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

class Input
class Window

Input --> Window

note right of Input
  Acciones y ejes
  sobre teclado y raton.
end note

@enduml
```

## 3.9 Sistema de escenas

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

abstract class Scene
class SceneManager
class GameScene
class SecondScene
class World
class PhysicsSystem

Scene <|-- GameScene
Scene <|-- SecondScene
SceneManager o-- Scene
Scene --> World
Scene --> PhysicsSystem
SceneManager --> World

@enduml
```

## 3.10 Fisica y colisiones

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

class PhysicsSystem
class Physics
class Collision
class Transform
class Collider
class Rigidbody

PhysicsSystem --> Physics
PhysicsSystem --> Collision
Physics --> Transform
Physics --> Collider
Physics --> Rigidbody
Collision --> Transform
Collision --> Collider

@enduml
```

## 3.11 Particulas y animacion

```plantuml
@startuml
skinparam classAttributeIconSize 0
skinparam shadowing false

class AnimationSystem
class ParticleSystem
class Animator
class AnimationClip
class ParticleEmitter
class Particle
class SpriteRenderer
class Transform

Animator *-- AnimationClip
AnimationSystem --> Animator
AnimationSystem --> SpriteRenderer
ParticleSystem --> ParticleEmitter
ParticleSystem --> Particle
ParticleSystem --> Transform
ParticleSystem --> SpriteRenderer

@enduml
```

## 3.12 Integracion general

```plantuml
@startuml
skinparam componentStyle rectangle
skinparam shadowing false
skinparam linetype ortho

package "Application" {
  [Application] as App
  [Window] as Win
  [Time] as Time
  [Input] as Input
  [ResourceManager] as Res
}

package "Simulation" {
  [SceneManager] as SceneMgr
  [PhysicsSystem] as Phys
  [AnimationSystem] as Anim
  [ParticleSystem] as Part
}

package "Presentation" {
  [RenderSystem] as Render
  [Renderer] as Renderer
  [Camera2D] as Camera
}

package "Gameplay" {
  [GameScene] as Game
  [SecondScene] as Second
}

App --> Win
App --> Time
App --> Input
App --> Res
App --> SceneMgr
App --> Phys
App --> Anim
App --> Part
App --> Render

Input --> Win
SceneMgr --> Phys
SceneMgr --> Game
SceneMgr --> Second
Phys --> Anim
Anim --> Part
Render --> Renderer
Render --> Camera
Render --> Res

Game --> SceneMgr
Second --> SceneMgr

@enduml
```
