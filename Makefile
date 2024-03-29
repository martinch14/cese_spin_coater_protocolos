# Copyright 2016, Pablo Ridolfi
# All rights reserved.
#
# This file is part of Workspace.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# application name
PROJECT_NAME := $(notdir $(PROJECT))

# In evalboard.mk, you can choose your EvalBoard.
# This file is optional and not included in this git repo.
include $(PROJECT)/evalboard.mk

ifndef EVALBOARD
EVALBOARD = TMC4671_eval
endif

CDEFS += $(EVALBOARD) TMC_TYPES_BOOL_TRUE_FALSE

# Modules needed by the application
PROJECT_MODULES := modules/$(TARGET)/sapi \
                   modules/$(TARGET)/fatfs_ssp \
                   modules/$(TARGET)/base \
                   modules/$(TARGET)/board \
                   modules/$(TARGET)/chip \
                   modules/$(TARGET)/ciaa

# source files folder
PROJECT_SRC_FOLDERS := $(PROJECT)/src \
                       TMC-API/tmc/ic/TMC5130 \
                       TMC-API/tmc/ic/TMC4671 \
                       TMC-API/tmc/ramp \
                       TMC-API/tmc/helpers

# header files folder
PROJECT_INC_FOLDERS := $(PROJECT)/inc \
                       $(PROJECT)/inc/comm \
                       TMC-API

# source files
PROJECT_C_FILES := $(wildcard $(PROJECT)/src/*.c) \
                   TMC-API/tmc/ic/TMC5130/TMC5130.c \
                   TMC-API/tmc/ic/TMC4671/TMC4671.c \
                   TMC-API/tmc/ramp/LinearRamp.c \
                   TMC-API/tmc/helpers/Functions.c
PROJECT_ASM_FILES := $(wildcard $(PROJECT)/src/*.S)