#include "mediacodec_loader.h"

#include <dlfcn.h>

namespace amedia {

AMediaCodec *  (*createDecoderByType)(const char *)                        = nullptr;
void           (*codec_delete)(AMediaCodec *)                              = nullptr;
media_status_t (*configure)(AMediaCodec *, const AMediaFormat *, void *,
                             void *, uint32_t)                             = nullptr;
media_status_t (*start)(AMediaCodec *)                                     = nullptr;
media_status_t (*stop)(AMediaCodec *)                                      = nullptr;
media_status_t (*flush)(AMediaCodec *)                                     = nullptr;
ssize_t        (*dequeueInputBuffer)(AMediaCodec *, int64_t)               = nullptr;
uint8_t *      (*getInputBuffer)(AMediaCodec *, size_t, size_t *)          = nullptr;
media_status_t (*queueInputBuffer)(AMediaCodec *, size_t, ssize_t, size_t,
                                   uint64_t, uint32_t)                    = nullptr;
ssize_t        (*dequeueOutputBuffer)(AMediaCodec *, AMediaCodecBufferInfo *,
                                      int64_t)                            = nullptr;
media_status_t (*releaseOutputBuffer)(AMediaCodec *, size_t, bool)        = nullptr;
AMediaFormat * (*getOutputFormat)(AMediaCodec *)                           = nullptr;

AMediaFormat * (*format_new)()                                             = nullptr;
void           (*format_delete)(AMediaFormat *)                            = nullptr;
void           (*format_setString)(AMediaFormat *, const char *,
                                   const char *)                           = nullptr;
void           (*format_setInt32)(AMediaFormat *, const char *,
                                  int32_t)                                 = nullptr;
void           (*format_setBuffer)(AMediaFormat *, const char *, void *,
                                   size_t)                                 = nullptr;
const char *   (*format_toString)(AMediaFormat *)                          = nullptr;

static bool s_available = false;
static bool s_loaded    = false;

#define LOAD_SYM(handle, fp, sym)                                              \
  do {                                                                         \
    fp = reinterpret_cast<decltype(fp)>(dlsym(handle, sym));                   \
    if (!(fp)) {                                                               \
      dlclose(handle);                                                         \
      return false;                                                            \
    }                                                                          \
  } while (0)

bool load() {
  if (s_loaded) return s_available;
  s_loaded = true;

  void *handle = dlopen("libmediandk.so", RTLD_LAZY | RTLD_LOCAL);
  if (!handle) {
    return false;
  }

  LOAD_SYM(handle, createDecoderByType, "AMediaCodec_createDecoderByType");
  LOAD_SYM(handle, codec_delete,        "AMediaCodec_delete");
  LOAD_SYM(handle, configure,           "AMediaCodec_configure");
  LOAD_SYM(handle, start,               "AMediaCodec_start");
  LOAD_SYM(handle, stop,                "AMediaCodec_stop");
  LOAD_SYM(handle, flush,               "AMediaCodec_flush");
  LOAD_SYM(handle, dequeueInputBuffer,  "AMediaCodec_dequeueInputBuffer");
  LOAD_SYM(handle, getInputBuffer,      "AMediaCodec_getInputBuffer");
  LOAD_SYM(handle, queueInputBuffer,    "AMediaCodec_queueInputBuffer");
  LOAD_SYM(handle, dequeueOutputBuffer, "AMediaCodec_dequeueOutputBuffer");
  LOAD_SYM(handle, releaseOutputBuffer, "AMediaCodec_releaseOutputBuffer");
  LOAD_SYM(handle, getOutputFormat,     "AMediaCodec_getOutputFormat");
  LOAD_SYM(handle, format_new,          "AMediaFormat_new");
  LOAD_SYM(handle, format_delete,       "AMediaFormat_delete");
  LOAD_SYM(handle, format_setString,    "AMediaFormat_setString");
  LOAD_SYM(handle, format_setInt32,     "AMediaFormat_setInt32");
  LOAD_SYM(handle, format_setBuffer,    "AMediaFormat_setBuffer");
  LOAD_SYM(handle, format_toString,     "AMediaFormat_toString");

  s_available = true;
  return true;
}

bool is_available() {
  if (!s_loaded) load();
  return s_available;
}

} // namespace amedia
