#!/usr/bin/env sh

######################################################################
# @author      : kistenklaus (karlsasssie@gmail.com)
# @file        : run
# @created     : Sonntag Jun 23, 2024 14:28:45 CEST
#
# @description : 
######################################################################

canzero gen motor_driver src/canzero
cmake -Bbuild
cmake --build build -j8
alacritty -e ./build/motor-board&


