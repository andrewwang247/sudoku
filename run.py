"""
Execute all Sudoku test cases.

Copyright 2020. Siwei Wang.
"""
from subprocess import run
from os import path, listdir
from pathlib import Path


def main():
    """Run all test cases and pipe output."""
    root = Path(__file__).parent.absolute()
    tests = listdir(path.join(root, 'Test'))
    run('make', check=True)
    for test in tests:
        name = path.splitext(test)[0]
        print(f'Running test case {name}...')
        solution = path.join('Solution', test)
        with open(solution, 'w') as fin:
            run(['./sudoku', f'Test/{test}'], stdout=fin, check=True)


if __name__ == '__main__':
    main()
