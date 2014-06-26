#!/usr/bin/env python

import argparse
import os
import sys

from lib.config import LIBCHROMIUMCONTENT_COMMIT, BASE_URL
from lib.util import execute, scoped_cwd


SOURCE_ROOT = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
VENDOR_DIR = os.path.join(SOURCE_ROOT, 'vendor')
PYTHON_26_URL = 'https://chromium.googlesource.com/chromium/deps/python_26'
NPM = 'npm.cmd' if sys.platform in ['win32', 'cygwin'] else 'npm'


def main():
  os.chdir(SOURCE_ROOT)

  args = parse_args()
  update_submodules()
  update_node_modules('.')
  update_atom_modules('atom/browser/default_app')
  update_atom_modules('spec')
  bootstrap_brightray(args.url)
  if sys.platform == 'cygwin':
    update_win32_python()
  if sys.platform in ['win32', 'cygwin']:
    install_runas()

  create_chrome_version_h()
  touch_config_gypi()
  update_atom_shell()


def parse_args():
  parser = argparse.ArgumentParser(description='Bootstrap this project')
  parser.add_argument('-u', '--url',
                      help='The base URL from which to download '
                      'libchromiumcontent (i.e., the URL you passed to '
                      'libchromiumcontent\'s script/upload script',
                      default=BASE_URL,
                      required=False)
  return parser.parse_args()


def update_submodules():
  execute(['git', 'submodule', 'sync'])
  execute(['git', 'submodule', 'update', '--init', '--recursive'])


def bootstrap_brightray(url):
  bootstrap = os.path.join(VENDOR_DIR, 'brightray', 'script', 'bootstrap')
  execute([sys.executable, bootstrap, '--commit', LIBCHROMIUMCONTENT_COMMIT,
           url])


def update_node_modules(dirname):
  with scoped_cwd(dirname):
    execute([NPM, 'install'])


def update_atom_modules(dirname):
  with scoped_cwd(dirname):
    apm = os.path.join(SOURCE_ROOT, 'node_modules', '.bin', 'apm')
    if sys.platform in ['win32', 'cygwin']:
      apm = os.path.join(SOURCE_ROOT, 'node_modules', 'atom-package-manager',
                         'bin', 'apm.cmd')
    execute([apm, 'install'])


def update_win32_python():
  with scoped_cwd(VENDOR_DIR):
    if not os.path.exists('python_26'):
      execute(['git', 'clone', PYTHON_26_URL])


def install_runas():
  # TODO This is needed by the tools/win/register_msdia80_dll.js, should move
  # this to a better place.
  with scoped_cwd(os.path.join(SOURCE_ROOT, 'tools', 'win')):
    execute([NPM, 'install', 'runas'])


def create_chrome_version_h():
  version_file = os.path.join(SOURCE_ROOT, 'vendor', 'brightray', 'vendor',
                              'libchromiumcontent', 'VERSION')
  target_file = os.path.join(SOURCE_ROOT, 'atom', 'common', 'chrome_version.h')
  template_file = os.path.join(SOURCE_ROOT, 'script', 'chrome_version.h.in')

  with open(version_file, 'r') as f:
    version = f.read()
  with open(template_file, 'r') as f:
    template = f.read()
  with open(target_file, 'w+') as f:
    content = template.replace('{PLACEHOLDER}', version.strip())
    if f.read() != content:
      f.write(content)


def touch_config_gypi():
  config_gypi = os.path.join(SOURCE_ROOT, 'vendor', 'node', 'config.gypi')
  with open(config_gypi, 'w+') as f:
    content = '\n{}'
    if f.read() != content:
      f.write(content)


def update_atom_shell():
  update = os.path.join(SOURCE_ROOT, 'script', 'update.py')
  execute([sys.executable, update])


if __name__ == '__main__':
  sys.exit(main())
