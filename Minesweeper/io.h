#pragma once
#include "cJSON.h"
#include "logic.h"

void writeLog(const char* message);

void saveGameToJson(struct board* board);

bool loadGameFromJson(struct board* board);
