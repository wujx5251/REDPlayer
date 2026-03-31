#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>

// ---------------------------------------------------------------------------
// Portable declarations for NdkMediaCodec / NdkMediaFormat types and
// constants. We load libmediandk.so at runtime (dlopen) so that the SO has
// no DT_NEEDED entry for it and can therefore load on Android < API 21.
// On API 21+ devices dlopen succeeds and hardware MediaCodec decode works.
// ---------------------------------------------------------------------------

typedef struct AMediaCodec AMediaCodec;
typedef struct AMediaFormat AMediaFormat;
typedef int32_t media_status_t;

enum { AMEDIA_OK = 0 };

enum {
  AMEDIACODEC_INFO_TRY_AGAIN_LATER        = -1,
  AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED  = -2,
  AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED = -3,
};

enum { AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM = 4 };

typedef struct {
  int32_t  offset;
  int32_t  size;
  int64_t  presentationTimeUs;
  uint32_t flags;
} AMediaCodecBufferInfo;

#define AMEDIAFORMAT_KEY_MIME           "mime"
#define AMEDIAFORMAT_KEY_WIDTH          "width"
#define AMEDIAFORMAT_KEY_HEIGHT         "height"
#define AMEDIAFORMAT_KEY_MAX_INPUT_SIZE "max-input-size"

namespace amedia {

// Attempt to dlopen libmediandk.so and resolve all required symbols.
// Returns true (and caches the result) on API 21+ devices.
// Safe to call multiple times; loads only once.
bool load();
bool is_available();

extern AMediaCodec *  (*createDecoderByType)(const char *mimeType);
extern void           (*codec_delete)(AMediaCodec *);
extern media_status_t (*configure)(AMediaCodec *, const AMediaFormat *,
                                   void *surface, void *crypto,
                                   uint32_t flags);
extern media_status_t (*start)(AMediaCodec *);
extern media_status_t (*stop)(AMediaCodec *);
extern media_status_t (*flush)(AMediaCodec *);
extern ssize_t        (*dequeueInputBuffer)(AMediaCodec *, int64_t timeoutUs);
extern uint8_t *      (*getInputBuffer)(AMediaCodec *, size_t idx,
                                        size_t *out_size);
extern media_status_t (*queueInputBuffer)(AMediaCodec *, size_t idx,
                                          ssize_t offset, size_t size,
                                          uint64_t time, uint32_t flags);
extern ssize_t        (*dequeueOutputBuffer)(AMediaCodec *,
                                             AMediaCodecBufferInfo *info,
                                             int64_t timeoutUs);
extern media_status_t (*releaseOutputBuffer)(AMediaCodec *, size_t idx,
                                             bool render);
extern AMediaFormat * (*getOutputFormat)(AMediaCodec *);

extern AMediaFormat * (*format_new)();
extern void           (*format_delete)(AMediaFormat *);
extern void           (*format_setString)(AMediaFormat *, const char *name,
                                          const char *value);
extern void           (*format_setInt32)(AMediaFormat *, const char *name,
                                         int32_t value);
extern void           (*format_setBuffer)(AMediaFormat *, const char *name,
                                          void *data, size_t size);
extern const char *   (*format_toString)(AMediaFormat *);

} // namespace amedia
