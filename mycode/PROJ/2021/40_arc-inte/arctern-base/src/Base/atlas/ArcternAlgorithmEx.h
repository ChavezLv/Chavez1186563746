//
// Created by Admin on 2020/6/2.
//

#ifndef ARCTERN_BASE_ARCTERNALGORITHMEX_H
#define ARCTERN_BASE_ARCTERNALGORITHMEX_H

#include "src/Base/algorithmbase.h"
#include "src/Engin/src/atlas/atlasengin.h"
#include "src/Processors/atlasAclPreprocess.h"
using namespace std;

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

        void SetEngines(std::vector<std::shared_ptr<AtlasEngine>> & engines);

        std::pair<int ,int> GetNetworkSize(){
            return std::make_pair(m_modelsInputWidth,m_modelsInputHeight);
        }
     public:
        virtual bool InitPreprocessor();

        virtual cv::Rect LocalGetROI(const RunParameter *p, int idx);

        virtual ErrorType CheckInitParam(const InitParameter *p);

        virtual  ErrorType CheckRunParam(const RunParameter *p);
        
        virtual ErrorType CommonPreProcess( const RunParameter *p,const int& idx,DvppDataInfo & decodeData);

        virtual int forward(const std::vector<std::pair<const void *, size_t>> &input, std::vector<std::vector<float>> &outputs,int batchSize) ;

     public:

        virtual ErrorType GetModelsConfigInfo(const TrtModels* models) = 0;

        virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,std::vector<std::vector<float>> &outputs) = 0;

        virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                          std::vector<DvppDataInfo>& vOutputInfos,
                                          std::vector<cv::Rect>& vRects) = 0;

    protected:
        int m_modelsInputHeight;
        int m_modelsInputWidth;
        
        shared_ptr<DvppDecodeManager> m_pDecodeMgr;

        vector<shared_ptr<AtlasEngine>> m_vEngines;

        shared_ptr<AtlasAclPreprocess> m_pPreprocessor;

        ///> algorithm(or inference) result
        Result* m_pResult ;
        ///> algorithm init parameter
        InitParameter m_initParam;

        ArcternImageFormat m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_BGR888;

        ErrorType m_lastErrorCode = ErrorType::ERR_SUCCESS;
    };
}

#endif //ARCTERN_BASE_ARCTERNALGORITHMEX_H
