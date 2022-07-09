from pathlib import Path

import pytest

from client.client_interface import Speculos, Ledgercomm
from client.cmd import Command

SCRIPT_DIR = Path(__file__).absolute().parent


def pytest_addoption(parser):
    parser.addoption("--hid", action="store_true")
    parser.addoption("--headless", action="store_true")
    parser.addoption("--manual", action="store_true")


@pytest.fixture(scope="module")
def sw_h_path():
    # path with tests
    conftest_folder_path: Path = Path(__file__).parent
    # sw.h should be in ../../src/sw.h
    sw_h_path = conftest_folder_path.parent.parent / "src" / "sw.h"

    if not sw_h_path.is_file():
        raise FileNotFoundError(f"Can't find sw.h: '{sw_h_path}'")

    return sw_h_path


@pytest.fixture(scope="session")
def hid(pytestconfig):
    return pytestconfig.getoption("hid")


@pytest.fixture(scope="session")
def headless(pytestconfig):
    return pytestconfig.getoption("headless")


@pytest.fixture(scope="session")
def manual(pytestconfig):
    return pytestconfig.getoption("manual")


@pytest.fixture(scope="session")
def cmd(hid, headless, manual):
    file_path = SCRIPT_DIR.parent.parent / "bin" / "app.elf"
    args = ["--model", "nanos", "--sdk", "2.1"]
    if headless:
        manual = False
    else:
        args.append("--display")
        args.append("qt")

    transport = (
        Ledgercomm()
        if hid
        else Speculos(file_path=str(file_path), args=args, automatic=not manual)
    )
    command = Command(transport=transport, debug=True)
    command.transport.start()
    yield command
    command.transport.close()
