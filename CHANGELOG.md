# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.3] - 2023-08-17

### Changed

- Refactored deprecated throwable functions and patterns
- Updated Ragger testing library and functional tests

## [1.1.2] - 2023-08-16

### Added

- Merged LedgerHQ downstream commits
- Added 'pending security review flag'

## [1.1.1] - 2023-06-10

### Changed

- Refactored transaction display code for better readability
- Set explicit versions for functional test dependencies

### Removed

- Simplified code by removing trivial switch statement
- Removed unnecessary vote minimum check
- Removed redundant functional test dependencies
- Cleaned up functional tests by removing unused utilities

## [1.1.0] - 2023-06-08

### Added

- Expanded and improved CI workflows
- Introduced Ragger for enhanced functional testing

### Changed

- Refactored to align with LedgerHQ's SDKs
- Updated app menu copyright message
- Renamed icons to conform to LedgerHQ's naming conventions
- Revised documentation
- Updated Makefile to match LedgerHQ conventions
- Updated vote terminology
- Improved '.vscode' dev environment files
- Updated license headers in source files
- Enhanced unit-tests to cover recent changes

### Removed

- Removed HTLC transaction support
- Removed MultiSignature Registration transaction support

### Fixed

- Fixed outdated Ledger dev doc links in README
- Resolved clang static analysis failures
- Corrected typos

## [1.0.0] - 2022-07-09

### Added

- Implemented Solar transactions
- Implemented BIP340 Schnorr
- Applied app context reset on sign/error

### Changed

- Modified instruction flags
- Adjusted app UI
- Updated functional and unit tests
- Refreshed documentation

[1.1.3]: https://github.com/Solar-network/ledger-app-solar/compare/1.1.2...1.1.3
[1.1.2]: https://github.com/Solar-network/ledger-app-solar/compare/1.1.1...1.1.2
[1.1.1]: https://github.com/Solar-network/ledger-app-solar/compare/1.1.0...1.1.1
[1.1.0]: https://github.com/Solar-network/ledger-app-solar/compare/1.0.0...1.1.0
[1.0.0]: https://github.com/Solar-network/ledger-app-solar/compare/LedgerHQ:app-boilerplate:d7c7ca843e43f7b4982b87f1ac1d7bd66045448c...1.0.0
