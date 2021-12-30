// Copyright 2021 Kamenev Ilya
#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

#include "../../../modules/task_3/kamenev_i_evenodd_batcher_merge/evenodd_batcher_merge.h"

using std::vector;

void makeCounters(double* arrayOfValue, uint64_t* arrayOfCounters,
                  uint64_t size) {
  memset(arrayOfCounters, 0, 256 * sizeof(double) * sizeof(uint64_t));

  uint8_t* startArrayOfValue = reinterpret_cast<uint8_t*>(arrayOfValue);
  uint8_t* endArrayOfValue = reinterpret_cast<uint8_t*>(arrayOfValue + size);

  while (startArrayOfValue != endArrayOfValue) {
    for (uint16_t counter = 0; counter < sizeof(double); counter++) {
      arrayOfCounters[256 * counter + *startArrayOfValue++] += 1;
    }
  }
}

void reinterpetToRadix(uint16_t offsetOfArray, uint64_t size,
                       double* pointerToSource, double* pointerToDestination,
                       uint64_t* nededCount) {
  double* nededPointer;
  uint64_t firstVar, secondVar;
  uint64_t* secondNededPointer;

  firstVar = 0;
  secondNededPointer = nededCount;
  for (uint64_t it = 256; it > 0; --it, ++secondNededPointer) {
    secondVar = *secondNededPointer;
    *secondNededPointer = firstVar;
    firstVar += secondVar;
  }

  uint8_t* bp = reinterpret_cast<uint8_t*>(pointerToSource) + offsetOfArray;
  nededPointer = pointerToSource;
  for (uint64_t it = size; it > 0; --it, bp += sizeof(double), ++nededPointer) {
    secondNededPointer = nededCount + *bp;
    pointerToDestination[*secondNededPointer] = *nededPointer;
    ++(*secondNededPointer);
  }
}

void reinterpetToDoubleRadix(uint16_t offsetOfArray, uint64_t size,
                             double* pointerToSource,
                             double* pointerToDestination,
                             uint64_t* nededCount) {
  double* nededPointer;
  uint64_t firstVar, secondVar;
  uint64_t* neded64Pointer;
  uint8_t* neded8Pointer;

  uint64_t negativeDouble = 0;
  for (uint64_t counter = 128; counter < 256; counter++) {
    negativeDouble += nededCount[counter];
  }

  firstVar = negativeDouble;
  neded64Pointer = nededCount;
  for (uint64_t counter = 0; counter < 128; ++counter, ++neded64Pointer) {
    secondVar = *neded64Pointer;
    *neded64Pointer = firstVar;
    firstVar += secondVar;
  }

  firstVar = nededCount[255] = 0;
  neded64Pointer = nededCount + 254;
  for (uint64_t counter = 254; counter >= 128; --counter, --neded64Pointer) {
    *neded64Pointer += firstVar;
    firstVar = *neded64Pointer;
  }

  neded8Pointer = reinterpret_cast<uint8_t*>(pointerToSource) + offsetOfArray;
  nededPointer = pointerToSource;
  for (uint64_t counter = size; counter > 0;
       --counter, neded8Pointer += sizeof(double), ++nededPointer) {
    neded64Pointer = nededCount + *neded8Pointer;
    if (*neded8Pointer < 128) {
      pointerToDestination[(*neded64Pointer)++] = *nededPointer;
    } else {
      pointerToDestination[--(*neded64Pointer)] = *nededPointer;
    }
  }
}

void simpleOddEvenMerge(vector<double>* arrayOfValue, int size, int left,
                        int right) {
  int main = 2 * right;

  if (main < size) {
    simpleOddEvenMerge(arrayOfValue, size, left, main);
    simpleOddEvenMerge(arrayOfValue, size, left + right, main);

    for (int counter = left + right; counter + right < left + size;
         counter += main) {
      if ((*arrayOfValue)[counter] >
          (*arrayOfValue)[static_cast<size_t>(counter + right)]) {
        std::swap((*arrayOfValue)[counter],
                  (*arrayOfValue)[static_cast<size_t>(counter + right)]);
      }
    }
  } else {
    if ((*arrayOfValue)[left] >
        (*arrayOfValue)[static_cast<size_t>(left + right)]) {
      std::swap((*arrayOfValue)[left],
                (*arrayOfValue)[static_cast<size_t>(left + right)]);
    }
  }
}

vector<double> batherMerge(vector<vector<double>> vectorOfVectorsValue) {
  while (vectorOfVectorsValue.size() != 1) {
    for (vector<vector<double>>::size_type counter = 0;
         counter < vectorOfVectorsValue.size() - 1; counter++) {
      vector<double> bufferVectorOfValue = vectorOfVectorsValue[counter];
      bufferVectorOfValue.insert(bufferVectorOfValue.end(),
                                 vectorOfVectorsValue[counter + 1].begin(),
                                 vectorOfVectorsValue[counter + 1].end());

      simpleOddEvenMerge(&bufferVectorOfValue, bufferVectorOfValue.size(), 0,
                         1);
      vectorOfVectorsValue[counter] = bufferVectorOfValue;
      vectorOfVectorsValue.erase(vectorOfVectorsValue.begin() + counter + 1);
    }
  }
  return vectorOfVectorsValue[0];
}

void getRandomVector(vector<double>* pointerToVector,
                     const vector<double>::size_type size) {
  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urd(100, 1000);

  pointerToVector->resize(size);
  for (vector<double>::size_type i = 0; i < size; i++) {
    pointerToVector->at(i) = urd(gen);
  }
}

void RadixSortSequential(double** pointerToVector,
                         const vector<double>::size_type size) {
  double* arrayOfOutput = new double[size];
  uint16_t counter;

  uint64_t* arrayOfCounters = new uint64_t[sizeof(double) * 256];
  uint64_t* currentCount;
  makeCounters(*pointerToVector, arrayOfCounters, size);

  for (counter = 0; counter < sizeof(double) - 1; counter++) {
    currentCount = arrayOfCounters + static_cast<size_t>(256 * counter);
    if (currentCount[0] == size) continue;
    reinterpetToRadix(counter, size, *pointerToVector, arrayOfOutput,
                      currentCount);
    std::swap(*pointerToVector, arrayOfOutput);
  }
  currentCount = arrayOfCounters + static_cast<size_t>(256 * counter);
  reinterpetToDoubleRadix(counter, size, *pointerToVector, arrayOfOutput,
                          currentCount);

  delete (*pointerToVector);
  (*pointerToVector) = arrayOfOutput;
  delete[] arrayOfCounters;
}

vector<double> RadixSortParallel(vector<double>* pointerToVector,
                                 const vector<double>::size_type size) {
  int numberOfProccess, currentRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProccess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

  int portionData = static_cast<int>(size) / numberOfProccess;
  vector<double> localVector(portionData), globalVector;

  MPI_Scatter(pointerToVector->data(), portionData, MPI_DOUBLE,
              localVector.data(), portionData, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  double* data = localVector.data();
  RadixSortSequential(&data, portionData);

  if (currentRank != 0) {
    MPI_Send(localVector.data(), portionData, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  } else {
    vector<vector<double>> lostVector;
    lostVector.push_back(localVector);

    for (int i = 1; i < numberOfProccess; ++i) {
      MPI_Status status;
      MPI_Recv(localVector.data(), portionData, MPI_DOUBLE, i, 1,
               MPI_COMM_WORLD, &status);
      lostVector.push_back(localVector);
    }

    globalVector = batherMerge(lostVector);
  }

  return globalVector;
}