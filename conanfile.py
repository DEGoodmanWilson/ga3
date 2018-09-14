from conans import ConanFile, CMake, tools


class Ga3Conan(ConanFile):
    name = "ga3"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/DEGoodmanWilson/ga3.git"
    description = "A basic library for implementing genetic algorithms in modern C++"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared":               [True, False],
               "build_ga3_tests":      [True, False],
               "build_ga3_coverage":   [True, False],
               "build_ga3_examples":   [True, False]}
    default_options = "shared=False", "build_ga3_tests=False", "build_ga3_coverage=False", "build_ga3_examples=False"
    exports = ["*"]
    generators = "cmake"

    def build_requirements(self):
        self.build_requires("Catch/[~=1.9]@bincrafters/stable")

    def configure(self):
        if self.options.build_ga3_coverage:
            self.options.build_ga3_tests=True

    def build(self):
        cmake = CMake(self)
        # cmake.configure(source_dir=self.source_folder)
        cmake.configure(defs={
            "BUILD_GA3_TESTS": "ON" if self.options.build_ga3_tests else "OFF",
            "BUILD_GA3_COVERAGE": "ON" if self.options.build_ga3_coverage else "OFF",
            "BUILD_GA3_EXAMPLES": "ON" if self.options.build_ga3_examples else "OFF"
        })
        cmake.build()
        if(self.options.build_ga3_tests):
            self.run('ctest . --verbose')


    def package(self):
        self.copy("*.hpp", dst="include/ga3", src="ga3")
        self.copy("*.hpp", dst="include/ThreadPool", src="ThreadPool") #TODO make this its own library!
        self.copy("*ga3.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ga3"]
