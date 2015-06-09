#ifndef THRIFT_BASE_H
#define THRIFT_BASE_H

using namespace ::apache::thrift;  // NOLINT
using namespace ::apache::thrift::protocol;  // NOLINT
using namespace ::apache::thrift::transport;  // NOLINT
using namespace ::apache::thrift::server;  // NOLINT

namespace base {
  template <typename T>
    const std::string FromThriftToJson(const T *object) {
      boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
      scoped_ptr<TProtocol> protocol(new TJSONProtocol(membuffer));
      object->write(protocol.get());
      uint8* buffer = NULL;
      uint32 buffer_size = 0;
      membuffer->getBuffer(&buffer, &buffer_size);
      return std::string(reinterpret_cast<const char*>(buffer), buffer_size);
    }

  template <typename T>
    void FromJsonToThrift(const std::string &buffer, T *object) {
      boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer(
            const_cast<uint8*>(reinterpret_cast<const uint8*>(buffer.c_str())),
            buffer.size()));
      scoped_ptr<TProtocol> binaryprotocol(new TJSONProtocol(membuffer));
      T tmp_object;
      tmp_object.read(binaryprotocol.get());
      *object = tmp_object;
    }
}
#endif
