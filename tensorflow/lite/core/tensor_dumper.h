#ifndef TENSORFLOW_LITE_CORE_TENSOR_DUMPER_
#define TENSORFLOW_LITE_CORE_TENSOR_DUMPER_

#include <iostream>

#include "tensorflow/lite/c/common.h"

namespace {

const int getTensorSize(const TfLiteTensor* tensor) {
  int size = 1;
  for (int i = 0; i < tensor->dims->size; ++i) {
    size *= tensor->dims->data[i];
  }
  return size;
}

void printTensorName(const TfLiteTensor* tensor) { std::cout << tensor->name; }

void printTensorDims(const TfLiteTensor* tensor) {
  std::cout << "( ";
  const TfLiteIntArray* dims = tensor->dims;
  for (int i = 0; i < dims->size; ++i) {
    std::cout << dims->data[i] << " ";
  }
  std::cout << ")";
}

template <class T>
void printTypedT(const TfLiteTensor* tensor) {
  std::cout << std::endl;
  printTensorName(tensor);
  std::cout << ": ";
  printTensorDims(tensor);
  std::cout << std::endl;
  // printTensorData
  const int tensor_size = getTensorSize(tensor);
  float tensor_sum = 0;
  const T* tensor_data = reinterpret_cast<T*>(tensor->data.raw);
  for (int i = 0; i < tensor_size; ++i) {
    tensor_sum += (float)tensor_data[i];
    if (i < 100) {
      std::cout << (float)tensor_data[i] << " ";
      if ((i + 1) % tensor->dims->data[tensor->dims->size - 1] == 0)
        std::cout << std::endl;
    }
  }
  std::cout << std::endl;
  std::cout << "...sum: " << tensor_sum << std::endl;
}

void printT(const TfLiteTensor* tensor) {
  switch (tensor->type) {
    case kTfLiteUInt8:
      printTypedT<uint8_t>(tensor);
      break;
    case kTfLiteInt32:
      printTypedT<int>(tensor);
      break;
    case kTfLiteFloat32:
      printTypedT<float>(tensor);
      break;
    default:
      break;
  }
}

}  // namespace
#endif  // TENSORFLOW_LITE_CORE_TENSOR_DUMPER_