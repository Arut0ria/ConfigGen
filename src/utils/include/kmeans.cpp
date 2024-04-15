template <typename T>
Kmeans<T>::Kmeans(std::shared_ptr<std::vector<T>> data, unsigned int k,
                  dist_func function)
    : data(data), k(k), function(function), clusters(k, std::vector<T *>()) {
  this->centroids.reserve(k);
}

template <typename T> void Kmeans<T>::clear_clusters() {
  std::for_each(this->clusters.begin(), this->clusters.end(),
                [this](std::vector<T *> &vect) { vect.clear(); });
}

template <typename T> void Kmeans<T>::init_centroids() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, data->size() - 1);

  centroids.emplace_back(data->at(dis(gen)));
  assign_cluster(data->begin(), data->end(), centroids.size());

  while (centroids.size() < k) {
    unsigned int current_centroid = centroids.size() - 1;
    T *max_val = nullptr;
    double max_dist = 0.0;

    std::for_each(clusters[current_centroid].begin(),
                  clusters[current_centroid].end(), [&](T *val) {
                    if (this->function(
                            *val, this->centroids[current_centroid]) > max_dist)
                      max_val = val;
                  });

    centroids.emplace_back(*max_val);

    assign_clusters(centroids.size());
  }

  std::sort(centroids.begin(), centroids.end());
}

template <typename T> void Kmeans<T>::merge_cluster(cluster_type &clust) {
  std::lock_guard<std::mutex> lock(mutex_clusters);
  for (unsigned int i = 0; i < clust.size(); i++) {
    this->clusters[i].insert(this->clusters[i].end(), clust[i].begin(),
                             clust[i].end());
  }
}

template <typename T>
void Kmeans<T>::assign_cluster(std::vector<T>::iterator begin,
                               std::vector<T>::iterator end, unsigned int k) {
  cluster_type ret_cluster(k, std::vector<T *>());

  std::for_each(begin, end, [&](T &point) {
    double minDist = std::numeric_limits<double>::max();
    int closestCentroid = 0;
    for (unsigned int i = 0; i < k; i++) {
      double dist = this->function(point, centroids[i]);

      if (dist < minDist) {
        minDist = dist;
        closestCentroid = i;
      }
    }
    ret_cluster[closestCentroid].emplace_back(&point);
  });

  merge_cluster(ret_cluster);
  ret_cluster.clear();
}

template <typename T> void Kmeans<T>::assign_clusters(unsigned int k) {
  clear_clusters();

  const unsigned int num_threads = std::thread::hardware_concurrency();
  const unsigned int points_per_thread = this->data->size() / num_threads;
  const unsigned int left_over = this->data->size() % points_per_thread;

  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  auto begin = this->data->begin();

  for (unsigned int i = 0; i < num_threads; i++) {
    auto th_begin = begin + i * points_per_thread;
    auto th_end = th_begin + points_per_thread;
    threads.emplace_back(std::thread([this, th_begin, th_end, k]() {
      this->assign_cluster(th_begin, th_end, k);
    }));
  }

  if (left_over > 0)
    assign_cluster(this->data->begin() + points_per_thread * num_threads,
                   this->data->end(), k);

  std::for_each(threads.begin(), threads.end(),
                [](std::thread &th) { th.join(); });
  threads.clear();
}

template <typename T> void Kmeans<T>::update_centroids() {
  centroids.clear();

  std::for_each(clusters.begin(), clusters.end(), [&](std::vector<T *> &clust) {
    T new_centroid;
    std::for_each(clust.begin(), clust.end(),
                  [&](T *point) { new_centroid += *point; });
    new_centroid /= clust.size();
    centroids.emplace_back(new_centroid);
  });
  std::sort(centroids.begin(), centroids.end());
}

template <typename T> std::vector<T> Kmeans<T>::get_result() {
  init_centroids();
  std::vector<T> prev_centroids = centroids;

  while (true) {
    assign_clusters(this->k);
    update_centroids();

    if (prev_centroids == centroids)
      break;

    prev_centroids = centroids;
  }

  return prev_centroids;
}