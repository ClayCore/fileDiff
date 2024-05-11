#!/bin/env python3

from typing import Dict
from pathlib import Path
import argparse
import contextlib
import os
import shutil

FILES = ['README.md', '.gitignore', 'CMakeLists.txt']
PROJECT_ROOT_PATH = None


def check_for_root() -> Path:
    cwd: str = os.getcwd()

    results: Dict[int, bool] = {}
    count = 0
    for file in FILES:
        tmp: Path = Path(cwd).resolve() / file
        if not tmp.exists():
            continue
        else:
            count += 1

    if count == len(FILES):
        return Path(cwd).resolve()
    else:
        for parent in range(-2, 3):
            root_cand: Path = Path(cwd).parents[parent].resolve()
            print(f'selected path: {root_cand}')
            print(f'index: {parent}')

            count = 0
            for file in FILES:
                tmp: Path = root_cand / file
                if not tmp.exists():
                    continue
                else:
                    count += 1

            if count == len(FILES):
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


def clean_dir():
    build_dir = Path(os.getcwd()) / 'build'
    target_dir = Path(os.getcwd()) / 'target'

    shutil.rmtree(build_dir)
    shutil.rmtree(target_dir)


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

    with pushd(PROJECT_ROOT_PATH):
        clean_dir()


if __name__ == '__main__':
    main()
