# Camel Game Engine

Camel is a game engine that I'm building to learn SDL2 and OpenGL. While it's still in the early stages of development, the aim is to slowly and steadily expand its capabilities.

## Why "Camel"?

The name "Camel" isn't just a random choice. Camels are renowned for their resilience and ability to endure challenging conditions. In the vast and demanding deserts, they can travel long distances without the need for frequent water stops. This characteristic reflects our vision for the engine - robust, efficient, and capable of handling the rigors of game development.

Moreover, camels have been beasts of burden for centuries, carrying heavy loads across difficult terrains. This symbolizes the engine's capability to handle intensive tasks and provide a sturdy foundation for game developers.

Lastly, "Camel" is a simple, catchy name. It's easy to remember and resonates with the robust characteristics we want our engine to embody. It's our hope that just as camels have left their mark in history, our Camel Game Engine will leave its footprint in the world of game development.

## Installation

### Requirements
- C++20
- Visual Studio 2022

### Setting Up Dependencies
The project relies on various libraries which can be easily installed using vcpkg. If you haven't installed vcpkg yet, you can follow the instructions [here](https://github.com/microsoft/vcpkg#getting-started).

After setting up vcpkg, you can install the required dependencies with the following commands:

```
vcpkg install sdl2 sdl2-image sdl2-ttf sdl2-mixer sdl2-net glew opengl glm entt --triplet x64-windows
```

Ensure that you have integrated `vcpkg` with Visual Studio using:

```
vcpkg integrate install
```

This makes sure that Visual Studio is aware of the libraries installed by vcpkg.

### Building & Running

1. Open the project in Visual Studio 2022.
2. Make sure to set your configuration to x64.
3. If you are running in Debug mode, ensure you add 'CAMEL_DEBUG_MODE' to the C/C++ Preprocessor Definition by right-clicking your project (Camel) > Properties > Configuration Properties > C/C++ > Preprocessor.
4. Build and run the solution.

## Contribution & Feedback

While this project is primarily for my learning, any feedback or contributions are always welcome. If you find any bugs or have any feature suggestions, please open an issue.

## License

This project is open source and available under the [MIT License](LICENSE.md).

---

Happy coding! Remember, every journey starts with a single step. Camel may be in its early days, but the possibilities are endless!