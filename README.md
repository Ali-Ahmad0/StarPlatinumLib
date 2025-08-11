# Star Platinum Engine | ECS Based 2D C++ Game Engine

- Still a work in progress so no planned release yet.
- You should be able to run it if you open the project using Visual Studio.
- All required libraries, dlls and a demo project is included in the repo.
- Note: Set startup project in Visual Studio to **Demo** if it's set to **Engine**.

## Demo project main.cpp file explanation:

```cpp
#include "main/Engine.hpp"

#include "test/scenes/topdown_test/TDScene.hpp"
#include "test/scenes/stress_test/StressTest.hpp"
#include "test/scenes/physics_test/PhysicsTest.hpp"

#undef main

int main() 
{
    StarPlatinumEngine game = StarPlatinumEngine();

    SceneManager::AddScene<TDScene>("topdown_test");
    SceneManager::AddScene<StressTest>("stress_test");
    SceneManager::AddScene<PhysicsTest>("physics_test");

    SceneManager::ChangeScene("topdown_test");

    game.Run();

    return 0;
}
```

## Example Scenes

There are 3 example scenes available:

- **topdown_test** (default) - A scene with a topdown character and a tilemap with collisions
- **stress_test** - Press Enter to spawn 1000 entities
- **physics_test** - Left click on the screen to spawn physics objects

Set your current scene in the `SceneManager::ChangeScene()` function.
