// Copyright (C) 2022-2023 56software
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include "lexer.h"

typedef struct Language {
    char name[MAX_STR_VALUE_SIZE];
    char compiler[MAX_STR_VALUE_SIZE];
} Language;

typedef struct Package {
    /// Package name
    char name[MAX_STR_VALUE_SIZE];

    /// Languages
    Language* languages;

    /// Package version
    char version[MAX_STR_VALUE_SIZE];

    /// Package repo
    char repo[MAX_STR_VALUE_SIZE];

    /// Package sources
    char* sources[MAX_STR_VALUE_SIZE];
} Package;

typedef struct Subdirectories {
    char* dirs[MAX_STR_VALUE_SIZE];
} Subdirectories;

typedef struct Body {
    Package* pac;
    Subdirectories* subdirs;
} Body;
