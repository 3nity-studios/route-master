<div align="center">
    
![logo](https://images.placeholders.dev/?width=416&height=110&fontFamily=sans-serif&fontWeight=1000&fontSize=42&text=TRINITY%20STUDIOS&bgColor=rgba(0,0,0,0.0)&textColor=rgba(100,90,255,1))
    
# Route Master

[![cpp-badge](https://img.shields.io/badge/C%2B%2B%2017-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![cmake-badge](https://img.shields.io/badge/CMake_3.30.2-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![sfml-badge](https://img.shields.io/badge/SFML_2.6.1-546D23?style=for-the-badge&logo=sfml&logoColor=white)](https://www.sfml-dev.org/)
[![designar-badge](https://img.shields.io/badge/DeSiGNAR_2.0.1-2A2A2A?style=for-the-badge)](https://github.com/3nity-studios/DeSiGNAR)
[![conventionalcommits-badge](https://img.shields.io/badge/Conventional_Commits_1.0.0-940C1E?style=for-the-badge&logo=conventionalcommits&logoColor=white)](https://www.conventionalcommits.org/en/v1.0.0/)

</div>

<details>
<summary><h2 style="display:inline">Table of Contents</h2></summary>

   * [Introduction](#introduction-)
   * [Building](#building-)
      * [Prerequisites](#prerequisites)
      * [Configurable options](#configurable-options)
      * [Build](#build)
      * [Install](#install)
   * [Hacking](#hacking-)
      * [Presets](#presets)
      * [Configure, build and test](#configure-build-and-test)
      * [Developer mode targets](#developer-mode-targets)
   * [Contributing](#contributing-)
   * [Acknowledgments](#acknowledgments-)
    
</details>

## Introduction [↑](#route-master)

Route Master is a simulation game where you manage a bus company, try to grow
your earnings by testing different routes, upgrading your fleet or taking
risks by signing contracts. Your goal is to become a tycoon in the urban
transport sector!

<details>

<summary><h3 style="display:inline">Screenshots</h3></summary>

<div align="center">

![title-screen-ss](https://images.placeholders.dev/?width=854&height=480&text=Title%20Screen&bgColor=%23f7f6f6&textColor=%236d6e71&fontSize=32)

Title Screen

![gameplay-screen-1-ss](https://images.placeholders.dev/?width=854&height=480&text=Gameplay%20Screen%201&bgColor=%23f7f6f6&textColor=%236d6e71&fontSize=32)

Gameplay Screen 1

</div>

</details>

## Building [↑](#route-master)

> [!WARNING]
> Neither building on other systems nor cross-compiling is supported for the
> moment, everything has been tested only under Debian 11 for the moment.

For a smooth experience, we recommend developing on a Debian-based distro
through a [DevContainer][4] compatible editor.
Otherwise, keep on reading to know how to match the needed development
environment and libraries.

### Prerequisites

* C++ `17` compatible compiler (gcc/clang `9+` recommended)
* cmake >= `3.30`

#### Libraries

The project will try to automatically [fetch and build][5] the following libraries for you
(see [setup section](#setup)).

Nonetheless, it is important that your system already fulfills the respective dependencies of
each library.

On Linux we recommend building and installing the libraries from their source code using CMake.
After all, it is our preferred way when developing.

On Windows use Visual Studio and feel free to install pre-built libraries if needed.

<ul>

<li>
<details>

<summary><b>SFML <code>2.6.1</code></b></summary>
<ul>
<li>
<details>

<summary><b>Linux installation steps <a href="https://www.sfml-dev.org/tutorials/2.6/start-linux.php" target="_blank">(guide source)</a></b></summary>

<ul>
<li>
<details>

<summary><b>Prerequisites</b></summary>

* freetype
* x11
* xrandr
* udev
* opengl
* flac
* ogg
* vorbis
* vorbisenc
* vorbisfile
* openal
* pthread

_Development headers required too_

</details>
</li>
<li>
<details>

<summary><b>Building and installing</b></summary>

1. Install required libraries (assuming `Debian 11`)
```shell
$ sudo apt update
$ sudo apt install \
    libfreetype-dev \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    libudev-dev \
    libgl-dev \
    libflac-dev \
    libvorbis-dev \
    libopenal-dev \
```

_Package names may differ on other distributions_

2. Download, build and install library
```shell
$ git clone --depth 1 --branch 2.6.x https://github.com/SFML/SFML.git
$ cd SFML
$ cmake -S . -B build
$ cmake --build build
$ sudo cmake --install build
```

</details>
</li>
</ul>
</details>
</li>
<li>
<details>

<summary><b>Windows installation steps <a href="https://www.sfml-dev.org/tutorials/2.6/start-vc.php" target="_blank">(guide source)</a></b></summary>
<ul>
<li>
<details>

<summary><b>Installing</b></summary>

1. Download [SFML SDK](https://www.sfml-dev.org/download/sfml/2.6.1/) corresponding to your Visual Studio version.

</details>
</li>
</ul>

</details>
</li>
</ul>
</details>
</li>

<li>
<details>

<summary><b>DeSiGNAR <code>2.0.1</code></b></summary>
<ul>
<li>
<details>

<summary><b>Linux installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Building</b></summary>

1. Download, build and install library (assuming up-to-date `cmake`)
```shell
$ git clone --depth 1 --branch v2.0.1 https://github.com/3nity-studios/DeSiGNAR.git
$ cd DeSiGNAR
$ cmake -S . -B build
$ cmake --build build
$ sudo cmake --install build
```

</details>
</li>
</ul>
</details>
</li>
<li>
<details>

<summary><b>Windows installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Prerequisites</b></summary>

no info

</details>
</li>
<li>
<details>

<summary><b>Building</b></summary>

not supported at the moment

</details>
</li>
</ul>

</details>
</li>
</ul>
</details>
</li>

<li>
<details>

<summary><b>nlohmann/json <code>3.11.3</code></b></summary>
<ul>
<li>
<details>

<summary><b>Linux installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Building</b></summary>

1. Download, build and install library (assuming up-to-date `cmake`)
```shell
$ git clone --depth 1 --branch v3.11.3 https://github.com/nlohmann/json.git
$ cd json
$ cmake -S . -B build -D JSON_BuildTests=OFF
$ cmake --build build
$ sudo cmake --install build
```

</details>
</li>
</ul>
</details>
</li>
<li>
<details>

<summary><b>Windows installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Prerequisites</b></summary>

no info

</details>
</li>
<li>
<details>

<summary><b>Building</b></summary>

no info

</details>
</li>
</ul>

</details>
</li>
</ul>
</details>
</li>

<li>
<details>

<summary><b>Catch2 <code>3.7.0</code></b></summary>
<ul>
<li>
<details>

<summary><b>Linux installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Building</b></summary>

1. Download, build and install library (assuming up-to-date `cmake`)
```shell
$ git clone --depth 1 --branch v3.7.0 https://github.com/catchorg/Catch2.git
$ cd Catch2
$ cmake -S . -B build
$ cmake --build build
$ sudo cmake --install build
```

</details>
</li>
</ul>
</details>
</li>
<li>
<details>

<summary><b>Windows installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Prerequisites</b></summary>

no info

</details>
</li>
<li>
<details>

<summary><b>Building</b></summary>

no info

</details>
</li>
</ul>

</details>
</li>
</ul>
</details>
</li>

<li>
<details>

<summary><b>tmxlite <code>1.4.4</code></b></summary>
<ul>
<li>
<details>

<summary><b>Linux installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Building</b></summary>

1. Download, build and install library (assuming up-to-date `cmake`)
```shell
$ git clone --depth 1 --branch v1.4.4 https://github.com/fallahn/tmxlite.git
$ cd tmxlite
$ cmake -S tmxlite -B build
$ cmake --build build
$ sudo cmake --install build
```

</details>
</li>
</ul>
</details>
</li>
<li>
<details>

<summary><b>Windows installation steps</b></summary>
<ul>
<li>
<details>

<summary><b>Prerequisites</b></summary>

no info

</details>
</li>
<li>
<details>

<summary><b>Building</b></summary>

no info

</details>
</li>
</ul>

</details>
</li>
</ul>
</details>
</li>
</ul>

### Configurable options

You can modify these in CMakeLists.txt (or in CMakeCache with ccmake/cmake-gui)

|             Option              |      Type      |                             Description                             |
|         :-------------:         | :------------- |                           :-------------                            |
| `RM_USE_SYSTEM_JSON`            | BOOL           | Turn ON to use system installation of nlohmann's JSON               |
| `RM_USE_SYSTEM_DESIGNAR`        | BOOL           | Turn ON to use system installation of DeSiGNAR                      |
| `RM_USE_SYSTEM_SFML`            | BOOL           | Turn ON to use system installation of SFML                          |
| `RM_USE_SYSTEM_CATCH2`          | BOOL           | Turn ON to use system installation of Catch2                        |
| `RM_USE_SYSTEM_TMXLITE`         | BOOL           | Turn ON to use system installation of tmxlite                       |
| `RM_DEVELOPER_MODE`             | BOOL           | Turn ON to enable tests and developer targets of Route Master       |

### Build

This project doesn't require any special command-line flags to build to keep
things simple.

Here are the steps for building in release mode:

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

An executable should be generated somewhere in the build folder (presumably
in the build/bin folder)

To build tests, use the following commands:
```sh
cmake -S . -B build -DRM_DEVELOPER_MODE=TRUE -DBUILD_TESTING=TRUE
cmake --build build
```

### Install

This project doesn't require any special command-line flags to install to keep
things simple. As a prerequisite, the project has to be built with the above
commands already.

Here is the command for installing the release mode artifacts, although no
install rules have been defined for the moment, we plan on keeping things
rather portable (see --prefix CMake flag) before delving into installing shenanigans.

```sh
cmake --install build
```

That's it! From here on, there are some tips for developing.

## Hacking [↑](#route-master)

**(Optional)** These are some goodies that we use in the development of our project:
* clang-format
* clang-tidy
* cppcheck
* codespell

These aren't strictly necessary to build the project but they provide
some Quality-of-Life features for development worth trying.

```shell
sudo apt update
sudo apt install \
    clang-format \
    clang-tidy \
    cppcheck \
    python3-pip
python3 -m pip install codespell
```

Build system targets that are only useful for developers of this project are
hidden if the `RM_DEVELOPER_MODE` option is disabled. Enabling this
option makes tests and other developer targets and options available. Not
enabling this option means that you are a consumer of this project and thus you
have no need for these targets and options (or you just wanna build a production
release).

~~Developer mode is always set to on in CI workflows.~~ No workflows have been
set up for the time being. Plans on this are underway.

### Presets

This project makes use of [presets][1] to simplify the process of configuring
the project. As a developer, you are recommended to always have the [latest
CMake version][2] installed to make use of the latest Quality-of-Life
additions.

You should create a `CMakeUserPresets.json` file at the root of the project.

For example, here's the one we're currently using:

```json
{
  "version": 8,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 30,
    "patch": 2
  },
  "configurePresets": [
    {
      "name": "dev-common",
      "hidden": true,
      "inherits": ["dev-mode", "clang-tidy"],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "dev",
      "binaryDir": "${sourceDir}/build/dev",
      "inherits": ["dev-common", "ci-linux"],
      "cacheVariables": {
        "RM_USE_SYSTEM_JSON": "TRUE",
        "RM_USE_SYSTEM_DESIGNAR": "TRUE",
        "RM_USE_SYSTEM_SFML": "TRUE",
        "RM_USE_SYSTEM_TMXLITE": "TRUE",
        "RM_USE_SYSTEM_CATCH2": "TRUE"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "jobs": 2
    }
  ],
  "testPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "output": {
        "outputOnFailure": true
      },
      "execution": {
        "jobs": 2,
        "noTestsAction": "error"
      }
    }
  ]
}
```

We currently have no plans for building on platforms
other than Linux. There might be intents of cross-compiling in a future but
that's just not a priority right now.

`CMakeUserPresets.json` is also the perfect place in which you can put all
sorts of things that you would otherwise want to pass to the configure command
in the terminal.

See the [presets documentation][1] if you want to go down that rabbit hole.

**Note**: Visual Studio Code is a great editor that can help you with this step!

### Configure, build and test

If you followed the above instructions, then you can configure, build and test
the project respectively with the following commands from the project root on
any operating system with any build system:

```sh
cmake -S . -B build --preset=dev-linux
cmake --build build --preset=dev-linux
ctest --preset=dev-linux
```

If you are using a compatible editor you can select the above created presets
for automatic integration, neat! Isn't?

### Developer mode targets

These are targets you may invoke using the build command from above, with an
additional `-t <target>` flag
(for example `cmake --build build/dev-linux/ --target format-check`).

- #### `format-check` and `format-fix`

	These targets run the clang-format tool on the codebase to check errors and to
fix them respectively. Customization available using the `FORMAT_PATTERNS` and
`FORMAT_COMMAND` cache variables.

- #### `spell-check` and `spell-fix`

	These targets run the codespell tool on the codebase to check errors and to fix
them respectively. Customization available using the `SPELL_COMMAND` cache
variable. We configured it to ignore CMakeLists files and uppercase words.

- #### `run-exe`
	 Runs the executable target `route-master_exe`.

## Contributing [↑](#route-master)

Please refer to the [CONTRIBUTING](CONTRIBUTING.md) document.

## Acknowledgments [↑](#route-master)

These are the great projects that make all of this possible:

* [DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR)
* [SFML](https://github.com/SFML/SFML)
* [Catch2](https://github.com/catchorg/Catch2)
* [tmxlite](https://github.com/fallahn/tmxlite)
* [JSON for Modern C++](https://github.com/nlohmann/json)

Repositories that served as reference:
* [sfml-game-template](https://github.com/nathandaven/sfml-game-template)
* [cmake-sfml-project](https://github.com/SFML/cmake-sfml-project)
* [cmake-sfml-demo](https://github.com/danebulat/cmake-sfml-demo)
* [cmake-init](https://github.com/friendlyanon/cmake-init)


[1]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
[2]: https://cmake.org/download/
[3]: https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd
[4]: https://containers.dev/
[5]: https://cmake.org/cmake/help/latest/module/FetchContent.html
[6]: https://www.sfml-dev.org/tutorials/2.6/compile-with-cmake.php
