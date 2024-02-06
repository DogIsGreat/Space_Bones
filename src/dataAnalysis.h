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

typedef struct {
    Vector2 *items;
    Vector2 *centroid;
    float distance;
    size_t count;
    size_t capacity;
} Samples2D;

extern Samples2D set;
extern Samples2D clusters[K];
extern Vector2 means[K];

typedef struct {
    double x;
    double y;
    Vector2 distance;
} Sample;

typedef struct {
    char* item;
    size_t count;
    size_t capacity;
} String_Map;

double math_test(double x);

extern inline float rand_float(void);

extern void generate_cluster(Vector2 center, float radius, size_t count, Samples2D *samples);

void generate_new_state(float min_x, float max_x, float min_y, float max_y);

extern void recluster_state(void);


#endif