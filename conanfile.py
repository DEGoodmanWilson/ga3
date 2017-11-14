from conans import ConanFile, CMake, tools


class Ga2Conan(ConanFile):
    name = "ga2"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/DEGoodmanWilson/ga2.git"
    description = "<Description of Ga2 here>"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"

#     def source(self):
#         self.run("git clone %s"%(self.url))
#         self.run("cd ga2 && git checkout v%s"%(self.version))
#         # This small hack might be useful to guarantee proper /MT /MD linkage in MSVC
#         # if the packaged project doesn't have variables to set it properly
#         tools.replace_in_file("ga2/CMakeLists.txt", "PROJECT(ga2)", '''PROJECT(ga2)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        # cmake.configure(source_dir="%s/ga2" % sellf.source_folder)
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/ga2 %s' % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)
        
    def package(self):
        self.copy("*.h", dst="include", src="ga2")
        self.copy("*ga2.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ga2"]
