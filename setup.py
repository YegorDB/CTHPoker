from distutils.core import setup, Extension

setup(
    name='cthpoker',
    version='1.0.0',
    ext_modules=[Extension('cthpoker', ['pokerfinder.c'])]
)
