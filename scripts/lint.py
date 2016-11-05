#!/usr/bin/env python

import fnmatch
import os
import sys
import subprocess

IGNORE_FILES = [
  
]

SOURCE_ROOT = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def execute(argv, env=os.environ):
  try:
    output = subprocess.check_output(argv, stderr=subprocess.STDOUT, env=env)
    return output
  except subprocess.CalledProcessError as e:
    print e.output
    raise e


def main():
  os.chdir(SOURCE_ROOT)
  files = list_files(['src'],
                     ['*.cc','*.h'])
  call_cpplint(list(set(files) - set(IGNORE_FILES)))


def list_files(directories, filters):
  matches = []
  for directory in directories:
    for root, _, filenames, in os.walk(os.path.join('.', directory)):
      for f in filters:
        for filename in fnmatch.filter(filenames, f):
          matches.append(os.path.join(root, filename))
  return matches


def call_cpplint(files):
  cpplint = os.path.join(SOURCE_ROOT,'scripts', 'cpplint.py')

  execute([sys.executable, cpplint] + files)


if __name__ == '__main__':
  sys.exit(main())