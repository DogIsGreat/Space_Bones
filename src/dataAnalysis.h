#ifndef __dataanalysis_h__
#define __dataanalysis_h__

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include <stdlib.h>

#include <math.h>
#include <float.h>
#include <limits.h>

#define KMEANS_NULL_CLUSTER -1
#define KMEANS_MAX_ITERATIONS 1000

#ifdef KMEANS_THREADED
#define KMEANS_THR_MAX 4
#define KMEANS_THR_THRESHOLD 250000
#endif

#define kmeans_malloc(size) malloc(size)
#define kmeans_free(ptr) free(ptr)

#define K 5
#define SAMPLE_RADIUS 4.0f
#define MEAN_RADIUS (3*SAMPLE_RADIUS)

typedef void * Pointer;

typedef enum {
    KMEANS_OK,
    KMEANS_EXCEEDED_MAX_ITERATIONS,
    KMEANS_ERROR,
} kmeans_result;


double math_test(double x);

extern inline float rand_float(void);

double calculate_distance(Vector2 a, Vector2 b);

void assign_clusters(Vector2 *data, int data_size, Vector2 *centroids, int k, int *assignments);

void update_centroids(Vector2 *data, int data_size, Vector2 *centroids, int k, int *assignments);

#endif