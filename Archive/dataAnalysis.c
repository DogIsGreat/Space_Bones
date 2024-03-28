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

//Samples2D set = {0};
//Samples2D clusters[K] = {0};
//Vector2 means[K]= {0};
//vector2 clusters[K]={0};
//Vector2 means[K]={0};

inline float rand_float(void){
    return (float)rand()/RAND_MAX;
}

/*
This function is just a distance calculating function and does not use gsl, just math library.
*/
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

/*
This version of  function calculates the distance and builds the samples cluster struct.
It does not use GSL.
This function is an alternative to using both the calculate_distance and assign_clusters function.
*/

/*
void generate_cluster(Vector2 center, float radius, size_t count, Samples2D *samples){
    for (size_t i = 0; i < count; ++i){
            float angle = rand_float()*2*PI;
            float mag = rand_float()*radius;
            Vector2 original  = {
                .x = cosf(angle)*mag,
                .y = sinf(angle)*mag,
            };
            //New code process.
            Sample data = {
                .x = original.x,
                .y = original.y,
                .distance = Vector2Add(original, center),
            };
    }
}


//Remember that now Samples2D has extra data types.
void generate_new_state(float min_x, float max_x, float min_y, float max_y){
    #ifndef STELLAR
    set.count = 0;
        generate_cluster(CLITERAL(Vector2){0},10,100, &set);
        generate_cluster(CLITERAL(Vector2){min_x*0.5f, max_y*0.5f}, 5,50, &set);
        generate_cluster(CLITERAL(Vector2){max_x*0.5f, max_y*0.5f},10,100, &set);
        generate_cluster(CLITERAL(Vector2){min_x*0.5f, min_y*0.5f},10,100, &set);
        generate_cluster(CLITERAL(Vector2){max_x*0.5f, min_y*0.5f},10,100, &set);
        log_info("New cluster successfully generated.");
    #endif

    #ifndef RANDO
        for (size_t i = 0; i < K; ++i){
            means[i].x = Lerp(min_x, max_x, rand_float());
            means[i].y = Lerp(min_y, max_y, rand_float());
        }
        log_info("Random cluster generation run.");
    #endif
}

//check if passing cluster is nessessary?
void recluster_state(void){
    for (size_t j = 0; j < K; ++j){
        clusters[j].count = 0;
    }
    for (size_t i = 0; i < set.count; ++i){
        Vector2 p = set.items[i];
        int k = -1;
        float s = FLT_MAX;
        for (size_t j = 0; j < K; ++j){
            Vector2 m = means[j];
            float sm = Vector2LengthSqr(Vector2Subtract(p, m));
            if (sm < s){
                s = sm;
                k = j;
            }
        }
        //update the currect position in cluster.
        //cluster->item = p;
    }
}

void update_means(float min_x, float max_x, float min_y, float max_y){
    for (size_t i = 0; i < K; ++i){
        if (clusters[i].count > 0){
            means[i] = Vector2Zero();
            for (size_t j = 0; j < clusters[i].count; ++j){
                means[i] = Vector2Add(means[i], clusters[i].items[j]);
            }
            means[i].x /= clusters[i].count;
            means[i].y /= clusters[i].count;
        } else {
            means[i].x = Lerp(min_x, max_x, rand_float());
            means[i].y = Lerp(min_x, max_y, rand_float());
        }
    }
}
*/