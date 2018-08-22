import setuptools

with open('readme.md', 'r') as fh:
    long_description = fh.read()

setuptools.setup(
    name='starid',
    version='2018.1.2',
    author='noah smith',
    author_email='noahhsmith@gmail.com',
    description='star identification',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/noahhsmith/starid',
    packages=setuptools.find_packages(),
    entry_points={
        'console_scripts': [
            'starid = starid.__main__:main'
        ]
    },
    classifiers=(
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ),
)