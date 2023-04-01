import subprocess
import os.path

def report_fail(msg):
  print(f'(FAIL: {msg})')

def report_pass():
  print('(PASS)')

def trim(s):
  s = repr(s)
  if len(s) > 100:
    s = s[:100] + '...'
  return s

def run_tests(TESTS):
  pass_count = 0
  for test in TESTS:
    (test_desc, test_cmd, test_expresultcode, test_expresult) = test
    print(f'Test: {test_desc} ', end='')
      
    result = subprocess.run('timeout 2 ' + test_cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    if result.returncode != test_expresultcode:
      report_fail('Incorrect exit code')
      print('Output:\n', trim(result.stdout))
      continue
    if result.stdout.strip() != test_expresult:
      report_fail('Incorrect output')
      print('Output:\n', trim(result.stdout))
      continue

    report_pass()
    pass_count += 1

  print(f'{pass_count} of {len(TESTS)} passed.')



def test_mycat():
  print('\nmy-cat tests')
  print('============')

  if not os.path.exists('my-cat'):
    print('No my-cat found.')
    return

  TESTS = [
    ('No arguments', './my-cat', 0, b''),
    ('Missing file', './my-cat notpresent', 1, b'my-cat: cannot open file'),
    ('Single file', './my-cat test_files/file1.txt', 0, b'''some data\nline 2'''),
    ('Multiple files', './my-cat test_files/file1.txt test_files/file2.txt test_files/file3.txt', 0, b'''some data\nline 2\nfile 2\nfile 3'''),
    ('Big file', './my-cat test_files/bigfile.txt', 0, b'X' * 4000000)
  ]

  run_tests(TESTS)


def test_mygrep():
  print('\nmy-grep tests')
  print('=============')

  if not os.path.exists('my-grep'):
    print('No my-grep found.')
    return

  TESTS = [
    ('No arguments', './my-grep < /dev/null', 1, b'my-grep: searchterm [file ...]'),
    ('Missing file', './my-grep foo notpresent < /dev/null', 1, b'my-grep: cannot open file'),
    ('No input file', './my-grep ne < test_files/grepfile.txt', 0, b'line 1\nline 2'),
    ('Multiple files', './my-grep ne test_files/grepfile.txt test_files/file1.txt', 0, b'line 1\nline 2\nline 2'),
  ]

  run_tests(TESTS)

import struct
def i2s(num):
  return struct.pack('<i', num)

def test_myzip():
  print('\nmy-zip tests')
  print('=============')

  if not os.path.exists('my-zip'):
    print('No my-zip found.')
    return

  TESTS = [
    ('No arguments', './my-zip < /dev/null', 1, b'my-zip: file1 [file2 ...]'),
    ('One file', './my-zip test_files/zipme.txt < /dev/null', 0, b'\x03\x00\x00\x00a\x01\x00\x00\x00b\x02\x00\x00\x00c'),
  ]

  run_tests(TESTS)

def test_myunzip():
  print('\nmy-unzip tests')
  print('===============')

  if not os.path.exists('my-unzip'):
    print('No my-unzip found.')
    return

  TESTS = [
    ('No arguments', './my-unzip < /dev/null', 1, b'my-unzip: file1 [file2 ...]'),
    ('One file', './my-unzip test_files/unzipme.dat < /dev/null', 0, b'aaabcc'),
  ]

  run_tests(TESTS)


test_mycat()
test_mygrep()
test_myzip()
test_myunzip()

