#!/bin/env python3

from dotenv import load_dotenv
from pathlib import Path
from typing import List
import argparse
import contextlib
import os
import subprocess as sp
import sys

PROJECT_ROOT_PATH = ''
CMDLINE = ['cmake', '--build', 'build', '--config', 'Debug']


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
        # fallback to .env.local file
        cwd = os.getcwd()
        load_dotenv(dotenv_path=f'{cwd}/.env.local')

        resolved_root = Path(os.environ['PROJECT_ROOT']).resolve()
        PROJECT_ROOT_PATH = resolved_root

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
