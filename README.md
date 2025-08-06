# TeiaCareImage

Welcome to TeiaCareImage!

*TeiaCareImage* is a collection of C++ image processing utilities.

[![Docs](https://img.shields.io/badge/Docs-TeiaCareImage-white.svg)](https://teiacare.github.io/TeiaCareImage/md_README.html)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/cc286559eb114092ae9346726fa61a0a)](https://app.codacy.com/gh/TeiaCare/TeiaCareImage/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/cc286559eb114092ae9346726fa61a0a)](https://app.codacy.com/gh/TeiaCare/TeiaCareImage/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_coverage)

![teiacareimage](https://socialify.git.ci/teiacare/teiacareimage/image?description=1&font=Raleway&name=1&pattern=Solid&theme=Auto)

## Getting Started

This project uses git submodules so it is required to clone it using the *--recursive* flag in order to retrive the required submodules.

```bash
git clone https://github.com/TeiaCare/TeiaCareImage.git --recursive
```

### Create Development Environment
In order to setup a development environment it is sufficient to run the script *scripts/env/setup.<bat|sh>* depending on your operating system.

```bash
# Linux/MacOS
scripts/env/setup.sh

# Windows
scripts\env\setup.bat
```

### Start Development Environment
In order to start the development environment it is sufficient to activate the Python Virtual Environment just created the step above.

```bash
# Linux/MacOS
source .venv/bin/activate

# Windows
.venv\Scripts\activate.bat
```

### Setup Build Environment (Windows Only)
When building from command line on Windows it is necessary to activate the Visual Studio Developer Command Prompt.

Depending on the version of Visual Studio compiler and on its install location it is required to run *vcvars64.bat* script the set the development environment properly.

*Note*: using Visual Studio IDE or the CMake extension for VSCode this step is already managed in the background, so no action is required.

Examples:

```bash
# Visual Studio 2022 - Build Tools
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# Visual Studio 2019 - Enterprise
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
```

### Dependencies Setup
This script must be executed in order to setup the 3rd party dependencies using conan packages.
```bash
conan remote add teiacare https://artifactory.app.teiacare.com/artifactory/api/conan/teiacare --insert 0 --force
conan user <USERNAME> -p <PASSWORD> -r teiacare

python scripts/conan/setup.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>
```

### Configure, Build and Install
This script configures, builds and installs the library.
```bash
python scripts/cmake.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>
```

## License

This project is licensed under the [Apache License, Version 2.0](./LICENSE).

[![License](https://img.shields.io/badge/License-Apache_v2-blue)](./LICENSE)

Copyright © 2025 [TeiaCare](https://teiacare.com/)
