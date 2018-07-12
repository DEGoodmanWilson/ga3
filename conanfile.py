from conans import ConanFile, CMake, tools


class Ga2Conan(ConanFile):
    name = "ga3"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/DEGoodmanWilson/ga3.git"
    description = "A basic library for implementing genetic algorithms in modern C++"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"

#     def source(self):
#         self.run("git clone %s"%(self.url))
#         self.run("cd ga3 && git checkout v%s"%(self.version))
#         # This small hack might be useful to guarantee proper /MT /MD linkage in MSVC
#         # if the packaged project doesn't have variables to set it properly
#         tools.replace_in_file("ga3/CMakeLists.txt", "PROJECT(ga3)", '''PROJECT(ga3)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()''')

    def build_requirements(self):
        self.build_requires("Catch/[~=1.9]@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        # cmake.configure(source_dir="%s/ga3" % sellf.source_folder)
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/ga3 %s' % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)
        
    def package(self):
        self.copy("*.h", dst="include", src="ga3")
        self.copy("*ga3.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ga3"]
