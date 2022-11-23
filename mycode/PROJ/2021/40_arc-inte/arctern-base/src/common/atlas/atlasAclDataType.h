//
// Created by zy on 8/28/20.
//

#ifndef ARCTERN_ATLASACLDATATYPE_H
#define ARCTERN_ATLASACLDATATYPE_H
#include "acl/acl.h"
#include "acl/ops/acl_dvpp.h"
#include "errorCode.h"

namespace arctern {
    /// switch context
    #define SWITCH_ATLAS_ACL_CONTEXT(context)                         \
        auto ret = aclrtSetCurrentContext(context);                   \
        if (ret != APP_ERR_OK) {                                      \
          LOGASDError << "Failed to set ACL context, ret = " << ret;  \
          return APP_ERR_COMM_FAILURE;                                \
        }

    #define DVPP_ALIGN_UP(x, align) ((((x) + ((align)-1)) / (align)) * (align))

    const int YUV_BGR_SIZE_CONVERT_3 = 3;
    const int YUV_BGR_SIZE_CONVERT_2 = 2;
    const int VPC_WIDTH_ALIGN = 16;
    const int VPC_HEIGHT_ALIGN = 2;

    enum VpcProcessType {
        VPC_PT_DEFAULT = 0,
        VPC_PT_PADDING,       // Resize with locked ratio
        VPC_PT_BOTTOM
    };

    struct DvppDataInfo {
        uint32_t width = 0;                                           // Width of image
        uint32_t height = 0;                                          // Height of image
        uint32_t widthStride = 0;                                     // Width after align up
        uint32_t heightStride = 0;                                    // Height after align up
        acldvppPixelFormat format = PIXEL_FORMAT_YUV_SEMIPLANAR_420;  // Format of image
        uint32_t frameId = 0;                                         // Needed by video
        uint32_t dataSize = 0;                                        // Size of data in byte
        uint8_t *data = nullptr;                                      // Image data
    };

    struct CropRoiConfig {
        uint32_t left;
        uint32_t right;
        uint32_t down;
        uint32_t up;
    };

    struct DvppCropInputInfo {
        DvppDataInfo dataInfo;
        CropRoiConfig roi;
    };
}
#endif //ARCTERN_ATLASACLDATATYPE_H
