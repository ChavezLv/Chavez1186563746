//
// Created by Admin on 2020/6/2.
//

#ifndef ARCTERN_BASE_ARCTERNALGORITHMEX_H
#define ARCTERN_BASE_ARCTERNALGORITHMEX_H

#include "src/common/arcterndefs_generated.h"
#include "src/Base/arcternalgorithm.h"
#include "src/Base/algorithmbase.h"
#include "src/Processors/NppPreprocessorEx.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/Engin/src/mxnet/MxnetEngineManager.h"
#include "src/Engin/src/mxnet/mxnetengin.h"
#include "src/common/imageConvert.h"
#include "src/common/testUtil.h"
#include "src/common/tensor.h"
#include "src/Algorithms/faceAbnomalLight/faceAbnomalLight.h"
#include "src/Engin/src/trt/trtEngin.h"
#include "src/Engin/src/trt/trtEngineManager.h"
using namespace std;
using namespace ImgPreprocessEx;

namespace arctern{
    class ArcternAlgorithmEx: public AlgorithmBase{
    public:
        ArcternAlgorithmEx();
        ~ArcternAlgorithmEx() override;

        ArcternAlgorithmEx(const ArcternAlgorithmEx&) = delete;
        ArcternAlgorithmEx(ArcternAlgorithmEx&&) = delete;
        ArcternAlgorithmEx &operator=(const ArcternAlgorithmEx&) = delete;
        ArcternAlgorithmEx &operator=(ArcternAlgorithmEx&&) = delete;

        virtual ErrorType Init(const InitParameter *p) override;
        virtual void Release() override;

        virtual ErrorType Process(const RunParameter *p,Result *r) override;


     public:
        ErrorType GetLastError();

        void SetEngines(std::vector<std::shared_ptr<TrtEngin>> & engines);

        void ArcternImage2DeviceBuffer(const ArcternImage& arctern_image,DeviceBuffer &device_buffer);
     public:

        virtual ErrorType CheckInitParam(const InitParameter *p);

        virtual void InitPreprocessorParam();

        virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm);

        virtual  ErrorType CheckRunParam(const RunParameter *p);

        virtual ErrorType CommonPreProcess(const RunParameter *p);

        virtual ArcternRect LocalGetROI(const RunParameter *p, int idx);

        virtual ErrorType LocalPreProcess( const RunParameter *p,const int& idx);

        virtual ErrorType CommonProProcess(const RunParameter *p);

        virtual int forward(const std::vector<std::pair<const void *, size_t>> &input, std::vector<arctern::OutputTensor<float> > &outputs) ;

     public:

        virtual ErrorType GetModelsConfigInfo(const MxnetModels* models) = 0;

        virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,vector<arctern::OutputTensor<float>> &outputs) = 0;

    protected:
        int m_modelsInputHeight;
        int m_modelsInputWidth;

        vector<shared_ptr<TrtEngin>> m_vEngines;

        ///> images preprocessor
        unique_ptr<NppPreprocessor> m_pPreprocessor;

        ///> images preprocessor result
        vector<unique_ptr<NppPreprocessorRet>> m_vPreprocessRet;

        ///> algorithm(or inference) result
        Result* m_pResult ;
        ///> algorithm init parameter
        InitParameter m_initParam;

        ArcternImageFormat m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_BGR888;

        ErrorType m_lastErrorCode = ErrorType::ERR_SUCCESS;

        ///< network shape ,N: batch size,C: image channels,H: image height,W: image width
        NetworkShape m_netShape = NetworkShape::NCHW;

        cudaStream_t *m_pStream = nullptr;
    };
}

#endif //ARCTERN_BASE_ARCTERNALGORITHMEX_H
