// Copyright 2021 Kamenev Ilya
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "./evenodd_batcher_merge.h"

using std::vector;

TEST(radix_sort_double_batcher, test_task_1) {
  int numberOfProcess, currentRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

  if (log2(numberOfProcess) == (int)(log2(numberOfProcess))) {
    if (currentRank == 0) {
      vector<double> vectorOfValue;
      getRandomVector(&vectorOfValue, 128);

      double* rowData = vectorOfValue.data();
      RadixSortSequential(&rowData, 128);
      for (vector<double>::size_type counter = 0;
           counter < vectorOfValue.size() - 1; counter++) {
        ASSERT_TRUE(vectorOfValue[counter] <= vectorOfValue[counter + 1]);
      }
    }
  }
}

TEST(radix_sort_double_batcher, test_task_2) {
  int numberOfProcess, currentRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

  if (log2(numberOfProcess) == (int)(log2(numberOfProcess))) {
    vector<double> vectorOfValue;
    if (currentRank == 0) {
      getRandomVector(&vectorOfValue, 256);
    }

    vectorOfValue = RadixSortParallel(&vectorOfValue, 256);
    if (currentRank == 0) {
      for (vector<double>::size_type counter = 0;
           counter < vectorOfValue.size() - 1; counter++) {
        ASSERT_TRUE(vectorOfValue[counter] <= vectorOfValue[counter + 1]);
      }
    }
  }
}

TEST(radix_sort_double_batcher, test_task_3) {
  int numberOfProcess, currentRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

  if (log2(numberOfProcess) == (int)(log2(numberOfProcess))) {
    vector<double> vectorOfValue;
    if (currentRank == 0) {
      getRandomVector(&vectorOfValue, 512);
    }

    vectorOfValue = RadixSortParallel(&vectorOfValue, 512);
    if (currentRank == 0) {
      for (vector<double>::size_type counter = 0;
           counter < vectorOfValue.size() - 1; counter++) {
        ASSERT_TRUE(vectorOfValue[counter] <= vectorOfValue[counter + 1]);
      }
    }
  }
}

TEST(radix_sort_double_batcher, test_task_4) {
  int numberOfProcess, currentRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

  if (log2(numberOfProcess) == (int)(log2(numberOfProcess))) {
    vector<double> vectorOfValue;
    if (currentRank == 0) {
      getRandomVector(&vectorOfValue, 1024);
    }

    vectorOfValue = RadixSortParallel(&vectorOfValue, 1024);
    if (currentRank == 0) {
      for (vector<double>::size_type counter = 0;
           counter < vectorOfValue.size() - 1; counter++) {
        ASSERT_TRUE(vectorOfValue[counter] <= vectorOfValue[counter + 1]);
      }
    }
  }
}

TEST(radix_sort_double_batcher, test_task_5) {
  int numberOfProcess, currentRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

  if (log2(numberOfProcess) == (int)(log2(numberOfProcess))) {
    vector<double> vectorOfValue;
    if (currentRank == 0) {
      getRandomVector(&vectorOfValue, 2048);
    }

    vectorOfValue = RadixSortParallel(&vectorOfValue, 2048);
    if (currentRank == 0) {
      for (vector<double>::size_type counter = 0;
           counter < vectorOfValue.size() - 1; counter++) {
        ASSERT_TRUE(vectorOfValue[counter] <= vectorOfValue[counter + 1]);
      }
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
