/*
Currently the program loads all data into memory and this may not be necessary.
*/

// Original Work
#include "dataAnalysis.h"

// Open Source Headers (statically compiled and linked)
#include "stb_ds.h"
#include "dbg.h"

// Open Source Headers (non-local and/or dynamically linked)
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include <math.h>
#include <time.h>
#include <float.h>
#include <limits.h>

#define MAX_NODES 100000

static Color colors[] = {
        CLITERAL(Color){255, 203, 0, 255},   // Gold
        CLITERAL(Color){255, 105, 180, 255}, // Pink
        CLITERAL(Color){50, 205, 50, 255},   // Lime
        CLITERAL(Color){135, 206, 235, 255}, // Skyblue
        CLITERAL(Color){255, 165, 0, 255},   // Orange
        CLITERAL(Color){255, 255, 0, 255},   // Yellow
        CLITERAL(Color){255, 0, 0, 255},    //Red
};

//#define colors_count (sizeof(colors) / sizeof(colors[0]))
//#define labels_count (sizeof(labels) / sizeof(labels))

int main()
{

    if (init_logging("debug.log")!= 0){
        return 1;
    }

    log_info(" STELLAR Mode initialized. ");
    float min_x = FLT_MAX;
    float max_x = FLT_MIN;
    float min_y = FLT_MAX;
    float max_y = FLT_MIN;

    //open data file
    FILE *spreadsheet;
    const char *filename = "data/random2.csv";
    spreadsheet = fopen(filename, "r");
    Vector2* p = (Vector2*)malloc(MAX_NODES * sizeof(Vector2));
    Vector2* means = (Vector2*)malloc(K * sizeof(Vector2));
    int* clusters = (int*)malloc(MAX_NODES * sizeof(int));
    int row = 1;

    if (!spreadsheet){
        log_err("Spreadsheet failed to open.");
        return -1;
    } else {
        log_info(" Pre-processing Stage. ");
        char buffer[1024];
        row = 1;
        int column = 0;

        while (fgets(buffer, 1024,spreadsheet) != NULL){
            if (row == 0) {
                row++;
                continue;
            }
            log_value(" Buffer = %s",buffer);

            // Allocate memory for new node
            p[row - 1].x = 0.0;
            p[row - 1].y = 0.0;

            //break rows into values
            char* value = strtok(buffer, ",");

            if(!value){
                log_err(" String Tokenization failure at row %d ", row);
            } else {
            log_value("Value of row:column =  %d:%d = %s ", row, column, value);
            }

            while(value != NULL){

                switch(column){
                    case 0:
                        p[row -1].x = atof(value);
                        column++;
                        break;
                    case 1:
                        p[row -1].y = atof(value);
                        if (p[row -1].x < min_x) min_x = p[row -1].x;
                        if (p[row -1].x < max_x) max_x = p[row -1].x;
                        if (p[row -1].y < min_y) min_y = p[row -1].y;
                        if (p[row -1].y < max_y) max_y = p[row -1].y;
                        column++;
                        break;
                    default:
                        column++;
                        break;
                }
                value = strtok(NULL, ",");
            }

            log_value(" p.x:p.y: column:value %d:%d: %d:%d ",p[row -1].x, p[row -1].y, column,value);
            column = 0;
            row++;
        }
    }

    log_info(" Visualization Pre-Stage. ");
    log_info("x = %f..%f", min_x, max_x);
    log_info("y = %f..%f", min_y, max_y);

    const int screenWidth = 800;
    const int screenHeight = 450;
    int colors_count = (sizeof(colors) / sizeof(colors[0]));
    size_t data_counter = 0;
    size_t data_place = 0;

    srand(time(0));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "K-means data visualization of Observable Stellar Attributes");

    data_counter = row;

    assign_clusters(p, data_counter, means ,K, clusters);
    update_centroids(p, data_counter, means, K, clusters);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    log_info("All Raylib Config settings successfully initialized");

    while (!WindowShouldClose()){

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){

            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);

            camera.target = Vector2Add(camera.target, delta);

        }

        // if button click space bar recalculate..

        float wheel = GetMouseWheelMove();
        if (wheel != 0)
        {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            camera.offset = GetMousePosition();

            camera.target = mouseWorldPos;

            const float zoomIncrement = 0.125f;

            camera.zoom += (wheel*zoomIncrement);
            if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
        }

        BeginDrawing();
            ClearBackground(WHITE);

            BeginMode2D(camera);

                rlPushMatrix();
                    rlTranslatef(0, 25*50, 0);
                    rlRotatef(90, 1, 0, 0);
                    DrawGrid(100, 50);
                rlPopMatrix();

                //DrawCircle(100, 100, 50, GREEN);

                for (size_t i = 0; i < K; ++i){
                    Color color = colors[i%colors_count];

                // correct This for assignment number...
                    for (size_t j= data_place; j < data_counter; ++j){
                        Vector2 it = {0};
                        it.x = p[j].x;
                        it.y = p[j].y;
                        DrawCircleV(it, SAMPLE_RADIUS/camera.zoom, color);
                        if (clusters[j] != clusters[j+1]){
                            data_place = j;
                            break;
                        }
                    }
                    DrawCircleV(means[i], 2*SAMPLE_RADIUS/camera.zoom, color);
                }

            EndMode2D();

        EndDrawing();

    }

    CloseWindow();
    log_info("Program successfully ending. ");
    free(clusters);
    free(means);
    free(p);
    fclose(spreadsheet);
    close_logging();

    return 0;
}