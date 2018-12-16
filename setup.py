# setup.py
# Author: Thomas MINIER - MIT License 2017-2018
from setuptools import setup, Extension
from os import listdir
import pybind11
import distutils
import platform

__pyhdt_version__ = "1.2.1"

with open('README.rst') as file:
    long_description = file.read()


def list_files(path, extension=".cpp", exclude="S.cpp"):
    """List paths to all files that ends with a given extension"""
    return ["%s/%s" % (path, f) for f in listdir(path) if f.endswith(extension) and (not f.endswith(exclude))]


# pyHDT source files
sources = [
    "src/hdt.cpp",
    "src/hdt_document.cpp",
    "src/triple_iterator.cpp",
    "src/tripleid_iterator.cpp",
    "src/join_iterator.cpp"
]

# HDT source files
sources += list_files("serd-0.30.0/src/", extension=".c")
sources += list_files("hdt-cpp-1.3.2/libcds/src/static/bitsequence")
sources += list_files("hdt-cpp-1.3.2/libcds/src/static/coders")
sources += list_files("hdt-cpp-1.3.2/libcds/src/static/mapper")
sources += list_files("hdt-cpp-1.3.2/libcds/src/static/sequence")
sources += list_files("hdt-cpp-1.3.2/libcds/src/static/permutation")
sources += list_files("hdt-cpp-1.3.2/libcds/src/utils")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/bitsequence")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/dictionary")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/hdt")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/header")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/huffman")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/libdcs")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/libdcs/fmindex")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/rdf")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/sequence")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/triples")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/util")
sources += list_files("hdt-cpp-1.3.2/libhdt/src/sparql")

# pybind11 + pyHDT + libcds +  HDT-lib headers
include_dirs = [
    pybind11.get_include(),
    pybind11.get_include(True),
    "include/",
    "hdt-cpp-1.3.2/libhdt/include/",
    "hdt-cpp-1.3.2/libhdt/src/dictionary/",
    "hdt-cpp-1.3.2/libhdt/src/sparql/",
    "hdt-cpp-1.3.2/libcds/include/",
    "hdt-cpp-1.3.2/libcds/src/static/bitsequence",
    "hdt-cpp-1.3.2/libcds/src/static/coders",
    "hdt-cpp-1.3.2/libcds/src/static/mapper",
    "hdt-cpp-1.3.2/libcds/src/static/permutation",
    "hdt-cpp-1.3.2/libcds/src/static/sequence",
    "hdt-cpp-1.3.2/libcds/src/utils",
    "serd-0.30.0"
]

# Need to build in c++11 minimum
# TODO add a check to use c++14 or c++17 if available
extra_compile_args_macos = ["-std=c++11", "-DHAVE_SERD", "-DHAVE_POSIX_MEMALIGN"]
extra_compile_args_win = ["-DHAVE_SERD", "-DWIN32", "-D_AMD64_", "-DUNICODE"]

plaf = platform.system()
if plaf == "Windows":
    extra_compile_args = extra_compile_args_win
elif plaf == "Darwin":
    extra_compile_args = extra_compile_args_macos
else:
    extra_compile_args = ["-std=c++11", "-DHAVE_SERD", "-DHAVE_POSIX_MEMALIGN"]

# build HDT extension
hdt_extension = Extension("hdt", sources=sources, include_dirs=include_dirs,
                          extra_compile_args=extra_compile_args, language='c++')

# monkey patch the distutils compiler to enable compilation of the Serd parser source
# it is C, and the C++11 compile argument is incompatible
c = distutils.ccompiler.new_compiler
def wrapped_new_compiler_fn(*args, **kwargs):
    compiler = c(*args, **kwargs)
    c_c = compiler._compile

    def wrapped_compiler_compile(obj, src, ext, cc_args, extra_postargs, pp_opts):
        if ext == ".c":
            return c_c(obj, src, ext, cc_args, [ "-DHAVE_SERD" ], pp_opts)
        else:
            return c_c(obj, src, ext, cc_args, extra_postargs, pp_opts)
    compiler._compile = wrapped_compiler_compile
    return compiler
distutils.ccompiler.new_compiler = wrapped_new_compiler_fn

setup(
    name="hdt",
    version=__pyhdt_version__,
    author="Thomas Minier",
    author_email="thomas.minier@univ-nantes.fr",
    url="https://github.com/Callidon/pyHDT",
    description="Read and query HDT document with ease in Python",
    long_description=long_description,
    keywords=["hdt", "rdf", "semantic web", "search"],
    license="MIT",
    install_requires=['pybind11==2.2.4'],
    ext_modules=[hdt_extension]
)
