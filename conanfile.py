from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class coffeeshopRecipe(ConanFile):
    name = "coffeeshop"
    version = "0.1"
    package_type = "application"

    # Optional metadata
    license = "MIT"
    author = "Max Musterman"
    description = "Who never wanted to own a coffee shop? I certainly didn't."
    topics = ("C++20", "V8", "SFML", "XML")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def requirements(self):
        self.requires("sfml/2.6.2")
        self.requires("boost/1.85.0")
        self.requires("libxml2/2.15.0")
        self.requires("nlohmann_json/3.12.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
