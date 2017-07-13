#!/bin/bash

# this script installs the following dependencies necessary to
# build the samd21 freertos demo
# - toolchain
# - source code libraries
# - programming utility

# global variables
BASE_DIR=$(dirname $0) # /path/to/samd21_freertos_demo
ASF_NAME="asf-standalone-archive-3.34.2.53.zip"
ASF_DIR="xdk-asf-3.34.2"
ASF_URL="http://ww1.microchip.com/downloads/en/DeviceDoc/"
EDBG_URL="https://github.com/ataradov/edbg.git"

function check_exit_code { # $1 = exit code, $2 = error string to print
    if [[ $1 -ne 0 ]]; then
        printf "Error: %s\n\tFailed with exit code: %d\n" "$2" $1
        exit 1
    fi
}

function linux_install {
    # use apt-get to install the arm-none-eabi toolchain for ARM Cortex-M MCU's
    if [[ -x $(which apt-get) ]]; then
        printf "\nInstalling toolchain, please provide password to apt-get\n\n"
        sudo apt-get --yes install wget make gcc-arm-none-eabi \
            binutils-arm-none-eabi libudev-dev || exit 1
    else
        printf "'apt-get' does not exist, cannot install dependencies\n"
        exit 1
    fi
}

function macosx_install {
    # check for homebrew
    if [[ ! -x $(which brew) ]]; then
        printf "Installing homebrew\n"
        /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    else
        printf "Updating homebrew\n"
        brew update
    fi

    # check exit code
    check_exit_code $? "homebrew install/update failed\n"

    # arm-none-eabi toolchain is not in the standard homebrew repos
    brew tap ARMmbed/homebrew-formulae || exit 1

    printf "\nInstalling toolchain\n\n"
    brew install wget make arm-none-eabi-gcc || exit 1
}

# check OS type. Only Mac OS X and a Debian based Linux are supported.
case "$OSTYPE" in
    darwin*)
        printf "Installing dependencies for a Mac OS X system\n"
        macosx_install
        ;;
    linux*)
        printf "Installing dependencies for a Linux system\n"
        linux_install
        ;;
    *)
        printf "Operating system not supported\n"
        exit 1
        ;;
esac

# if samd21_freertos_demo/xdk-asf-3.34.2 does not exist
# then install the Advanced Software Framework
if [[ ! -d "$BASE_DIR/$ASF_DIR" ]]; then
    printf "\nInstalling the Advanced Software Framework\n\n"

    # download the archive and (if successful) extract it
    wget $ASF_URL$ASF_NAME -P $BASE_DIR && \
    unzip $BASE_DIR/$ASF_NAME

        # unzip on Mac OSX extracts successfully, but returns an exit code of 3
        # check if archive was extracted, not the unzip return code
        if [[ -d "$BASE_DIR/$ASF_DIR"  && -f "$BASE_DIR/$ASF_NAME" ]]; then
            rm -v $BASE_DIR/$ASF_NAME
            check_exit_code $? "Cleanup failed"
        else
            printf "\nASF installation failed\n\n"
            exit 1
        fi
fi

# install the edbg programming utility
if [[ -x $(which git) ]]; then
    printf "\nInstalling the 'edbg' programming utility\n\n"
    git clone "$EDBG_URL" && \
    make -C edbg all
else
    printf "'git' command not found\n"
    exit 1
fi

printf "\nDone installing dependencies\n\n"
