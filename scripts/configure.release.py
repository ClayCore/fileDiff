#!/bin/env python3

from pathlib import Path
from typing import List, Dict
import argparse
import contextlib
import os
import subprocess as sp
import sys

CMDLINE = ['cmake', '-S', '.', '-B', 'build', '-G', 'Ninja',
           '-Ddev_WARNINGS_AS_ERRORS=OFF', '-DCMAKE_BUILD_TYPE=Release']
FILES = ['README.md', '.env.local', '.gitignore', 'CMakeLists.txt']
PROJECT_ROOT_PATH = None


def check_for_root() -> Path:
    cwd: str = os.getcwd()

    results: Dict[int, bool] = {}
    for parent in range(1, 3):
        root_cand: Path = Path(cwd).parents[parent].resolve()
        print(f'selected path: {root_cand}')

        for file in FILES:
            tmp: Path = root_cand / file
            if not tmp.exists():
                continue
            else:
                results[parent] = True

    for parent_index, is_root in results.items():
        if is_root == True:
            root_path = Path(cwd).parents[parent_index].resolve()
            return root_path

    return None


@contextlib.contextmanager
def pushd(new_dir: str):
    # pushd implementation

    previous_dir = os.getcwd()
    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(previous_dir)


def run_cmd(cmdline: List[str]) -> sp.CompletedProcess[str]:
    process = sp.run(cmdline, capture_output=True)

    return process


def main():
    # read and parse launch params
    parser = argparse.ArgumentParser(description='Build utility script')
    parser.add_argument('--root', type=str, help='path to project root')
    args = parser.parse_args()

    if args.root:
        resolved_root = Path(args.root).resolve()
        PROJECT_ROOT_PATH = resolved_root
    else:
        # fallback to finding root path ourselves
        PROJECT_ROOT_PATH = str(check_for_root())

    process: sp.CompletedProcess[str] = None
    with pushd(PROJECT_ROOT_PATH):
        process = run_cmd(CMDLINE)

    if process.returncode != 0:
        print('failed to run CMake', file=sys.stderr)
        print(f' - return code: {process.returncode}', file=sys.stderr)

        print(f' - command line: {CMDLINE}', file=sys.stderr)
        if process.stdout:
            stdout = process.stdout.decode('utf-8')
            print(f' - stdout:\n{stdout}', file=sys.stderr)
        if process.stderr and not (process.stdout == process.stderr):
            stderr = process.stderr.decode('utf-8')
            print(f' - stderr:\n{stderr}', file=sys.stderr)


if __name__ == '__main__':
    main()
