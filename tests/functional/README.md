# Functional Tests

> :point_right: Every path on this document assumes you are at the root of the repository.

These tests are implemented in Python and can be executed either using the
[Speculos](https://github.com/LedgerHQ/speculos) emulator or a Ledger Nano S/X.

Python dependencies are listed in [requirements.txt](requirements.txt), install
them using [pip](https://pypi.org/project/pip/)

```shell
pip install --extra-index-url https://test.pypi.org/simple/ -r tests/functional/requirements.txt
```

> The `--extra-index-url` parameter helps fetch the latest version of Speculos.

## Manual (Speculos GUI)

The `--manual` parameter is used to launch the Speculos GUI via command line and is offered as a convenient alternative to opening Speculos separately.

```shell
pytest tests/functional/ --manual
```

## Headless (Speculos Backend)

The `--headless` parameter is used to run functional tests using the Speculos backend where button presses will be handled automatically.

```shell
pytest tests/functional/ --headless
```

## Launch with your Nano S/X/SPlus

To run the tests on your Ledger Nano S/X/SPlus, make sure you have the Solar application installed (see
[this page](https://developers.ledger.com/docs/nano-app/load/) for installing an
application on a Nano S) and opened on the device, and the device connected
through USB, without any other software interacting with it. Then run:

```shell
pytest tests/functional/ --hid
```
