# ****************************************************************************
#    Ledger App Boilerplate
#    (c) Ledger SAS.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
# ****************************************************************************

ifeq ($(BOLOS_SDK),)
$(error Environment variable BOLOS_SDK is not set)
endif

include $(BOLOS_SDK)/Makefile.defines

########################################
#        Mandatory configuration       #
########################################
APP_LOAD_PARAMS += --tlvraw 9F:01

# Application name
APPNAME = "Solar"

# Application version
APPVERSION_M = 1
APPVERSION_N = 1
APPVERSION_P = 3
APPVERSION = "$(APPVERSION_M).$(APPVERSION_N).$(APPVERSION_P)"

# Application source files
APP_SOURCE_PATH += src

# Application icons following guidelines:
# https://developers.ledger.com/docs/embedded-app/design-requirements/#device-icon
ICON_NANOS = icons/app_solar_16px.gif
ICON_NANOX = icons/app_solar_14px.gif
ICON_NANOSP = icons/app_solar_14px.gif
# ICON_STAX = icons/app_boilerplate_32px.gif

# Application allowed derivation curves.
CURVE_APP_LOAD_PARAMS = secp256k1

# Application allowed derivation paths.
PATH_APP_LOAD_PARAMS = "44'/3333'" "44'/1'"

# Setting to allow building variant applications
VARIANT_PARAM = COIN
VARIANT_VALUES = SXP

# Enabling DEBUG flag will enable PRINTF and disable optimizations
#DEBUG = 1

########################################
# Application communication interfaces #
########################################
ENABLE_BLUETOOTH = 1
#ENABLE_NFC = 1

########################################
#         NBGL custom features         #
########################################
#ENABLE_NBGL_QRCODE = 1
#ENABLE_NBGL_KEYBOARD = 1
#ENABLE_NBGL_KEYPAD = 1

include $(BOLOS_SDK)/Makefile.standard_app
