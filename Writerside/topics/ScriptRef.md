# ScriptRef

## ExternalData.h

```C++
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
#pragma once
#ifndef SINGLETON_DATA_RECEIVER_H
#define SINGLETON_DATA_RECEIVER_H

#include <array>
#include <mutex>

/**
 * \brief Singleton data receiver
 */
class ExternalData {
public:
  static ExternalData &get_instance();

  void set_value(const float v0, const float v1, const float v2, const float v3, const float v4, const float v5, const float v6, const float v7, const float v8, const float v9, const float v10, const float v11, const float v12, const float v13, const float v14, const float v15, const float v16, const float v17, const float v18, const float v19, const float v20, const float v21, const float v22, const float v23, const float v24, const float v25, const float v26, const float v27, const float v28, const float v29, const float v30, const float v31, const float v32, const float v33, const float v34, const float v35, const float v36, const float v37, const float v38, const float v39);

  void update_data(float value, size_t index);

  float get_data(size_t index) const;

  ExternalData(ExternalData const &);

private:
  ExternalData();

  mutable std::mutex mutex_;
  std::array<float,40> data_;
};

inline ExternalData &ExternalData::get_instance() {
  static ExternalData instance;
  return instance;
}

inline void ExternalData::set_value(const float v0, const float v1, const float v2, const float v3, const float v4, const float v5, const float v6, const float v7, const float v8, const float v9, const float v10, const float v11, const float v12, const float v13, const float v14, const float v15, const float v16, const float v17, const float v18, const float v19, const float v20, const float v21, const float v22, const float v23, const float v24, const float v25, const float v26, const float v27, const float v28, const float v29, const float v30, const float v31, const float v32, const float v33, const float v34, const float v35, const float v36, const float v37, const float v38, const float v39) {
  //std::lock_guard<std::mutex> lock(mutex_);
  data_[0] = v0;
  data_[1] = v1;
  data_[2] = v2;
  data_[3] = v3;
  data_[4] = v4;
  data_[5] = v5;
  data_[6] = v6;
  data_[7] = v7;
  data_[8] = v8;
  data_[9] = v9;
  data_[10] = v10;
  data_[11] = v11;
  data_[12] = v12;
  data_[13] = v13;
  data_[14] = v14;
  data_[15] = v15;
  data_[16] = v16;
  data_[17] = v17;
  data_[18] = v18;
  data_[19] = v19;
  data_[20] = v20;
  data_[21] = v21;
  data_[22] = v22;
  data_[23] = v23;
  data_[24] = v24;
  data_[25] = v25;
  data_[26] = v26;
  data_[27] = v27;
  data_[28] = v28;
  data_[29] = v29;
  data_[30] = v30;
  data_[31] = v31;
  data_[32] = v32;
  data_[33] = v33;
  data_[34] = v34;
  data_[35] = v35;
  data_[36] = v36;
  data_[37] = v37;
  data_[38] = v38;
  data_[39] = v39;
}

inline void ExternalData::update_data(float value, size_t index) {
  //std::lock_guard<std::mutex> lock(mutex_);
  if (index < data_.size()) { data_[index] = value; }
}

inline float ExternalData::get_data(size_t index) const {
  //std::lock_guard<std::mutex> lock(mutex_);
  //if (index < data_.size()) { return data_[index]; }
  return data_[index];
}

inline ExternalData::ExternalData(ExternalData const &)
  : data_() {}

inline ExternalData::ExternalData()
  : data_() {}
#endif// SINGLETON_DATA_RECEIVER_H

```
{collapsible="true" collapsed-title="ExternalData.h"}