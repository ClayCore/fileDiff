#!/bin/env python3

import argparse
import os
import contextlib
import shutil
from dotenv import load_dotenv
from pathlib import Path

PROJECT_ROOT_PATH = ''


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
    build_dir = Path(PROJECT_ROOT_PATH) / 'build'
    target_dir = Path(PROJECT_ROOT_PATH) / 'target'

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
        # fallback to .env.local file
        cwd = os.getcwd()
        load_dotenv(dotenv_path=f'{cwd}/.env.local')

        resolved_root = Path(os.environ['PROJECT_ROOT']).resolve()
        PROJECT_ROOT_PATH = resolved_root

    with pushd(PROJECT_ROOT_PATH):
        clean_dir()


if __name__ == '__main__':
    main()
