#!/bin/env python3

from dotenv import load_dotenv
from pathlib import Path
from typing import List, Dict
import argparse
import contextlib
import os
import subprocess as sp
import shutil
import sys

# CMake commands
CMAKE_ARGS = {
    'test:lib': [
        'ctest',
        '--test-dir', '$BUILDDIR$/lib',
        '--verbose'
    ],
    'test:cli': [
        'ctest',
        '--test-dir', '$BUILDDIR$/bin/fileDiffCLI',
        '--verbose'
    ],
    'test:gui': [
        'ctest',
        '--test-dir', '$BUILDDIR$/bin/fileDiffGUI',
        '--verbose'
    ],
    'build:debug': [
        'cmake',
        '--build', '$BUILDDIR$',
        '--config', 'Debug',
        '--verbose'
    ],
    'build:release': [
        'cmake',
        '--build', '$BUILDDIR$',
        '--config', 'Release'
    ],
    'configure:debug': [
        'cmake',
        '-S', '.',
        '-B', '$BUILDDIR$',
        '-G', '$GENERATOR$',
        # '-DMSYSTEM=$MSYSTEM$',
        # '-DCMAKE_MODULE_PATH=vendor/MSYS2-toolchain/scripts/cmake/Modules',
        # '-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN$',
        '-DCMAKE_BUILD_TYPE=Debug',
        '-Ddev_WARNINGS_AS_ERRORS=OFF',
        '--log-level=DEBUG'
    ],
    'configure:release': [
        'cmake',
        '-S', '.',
        '-B', '$BUILDDIR$',
        '-G', '$GENERATOR$',
        # '-DMSYSTEM=$MSYSTEM$',
        # '-DCMAKE_MODULE_PATH=vendor/MSYS2-toolchain/scripts/cmake/Modules',
        # '-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN$',
        '-DCMAKE_BUILD_TYPE=Release'
    ],
}

# what files to look for to find the root project
ROOT_FILES = [
    '.editorconfig',
    '.gitignore',
    'CMakeLists.txt',
    'LICENSE',
    'README.md',
]


def eprint(*args, **kwargs):
    # print to stderr
    print(*args, file=sys.stderr, **kwargs)


@contextlib.contextmanager
def pushd(new_dir: str):
    # pushd implementation
    prev = os.getcwd()

    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(prev)


def check_for_root() -> Path:
    # find root in subdirectories above
    cwd: str = os.getcwd()

    # check cwd before going up/down dirs
    count = 0
    results: Dict[int, bool] = {}
    for file in ROOT_FILES:
        tmp: Path = Path(cwd).resolve() / file
        if tmp.exists():
            count += 1
        else:
            continue

    if count == len(ROOT_FILES):
        return Path(cwd).resolve()

    # now check parent directories
    for parent_index in range(0, 3):
        root_cand: Path = Path(cwd).parents[parent_index].resolve()

        count = 0
        for file in ROOT_FILES:
            tmp: Path = root_cand / file
            if tmp.exists():
                count += 1
            else:
                continue

        if count == len(ROOT_FILES):
            results[parent_index] = True

    for parent_index, is_root in results.items():
        if is_root == True:
            root_path = Path(cwd).parents[parent_index].resolve()
            return root_path

    return None


def run_cmake_cmd(cmdline: List[str]) -> sp.CompletedProcess[str]:
    process = sp.run(cmdline, capture_output=True)

    return process


def clean_dirs(path: Path, build: str):
    build_dir = path / build
    target_dir = path / 'target'

    shutil.rmtree(build_dir)
    shutil.rmtree(target_dir)


def main():
    # read and parse launch params
    root_path = None

    parser = argparse.ArgumentParser(description='Build utility scripts')
    parser.add_argument('--root', type=str, help='path to project root')
    parser.add_argument('--config', type=str, help='debug or release')
    parser.add_argument('--action', type=str, help='build or configure')
    parser.add_argument(
        '-T', '--test', action='store_true', help='run tests?')
    parser.add_argument('-B', '--binary', type=str, help='test binary to run')
    parser.add_argument(
        '-C', '--clean', action='store_true', help='clean build?')
    args = parser.parse_args()

    if args.root:
        root_path = Path(args.root).resolve()
    else:
        root_path = check_for_root()

    # substitute variables from dotenv
    load_dotenv(dotenv_path=f'{root_path}/.env')

    build_dir = os.environ['BUILDDIR']
    generator = os.environ['GENERATOR']
    # toolchain = os.environ['TOOLCHAIN']
    # msystem = os.environ['MSYSTEM']

    if args.clean and not args.test:
        clean_dirs(root_path, build_dir)
        return ()

    # fetch cmake/ctest params using launch params
    cmake_params = ''
    cmake_cmdline = []

    if not args.test:
        cmake_params = f'{args.action}:{args.config}'
        cmake_cmdline = CMAKE_ARGS[cmake_params]
    else:
        cmake_params = f'test:{args.binary}'
        cmake_cmdline = CMAKE_ARGS[cmake_params]

    cmake_cmdline = [c.replace('$BUILDDIR$', build_dir) for c in cmake_cmdline]
    cmake_cmdline = [c.replace('$GENERATOR$', generator)
                     for c in cmake_cmdline]
    # cmake_cmdline = [c.replace('$TOOLCHAIN$', toolchain)
    #                  for c in cmake_cmdline]
    # cmake_cmdline = [c.replace('$MSYSTEM$', msystem)
    #                  for c in cmake_cmdline]

    # run the commands
    process: sp.CompletedProcess[str] = None
    with pushd(root_path):
        process = run_cmake_cmd(cmake_cmdline)

    if process.returncode != 0:
        eprint('failed to run CMake')
        eprint(f' - return code: ${process.returncode}')
        eprint(f' - command line: ${cmake_cmdline}')

        if process.stdout:
            stdout = process.stdout.decode('utf-8')
            eprint(f' - stdout:\n{stdout}')
        if process.stderr and not (process.stdout == process.stderr):
            stderr = process.stderr.decode('utf-8')
            eprint(f' - stderr:\n{stderr}')
    else:
        if process.stdout:
            stdout = process.stdout.decode('utf-8')
            print(f' - stdout:\n{stdout}')


if __name__ == '__main__':
    main()
