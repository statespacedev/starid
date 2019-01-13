import os
import sys
import subprocess
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " + ", ".join(e.name for e in self.extensions))
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir, '-DPYTHON_EXECUTABLE=' + sys.executable]
        cfg = 'Release' #'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        build_args += ['--', '-j2']
        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''), self.distribution.get_version())
        if not os.path.exists(self.build_temp): os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)
        print()

__version__ = None
if os.environ.get('CI_COMMIT_TAG'):
    __version__ = os.environ['CI_COMMIT_TAG']
elif os.environ.get('CI_JOB_ID'):
    __version__ = os.environ['CI_JOB_ID']
if __version__:
    with open('__version__', 'wt') as fout:
        fout.write(__version__)
if os.path.exists('__version__'):
    with open('__version__', 'rt') as fin:
        for line in fin:
            __version__ = line.strip()

with open('requirements.txt') as fin:
    required = fin.read().splitlines()
with open('README.md', 'r', encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name='starid',
    version=__version__,
    author='noah smith',
    author_email='noahhsmith@gmail.com',
    description='star identification',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://gitlab.com/noahhsmith/starid',
    packages=find_packages(),
    entry_points={'console_scripts': ["starid = starid.__main__:main"]},
    ext_modules=[CMakeExtension('starid')],
    cmdclass=dict(build_ext=CMakeBuild),
    install_requires=required,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)