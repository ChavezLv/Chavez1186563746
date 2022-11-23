//
// Created by Admin on 2021/5/16.
//

#ifndef ARCTERN_BASE_TRTFACEPHONEIMPL_H
#define ARCTERN_BASE_TRTFACEPHONEIMPL_H
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/facePhone/facePhone.h"
using namespace std;
namespace arctern{
    class TrtFacePhoneImpl: public ArcternAlgorithmEx{
    public:
        TrtFacePhoneImpl();

        virtual ~TrtFacePhoneImpl();




        virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

        virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                           int endId,vector<arctern::OutputTensor<float>> &outputs) override;

        virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

        virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

     private:

      pair<int,float> CalculateResult(const float *data, int step);


      std::array<float,4> m_extScale;
    };
}

#endif //ARCTERN_BASE_TRTFACEPHONEIMPL_H
