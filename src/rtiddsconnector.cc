#include "connector.h"
#include "reader.h"
#include "writer.h"
#include "samples.h"

extern "C" {

  using v8::FunctionTemplate;

  NAN_MODULE_INIT(InitAll) {

    dds::InitConnector(target);
    dds::InitReader(target);
    dds::InitWriter(target);
    dds::InitSamples(target);

  }

  NODE_MODULE(rtiddsconnector, InitAll);
}
