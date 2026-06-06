#pragma once
#include "cJSON.h"
#include "logic.h"

void saveGameToJson(struct board* board);

bool loadGameFromJson(struct board* board);
