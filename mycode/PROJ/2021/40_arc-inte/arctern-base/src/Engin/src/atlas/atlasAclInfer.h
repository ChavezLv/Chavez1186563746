
#ifndef ARCTERN_ATLASACLINFER_H
#define ARCTERN_ATLASACLINFER_H
#include "src/common/file_buffer.h"
#include "src/common/arcterndefs_generated.h"
#include "acl/acl.h"
#include "src/common/atlas/atlasLog.h"
#include "src/common/atlas/errorCode.h"
#include <memory>
#include "src/common/ReadWriteLock.h"
#include "src/common/ThreadSafeMap.h"
#include <map>
namespace arctern {
    class AtlasAclModel;
    class AtlasAclModelManager{
    public:
        static AtlasAclModelManager * getInstance();

        bool init(const FileBuffer &file_buffer,const std::string &key,int deviceId);

        AtlasAclModel* getAclModel(std::string key);

        const aclrtContext & getAclContext(std::string key);
    private:
        static AtlasAclModelManager *m_aclModelManager;
        //static aclrtContext m_aclContext;
        AtlasAclModelManager();
        AtlasAclModelManager(const AtlasAclModelManager&);
        AtlasAclModelManager(AtlasAclModelManager &&);
        AtlasAclModelManager & operator=(const AtlasAclModelManager&);
        AtlasAclModelManager & operator=(AtlasAclModelManager&&);
        ~AtlasAclModelManager();
        std::map<std::string,AtlasAclModel *> m_map;
        std::map<std::string,aclrtContext> m_map_context;
    };

    class AtlasAclModel{
    public:
        AtlasAclModel(aclrtContext context);
        friend class AtlasAclModelManager;
    private:
        bool init(const FileBuffer &file_buffer);
        void destroy();
        ~AtlasAclModel();
        aclrtContext context_ = nullptr;
    public:
        size_t modelDevPtrSize_ = 0;
        size_t weightDevPtrSize_ = 0;
        
        void *weightDevPtr_ = nullptr;
       
        ///> host model from file by flatbuffer
        int mode_size_ = 0;
        void* pdata_ = nullptr;
  
    };

    class AtlasAclInfer {
    public:
        AtlasAclInfer(aclrtContext context);

        ~AtlasAclInfer();

        bool init(uint32_t modelId);

        int infer(std::vector<void *> &inputBufs, std::vector <size_t> &inputSizes,
                  std::vector<void *> &ouputBufs, std::vector <size_t> &outputSizes,
                  aclrtStream &stream, size_t dynamicBatchSize = 0);

        aclmdlDesc *GetModelDesc();

    private:
        aclmdlDataset *CreateAndFillDataset(std::vector<void *> &bufs, std::vector <size_t> &sizes);

        void DestroyDataset(aclmdlDataset *dataset);

    private:
        ///> Id of import model
        uint32_t modelId_ = 0; 

        aclrtContext context_ = nullptr;
        
        std::shared_ptr <aclmdlDesc> modelDesc_ = nullptr;
    };

}
#endif //ARCTERN_ATLASACLINFER_H
