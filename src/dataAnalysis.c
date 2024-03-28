/**
For the time being both GSL and non-GSL functions will be added for testing, optimization, and educational purposes.
*/

#include "dbg.h"
#include "dataAnalysis.h"

#include <stdio.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include <math.h>
#include <float.h>
#include <limits.h>


inline float rand_float(void){
    return (float)rand()/RAND_MAX;
}


double calculate_distance(Vector2 a,  Vector2 b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void assign_clusters(Vector2 *data, int data_size, Vector2 *centroids, int k, int *assignments){
    for (int i = 0; i < data_size; i++){
        double min_distance = calculate_distance(data[i], centroids[0]);
        int cluster = 0;
        for (int j = 1; j < k; j++){
            double distance = calculate_distance(data[i], centroids[j]);
            if (distance < min_distance){
                min_distance = distance;
                cluster = j;
            }
        }
        assignments[i] = cluster;
    }
}

void update_centroids(Vector2 *data, int data_size, Vector2 *centroids, int k, int *assignments){
    // Reset Centroids
    for (int i = 0; i < k; i++){
        centroids[i].x = 0;
        centroids[i].y = 0;
    }

    // Sum up all points assigned to each cluster
    int counts[k];
    for (int i = 0; i < k; i++) counts[i] = 0;
    for (int i = 0; i < data_size; i++){
        centroids[assignments[i]].x += data[i].x;
        centroids[assignments[i]].y += data[i].y;
        counts[assignments[i]]++;
    }

}

