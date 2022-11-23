/* @ file fml_nn.cc
   @ breif ai engine for model-net process in fml layer.
   @ modify
     date: 2020/5/7
   author: wangchengbo
   content: add jpeg & video-codec task type in stream-mode.
*/
#ifndef FML_AI_ENGINE_EXT_H
#define FML_AI_ENGINE_EXT_H

#include "de_aiengine_node.h"

namespace de{

struct NnJpegRslt{
  uint32_t stream_id; ///< stream id used for multi-stream
  uint64_t pts;       ///< timestamp        
  std::vector<de::NDArray> tensors;
};

struct NnVideoRslt{
  uint32_t stream_id;  ///< stream id used for multi-stream
  uint32_t frame_id;
  uint64_t pts;       ///< timestamp       
  std::vector<de::NDArray> tensors;      
};

namespace serializer {
STRUCT_SERIALIZE_3(NnJpegRslt,
                   uint32_t, stream_id,
                   uint64_t, pts,
                   std::vector<de::NDArray>, tensors);
};

namespace serializer {
STRUCT_SERIALIZE_4(NnVideoRslt,
                   uint32_t,  stream_id,
                   uint32_t,  frame_id,
                   uint64_t,  pts,
                   std::vector<de::NDArray>, tensors);
};

class DE_DLL AiEngineExt : public AiEngine{
 public:
  AiEngineExt(bool stream_mode=true);
  void UpdatePara(void);
  void Stop(void) override;
  ~AiEngineExt();
};

}

#endif
