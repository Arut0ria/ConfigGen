#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#include "hsl.hpp"
#include "timer.hpp"

template <typename T> class Kmeans {
public:
  typedef double (*dist_func)(const T &, const T &);
  typedef std::vector<std::vector<T *>> cluster_type;

  Kmeans(std::shared_ptr<std::vector<T>> data, unsigned int k,
         dist_func function);
  ~Kmeans(){};

  std::vector<T> get_result();

private:
  std::shared_ptr<std::vector<T>> data;
  unsigned int k;
  dist_func function;
  cluster_type clusters;
  
  std::vector<T> centroids;
  std::mutex mutex_clusters;

  void clear_clusters();

  void init_centroids();
  void assign_clusters(unsigned int k);
  void assign_cluster(std::vector<T>::iterator begin,
                      std::vector<T>::iterator end, unsigned int k);
  void merge_cluster(cluster_type &clust);
  void update_centroids();
};

#include "kmeans.cpp"

#endif