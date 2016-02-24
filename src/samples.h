#ifndef RTIDDS_SAMPLES_H
#define RTIDDS_SAMPLES_H

#include <nan.h>

namespace dds {

  NAN_METHOD(get_samples_length);
  NAN_METHOD(get_number_from_samples);
  NAN_METHOD(get_boolean_from_samples);
  NAN_METHOD(get_string_from_samples);
  NAN_METHOD(get_json_sample);

  NAN_METHOD(set_number_into_samples);
  NAN_METHOD(set_string_into_samples);
  NAN_METHOD(set_boolean_into_samples);

  NAN_METHOD(get_infos_length);
  NAN_METHOD(get_boolean_from_infos);

  NAN_MODULE_INIT(InitSamples);

}

#endif
