//
// Created by Admin on 2021/5/16.
//

#ifndef ARCTERN_BASE_TRTFACEABNORMALLIGHTIMPL_H
#define ARCTERN_BASE_TRTFACEABNORMALLIGHTIMPL_H
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/faceAbnomalLight/faceAbnomalLight.h"
using namespace std;
namespace arctern{
    class TrtFaceAbnormalLightImpl:public ArcternAlgorithmEx{
    public:
        TrtFaceAbnormalLightImpl();

        virtual ~TrtFaceAbnormalLightImpl();

        virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

        virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                           int endId,vector<arctern::OutputTensor<float>> &outputs) override;

        virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

        virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

     private:
      ArcternRect check_shrink_box(const ArcternRect& box, int image_width,int image_height);

      pair<int,float> CalculateResult(const float *data, int step);
    };
}

#endif //ARCTERN_BASE_TRTFACEABNORMALLIGHTIMPL_H
