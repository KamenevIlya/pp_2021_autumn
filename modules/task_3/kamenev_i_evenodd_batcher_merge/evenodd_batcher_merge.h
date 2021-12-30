// Copyright 2021 Kamenev Ilya
#ifndef MODULES_TASK_3_KAMENEV_I_EVENODD_BATCHER_MERGE_H_
#define MODULES_TASK_3_KAMENEV_I_EVENODD_BATCHER_MERGE_H_

#include <vector>

void getRandomVector(std::vector<double>* pointerToVector,
                     const std::vector<double>::size_type size);
void RadixSortSequential(double** pointerToVector,
                         const std::vector<double>::size_type size);
std::vector<double> RadixSortParallel(
    std::vector<double>* pointerToVector,
    const std::vector<double>::size_type size);

#endif  // MODULES_TASK_3_KAMENEV_I_EVENODD_BATCHER_MERGE_H_
