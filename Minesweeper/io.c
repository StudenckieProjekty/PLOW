#include "cJSON.h"
#include "logic.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void writeLog(const char* message) {
    time_t timeInSeconds = time(NULL);
    struct tm* timeInfo = localtime(&timeInSeconds);

    char timeText[24];
    strftime(timeText, sizeof(timeText), "%Y-%m-%d %H:%M:%S", timeInfo);

    printf("[%s] %s\n", timeText, message);

    FILE* fileToSave = fopen("game.log", "a");
    if (fileToSave != NULL) {
        fprintf(fileToSave, "[%s] %s\n", timeText, message);
        fclose(fileToSave);
    }
}

void saveGameToJson(struct board* board) {
    cJSON* json = cJSON_CreateObject();

    cJSON* matrixArray = cJSON_AddArrayToObject(json, "matrix");
    for (int i = 0; i < board->height; i++) {
        cJSON* rowArray = cJSON_CreateArray();
        for (int j = 0; j < board->width; j++) {
            cJSON* tile = cJSON_CreateObject();

            cJSON_AddNumberToObject(tile, "adjacentMinesNum", board->matrix[i][j].adjacentMinesNum);
            cJSON_AddNumberToObject(tile, "state", (int)board->matrix[i][j].state);
            cJSON_AddBoolToObject(tile, "bHasMine", board->matrix[i][j].bHasMine);

            cJSON_AddItemToArray(rowArray, tile);
        }
        cJSON_AddItemToArray(matrixArray, rowArray);
    }

    cJSON_AddNumberToObject(json, "width", board->width);
    cJSON_AddNumberToObject(json, "height", board->height);
    cJSON_AddNumberToObject(json, "minesNum", board->minesNum);
    cJSON_AddNumberToObject(json, "flagsPlacedNum", board->flagsPlacedNum);
    cJSON_AddNumberToObject(json, "tilesRevealedNum", board->tilesRevealedNum);
    cJSON_AddNumberToObject(json, "frameCounter", board->frameCounter);
    cJSON_AddNumberToObject(json, "timeCounter", board->timeCounter);

    char* jsonString = cJSON_Print(json);
    FILE* fileToSave = fopen("save.json", "w");
    if (fileToSave != NULL) {
        fprintf(fileToSave, "%s", jsonString);
        fclose(fileToSave);
        writeLog("Saved the game board info to save.json.");
    }
    else {
        writeLog("Could not save the game board info to save.json.");
    }

    cJSON_free(jsonString);
    cJSON_Delete(json);
}

bool loadGameFromJson(struct board* board) {
    FILE* fileToLoad = fopen("save.json", "r");
    if (fileToLoad == NULL) {
        writeLog("The save.json file was not found.");
        return false;
    }

    fseek(fileToLoad, 0, SEEK_END);
    long fileSize = ftell(fileToLoad);
    fseek(fileToLoad, 0, SEEK_SET);

    if (fileSize <= 0) {
        fclose(fileToLoad);
        writeLog("The save.json file is empty.");
        return false;
    }

    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        fclose(fileToLoad);
        return false;
    }

    fread(fileContent, 1, fileSize, fileToLoad);
    fileContent[fileSize] = '\0';
    fclose(fileToLoad);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        writeLog("Could not properly read the save.json file.");
        return false;
    }

    board->width = cJSON_GetObjectItem(json, "width")->valueint;
    board->height = cJSON_GetObjectItem(json, "height")->valueint;
    board->minesNum = cJSON_GetObjectItem(json, "minesNum")->valueint;
    board->flagsPlacedNum = cJSON_GetObjectItem(json, "flagsPlacedNum")->valueint;
    board->tilesRevealedNum = cJSON_GetObjectItem(json, "tilesRevealedNum")->valueint;
    board->frameCounter = cJSON_GetObjectItem(json, "frameCounter")->valueint;
    board->timeCounter = cJSON_GetObjectItem(json, "timeCounter")->valueint;

    cJSON* matrixArray = cJSON_GetObjectItem(json, "matrix");
    cJSON* rowArray = NULL;
    int i = 0;

    cJSON_ArrayForEach(rowArray, matrixArray) {
        cJSON* tileNode = NULL;
        int j = 0;

        cJSON_ArrayForEach(tileNode, rowArray) {
            board->matrix[i][j].adjacentMinesNum = cJSON_GetObjectItem(tileNode, "adjacentMinesNum")->valueint;
            board->matrix[i][j].state = (enum TileState)cJSON_GetObjectItem(tileNode, "state")->valueint;
            board->matrix[i][j].bHasMine = cJSON_IsTrue(cJSON_GetObjectItem(tileNode, "bHasMine"));
            j += 1;
        }
        i += 1;
    }

    cJSON_Delete(json);

    remove("save.json");
    writeLog("Loaded game board info from save.json and removed the file.");

    return true;
}
