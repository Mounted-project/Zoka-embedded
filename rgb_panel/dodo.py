from os import path, getenv
from doit.tools import Interactive

from glob import glob
from sys import platform

IDF_PATH = 'tools/esp-idf/'
ZOKA_VIRTUAL_ENV         = './zoka-venv'

env_port = getenv('ZOKA_SERIAL_PORT')
if env_port:
  DEFAULT_SERIAL_PORT = env_port
else:
  if platform == "linux" or platform == "linux2":
    DEFAULT_SERIAL_PORT = "/dev/ttyUSB1"
  elif platform == "darwin":
    usbmodem_ports = glob('/dev/tty.usbmodem*')
    if usbmodem_ports:
        DEFAULT_SERIAL_PORT = usbmodem_ports[0]  # Use the first found USB modem port
    else:
        DEFAULT_SERIAL_PORT = "No USB-MODEM found, please check ESP-S3 connection"  # Use the first found USB modem port
  elif platform == "win32":
    DEFAULT_SERIAL_PORT = "COM0"


def WITH_IDF(s):
  return f'source {path.join(IDF_PATH, "export.sh")} && {s}'

def WITH_PACKAGES(s):
  return f'echo OK'
  # return f'pip install -r requirements.txt && {s}'

def CLI_PRINT(s):
  return '1>&2 ' + s

# Toolchain
def task_install():
  """Creates Python virtual env and installs the ESP-IDF toolchain and Pip dependencies in it."""
  return {
    'actions': [
      # f'python3 -m venv {ZOKA_VIRTUAL_ENV}',
      # f'. {ZOKA_VIRTUAL_ENV}/bin/activate',
      Interactive(CLI_PRINT('git submodule update --init --recursive')),
      # 'pip install -r requirements.txt',
      Interactive(CLI_PRINT('tools/esp-idf/install.sh')),
      Interactive(CLI_PRINT('source tools/esp-idf/export.sh')),
      Interactive(WITH_IDF(CLI_PRINT('idf.py fullclean'))),
    ],
  }

def task_build():
  """Builds the firmware."""
  return {
    'actions': [
      Interactive(WITH_IDF(CLI_PRINT('idf.py build')))
    ],
  }


def task_flash():
  """Flashes the firmware."""
  return {
    'actions': [
      Interactive(WITH_IDF(WITH_PACKAGES(CLI_PRINT('idf.py flash -p "%(port)s"'))))
    ],
    'params':[{
      'name':'port',
      'short':'p',
      'default': DEFAULT_SERIAL_PORT
    }],
  }

# def task_flash_monitor():
#     """Flash the firmware to the device"""
#     def flash_monitor(port):
#         return f'idf.py -p {port} flash monitor {port}'

#     return {
#         'actions': [flash_monitor],
#         'params': [{
#             'name': 'port',
#             'short': 'p',
#             'default': '/dev/ttyUSB0',  # Default port if not specified
#             'help': 'Serial port to use for flashing the device',
#         }],
#         'file_dep': ['build/app.elf'],  # depends on the compiled binary
#     }

def task_monitor():
  """Monitors the connected Zoka."""
  return {
    'actions': [
      Interactive(WITH_IDF(WITH_PACKAGES(CLI_PRINT('idf.py monitor -p "%(port)s"'))))
    ],
    'params':[{
      'name':'port',
      'short':'p',
      'default': DEFAULT_SERIAL_PORT
    }]
  }

def task_fullclean():
    """Clean the project"""
    return {
        'actions': ['idf.py fullclean'],
        'targets': ['.config', 'build/'],
        'clean': True,
    }
