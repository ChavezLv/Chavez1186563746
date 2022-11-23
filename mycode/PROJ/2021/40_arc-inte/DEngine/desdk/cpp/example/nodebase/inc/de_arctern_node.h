
#ifndef DE_ARCTERN_NODE_H_
#define DE_ARCTERN_NODE_H_
#include <core/de_serializer.h>
#include <core/de_ndarray.h>
#include <node/de_node_common.h>



namespace de
{
using PairIntRect = std::pair<int32_t, deRect>;
using PairBoolFloat = std::pair<bool, float>;
using PairIntFloat = std::pair<int32_t, float>;
using PairStringFloat = std::pair<std::string, float>;
using PairIntInt = std::pair<int32_t, int32_t>;
using PairIntString = std::pair<int32_t, std::string>;
using PairIntVectorUint8 = std::pair<int32_t, std::vector<uint8_t>>;
using PairIntVectorInt = std::pair<int32_t, std::vector<int32_t>>;
using PairIntVectorPairIntFloat = std::pair<int32_t, std::vector<PairIntFloat>>;
using PairStringVectorString = std::pair<std::string, std::vector<std::string>>;
using PairStringVectorPairStringFloat = std::pair<std::string, std::vector<PairStringFloat>>;
using MapIntPairIntInt = std::map<int32_t, PairIntInt>;
using MapIntVectorPairIntInt = std::map<int32_t,std::vector<PairIntInt>>;
using MapIntVectorPairIntString = std::map<int32_t,std::vector<PairIntString>>;
using MapIntVectorPairIntVectorInt = std::map<int32_t,std::vector<PairIntVectorInt>>;
using MapIntVectorPairIntVectorPairIntFloat = std::map<int32_t,std::vector<PairIntVectorPairIntFloat>>;
using MapIntVectorPairStringVectorString = std::map<int32_t,std::vector<PairStringVectorString>>;
using MapIntVectorPairStringVectorPairStringFloat = std::map<int32_t,std::vector<PairStringVectorPairStringFloat>>;
using MapIntVectorPairIntRect = 	std::map<int32_t, std::vector<PairIntRect>>;
using MapIntVectorInt = std::map<int32_t, std::vector<int32_t>>;


namespace arctern
{

//----------------------------------------------------------------
//
//          类型定义
//
//---------------------------------------------------------------

/**
 * @brief 类型分为2级
 * 1. 第一级别大类型的最低8bit为0，支持人脸、人体、车等；
 * 2. 第二级别小类型为细化补充类型，比如人脸年龄、人脸性别等；
 */
enum
{
    // 人脸类型
    FACE_CAT_ID = 0,
    FACE_QUAL_ID ,
    FACE_ATTR_AGE_GENDER, // for face-attr-xxx.bin
    FACE_ATTR_AGE,
    FACE_ATTR_GENDER,
    FACE_ATTR_HAT,
    FACE_ATTR_GLASS,
    FACE_ATTR_MASK,
    FACE_ATTR_TYPE,
    FACE_ATTR_TYPE2,
    FACE_FEATURE,

    // 人体类型
    HUMAN_CAT_ID = 1l << 8,
    HUMAN_QUAL_ID,
    HUMAN_ATTR,  //for person-attr-xxx.bin, from HUMAN_ATTR to HUMAN_ATTR_USE_PHONE must be continuous!!
    HUMAN_ATTR_AGE,
    HUMAN_ATTR_BACKPACK,
    HUMAN_ATTR_CART,
    HUMAN_ATTR_FRONT_HOLD,
    HUMAN_ATTR_GENDER,
    HUMAN_ATTR_GLASSES,
    HUMAN_ATTR_HAIR_COLOR,
    HUMAN_ATTR_HAIR_LENGTH,
    HUMAN_ATTR_HANDBAG,
    HUMAN_ATTR_HEAD_WEAR,
    HUMAN_ATTR_LOWER_COLOR,
    HUMAN_ATTR_LOWER_TEXTURE,
    HUMAN_ATTR_LOWER_TYPE,
    HUMAN_ATTR_MASK,
    HUMAN_ATTR_ORIENTATION,
    HUMAN_ATTR_OVERCOAT,
    HUMAN_ATTR_PERSON,
    HUMAN_ATTR_SINGLE_BAG,
    HUMAN_ATTR_TROLLEY_CASE,
    HUMAN_ATTR_UPPER_COLOR,
    HUMAN_ATTR_UPPER_TEXTURE,
    HUMAN_ATTR_UPPER_TYPE,
    HUMAN_ATTR_USE_PHONE,
    HUMAN_COMPLETENESS, //for person-completeness-xxx.bin, from HUMAN_COMPLETENESS_TRUNCATION to HUMAN_COMPLETENESS_SOFTMASK must be continuous!!
    HUMAN_COMPLETENESS_TRUNCATION,
    HUMAN_COMPLETENESS_VISIBLE,
    HUMAN_COMPLETENESS_SOFTMASK,
    

    // 机动车类型
    CAR_CAT_ID = 2l << 8,
    CAR_QUAL_ID,
    CAR_ATTR, //for vehicle-attributes-x.x.x.bin
    CAR_ATTR_COLOR,
    CAR_ATTR_TYPE,
    CAR_LICENSE_ID,


    // 非机动车类型
    BIKE_CAT_ID = 3l << 8,
    BIKE_QUAL_ID,


    // 车牌类型, 注意：车牌不做为独立抓拍类型，从属于车属性
    LICENSE_CAT_ID = 4l << 8,
    LICENSE_QUAL_ID,

};

inline int32_t GetBigCat(int32_t cat_type)
{
    return cat_type & ~0xff;
};

//
//      通用类型定义
//
struct MediaTaskS
{
    std::vector<MediaTask> tasks;
};

//
//      Track-Sync Node
//
//

/**
 * @brief 跟踪结果信息
 */
struct MultiTrackResult
{
    int32_t stream_id;
    int64_t pts;
    int32_t frm_id;
    /**
     * @brief 跟踪结果，map中存放的是目标类型，以及该类型下的跟踪结果
     * 跟踪结果Pair中int类型为跟踪结果的track id, Rect为跟踪结果的Boxes框
     */
    std::map<int32_t, std::vector<de::PairIntRect>> obj_boxes;
	/**
	* @brief 各跟踪目标亮度值,map<目标类型,pair(track id,亮度)>
	*/
	std::map<int32_t, std::vector<de::PairIntInt>> obj_lumas;
};


/**
 * @brief 跟踪结果信息(携带img图)
 */
struct MultiTrackResultImg
{
    int32_t stream_id;
    int64_t pts;
    int32_t frm_id;
    de::NDArray img;
    /**
     * @brief 跟踪结果，map中存放的是目标类型(face、human、bike、car、license)，以及该类型下的跟踪结果
     * 跟踪结果Pair中int类型为跟踪结果的track id, Rect为跟踪结果的Boxes框
     */
    std::map<int32_t, std::vector<de::PairIntRect>> obj_boxes;
	
	/**
	* @brief 各跟踪目标亮度值,map<目标类型,pair(track id,亮度)>
	*/
	std::map<int32_t, std::vector<de::PairIntInt>> obj_lumas;
};

/**
 * @brief 跟踪丢失结果信息
 */
struct TrackMissedResult
{
    int32_t stream_id;
    int64_t pts;
    int32_t frm_id;
    std::map<int32_t, std::vector<int32_t>> obj_missed;
};

/**
* @brief 单类型检测任务输出(图像)
*/
struct DetRsltImg
{
    int32_t stream_id;
    int64_t id; // pic id or frame id or video frame pts
    de::NDArray img;
    std::vector<deRect> rects;
};

/**
* @brief 单类型检测任务输出(无图像)
*/
struct DetRslt
{
    int32_t stream_id;
    int64_t id; // pic id or frame id or video frame pts
    std::vector<deRect> rects;
};

struct DetRsltS
{
    std::vector<DetRslt> rslts;
};

/**
 * @brief 多检测类型
 */
struct MultiCatDetection
{
    deRect box;
    float confidence;
    int32_t type; ///< BIKE_CAT_ID/CAR_CAT_ID/LICENSE_CAT_ID/HUMAN_CAT_ID
	float quality;
};


/**
 * @brief 多类型检测任务输出(图像)
 */
struct MultiDetRsltImg
{
    int32_t stream_id;
    int64_t id; // pic id or frame id or video frame pts
    de::NDArray img;
    std::vector<MultiCatDetection> rects;
};

/**
 * @brief 多类型检测任务输出(无图像)
 */
struct MultiDetRslt
{
    int32_t stream_id;
    int64_t id; // pic id or frame id or video frame pts
    std::vector<MultiCatDetection> rects;
};

struct MultiDetRsltS
{
    std::vector<MultiDetRslt> rslts;
};
/**
 * @brief 目标质量请求任务
 */
struct ObjQualReq
{
    int32_t stream_id;
    int64_t img_id;
    de::NDArray img;
    std::vector <PairIntRect> rects;  ///< pair type = <obj_id, rect>
};

struct ObjQualReqS
{
    std::vector<ObjQualReq> reqs;
};

/**
 * @brief 目标质量结果(带图像数据)
 */
struct ObjQualRsltImg
{
    int32_t stream_id;
    int64_t img_id;
    int32_t cat_id; ///< 目标类型
    std::vector <PairIntRect> rects;  ///< pair type = <obj_id, rect>
    std::vector<PairIntFloat> qual; ///< pair type = <obj_id, score>
    de::NDArray img;
};

/**
 * @brief 目标质量结果
 */
struct ObjQualRslt
{
    int32_t stream_id;
    int64_t img_id;
    int32_t cat_id; ///< 目标类型
    std::vector <PairIntRect> rects;  ///< pair type = <obj_id, rect>
    std::vector<PairIntFloat> qual; ///< pair type = <obj_id, score>
};

struct ObjQualRsltS
{
    std::vector<ObjQualRslt> rslts;
};
/**
 * @brief 目标属性请求
 */
struct ObjAttrReq
{
    int32_t stream_id;
    int32_t img_id;
    de::NDArray img;
    std::vector<std::vector<float>> pad_scale;
    //std::vector < std::pair<int32_t, deRect>> rects;  ///< pair type = <obj_id, rect>
    std::vector<PairIntRect> rects;  ///< pair type = <obj_id, rect>
};

struct ObjAttrReqS
{
    std::vector<ObjAttrReq> reqs;
};
/**
 * @brief 目标单类型属性结果
 */
struct ObjAttrRslt
{
    int32_t stream_id;
    int64_t img_id;
    int32_t attr_id; ///< 目标类型
    std::vector<PairIntInt> attr; ///< pair type = <obj_id, attr>, attr和模型有关
};

/**
 * @brief 目标多类型属性结果
 */
struct ObjMultiAttrRslt
{
    int32_t stream_id;
    int64_t img_id;
    int32_t attr_id; ///< 目标类型    
    std::map<int32_t, std::vector<PairIntVectorPairIntFloat>> attrs; ///< key: obj id,  val : { PairIntInt.key :attr_type_id, PairIntVectorPairIntFloat: attr};
};

struct ObjMultiAttrRsltS
{
    std::vector<ObjMultiAttrRslt> rslts;
};

struct ObjMultiAttrRsltString
{
    int32_t stream_id;
    int64_t img_id;
    int32_t attr_id; ///< 目标类型    
    std::map<int32_t,std::vector<PairStringVectorPairStringFloat>> attrs; ///< key: obj id,  val : { PairSting.key :attr_type_str, PairStringFloat: attr_str};
};


struct ObjMultiAttrRsltStringS
{
    std::vector<ObjMultiAttrRsltString> rslts;
};


struct FaceAttribute {
  // gender field:
  //     the first bool value is the predicted label, 0 means female, 1 means male;
  //     the second float value is the corresponding score.
  std::pair<bool, float> gender;

  // age field:
  //     the first int value is the predicted age;
  //     the second float value is the corresponding score of age.
  std::pair<int, float> age;
};


/**
 * @brief 目标特征请求
 */
struct ObjFeatReq
{
    int32_t stream_id;
    int32_t img_id;
    de::NDArray img;
    std::vector<PairIntRect> rects;  ///< pair type = <obj_id, rect>
};

struct ObjFeatReqS
{
    std::vector<ObjFeatReq> reqs;
};
/**
 * @brief 目标特征结果
 */
struct ObjFeatRslt
{
    int32_t stream_id;
    int64_t img_id;
    int32_t cat_id; ///< 目标类型
    int32_t data_type; // 0: uint8, 1 - float16, 2 - float32
    std::vector<PairIntVectorUint8> feature; ///< pair type = <obj_id, feature>,
};

struct ObjFeatRsltS
{
    std::vector<ObjFeatRslt> rslts;
};

//
//      专用类型定义
//

/**
 * @brief 人体多类型属性结果
 */
struct PersonMultiAttrRslt
{
    int32_t stream_id;
    int64_t img_id;
    std::map<int32_t, std::vector<PairIntString>> attrs; ///< key: attr_type,  val : { PairIntInt.key : obj id, PairIntString: attr};
};

/**
 * @brief 机动车多类型属性结果
 */
struct CarMultiAttrRslt
{
    int32_t stream_id;
    int64_t img_id;
    std::map<int32_t, std::vector<PairIntString>> attrs; ///< key: attr_type,  val : { PairIntInt.key : obj id, PairIntString: attr};
};


} // namespace arctern



//---------------------------------------------------------------------
//
//              序列化
//
//--------------------------------------------------------------------

namespace serializer
{
 STRUCT_SERIALIZE_2(arctern::FaceAttribute,
                   PairBoolFloat, gender,
                   PairIntFloat, age);
  
STRUCT_SERIALIZE_1(arctern::MediaTaskS,
                   std::vector<MediaTask>, tasks);

STRUCT_SERIALIZE_4(arctern::DetRsltImg,
                   int32_t, stream_id,
                   int64_t, id,
                   NDArray, img,
                   std::vector<deRect>, rects);

STRUCT_SERIALIZE_3(arctern::DetRslt,
                   int32_t, stream_id,
                   int64_t, id,
                   std::vector<deRect>, rects);

STRUCT_SERIALIZE_1(arctern::DetRsltS,
                   std::vector<arctern::DetRslt>, rslts);


STRUCT_SERIALIZE_4(arctern::MultiCatDetection,
                   deRect, box,
                   float, confidence,
                   int32_t, type,
                   float, quality);


STRUCT_SERIALIZE_4(arctern::MultiDetRsltImg,
                   int32_t, stream_id,
                   int64_t, id,
                   de::NDArray, img,
                   std::vector<arctern::MultiCatDetection>, rects);

STRUCT_SERIALIZE_3(arctern::MultiDetRslt,
                   int32_t, stream_id,
                   int64_t, id,
                   std::vector<arctern::MultiCatDetection>, rects);

STRUCT_SERIALIZE_1(arctern::MultiDetRsltS,
                   std::vector<arctern::MultiDetRslt>, rslts);

STRUCT_SERIALIZE_4(arctern::ObjQualReq,
                   int32_t, stream_id,
                   int64_t, img_id,
                   de::NDArray, img,
                   std::vector<PairIntRect>, rects);

STRUCT_SERIALIZE_1(arctern::ObjQualReqS,
                   std::vector<arctern::ObjQualReq>, reqs);

STRUCT_SERIALIZE_6(arctern::ObjQualRsltImg,
                   int32_t, stream_id,
                   int64_t, img_id,
                   int32_t, cat_id,
                   std::vector<PairIntRect>, rects,
                   std::vector<PairIntFloat>, qual,
                   de::NDArray, img);

STRUCT_SERIALIZE_5(arctern::ObjQualRslt,
                   int32_t, stream_id,
                   int64_t, img_id,
                   int32_t, cat_id,
                   std::vector<PairIntRect>, rects,
                   std::vector<PairIntFloat>, qual);


STRUCT_SERIALIZE_1(arctern::ObjQualRsltS,
                   std::vector<arctern::ObjQualRslt>, rslts);


STRUCT_SERIALIZE_5(arctern::ObjAttrReq,
                   int32_t, stream_id,
                   int32_t, img_id,
                   de::NDArray, img,
                   std::vector<std::vector <float>>, pad_scale,
                   std::vector < PairIntRect>, rects);

STRUCT_SERIALIZE_1(arctern::ObjAttrReqS,
                   std::vector<arctern::ObjAttrReq>, reqs);

STRUCT_SERIALIZE_4(arctern::ObjAttrRslt,
                   int32_t, stream_id,
                   int64_t, img_id,
                   int32_t, attr_id,
                   std::vector<PairIntInt>, attr);


STRUCT_SERIALIZE_4(arctern::ObjMultiAttrRslt,
                   int32_t, stream_id,
                   int64_t, img_id,
                   int32_t, attr_id,
                   MapIntVectorPairIntVectorPairIntFloat, attrs);

STRUCT_SERIALIZE_4(arctern::ObjMultiAttrRsltString,
                  int32_t, stream_id,
                  int64_t, img_id,
                  int32_t, attr_id,
                  MapIntVectorPairStringVectorPairStringFloat, attrs);               


STRUCT_SERIALIZE_1(arctern::ObjMultiAttrRsltS,
                   std::vector<arctern::ObjMultiAttrRslt>, rslts);


STRUCT_SERIALIZE_1(arctern::ObjMultiAttrRsltStringS,
                  std::vector<arctern::ObjMultiAttrRsltString>, rslts);


STRUCT_SERIALIZE_4(arctern::ObjFeatReq,
                   int32_t, stream_id,
                   int32_t, img_id,
                   de::NDArray, img,
                   std::vector <PairIntRect>, rects);

STRUCT_SERIALIZE_1(arctern::ObjFeatReqS,
                   std::vector<arctern::ObjFeatReq>, reqs);

STRUCT_SERIALIZE_5(arctern::ObjFeatRslt,
                   int32_t , stream_id,
                   int64_t, img_id,
                   int32_t , cat_id,
                   int32_t , data_type,
                   std::vector<PairIntVectorUint8>, feature);

STRUCT_SERIALIZE_1(arctern::ObjFeatRsltS,
                   std::vector<arctern::ObjFeatRslt>, rslts);

STRUCT_SERIALIZE_3(arctern::PersonMultiAttrRslt,
                   int32_t, stream_id,
                   int64_t, img_id,
                   MapIntVectorPairIntString, attrs);

STRUCT_SERIALIZE_3(arctern::CarMultiAttrRslt,
                   int32_t, stream_id,
                   int64_t, img_id,
                   MapIntVectorPairIntString, attrs);
				   
STRUCT_SERIALIZE_5(de::arctern::MultiTrackResult,
                   int32_t, stream_id,
                   int64_t, pts,
                   int32_t, frm_id,
                   MapIntVectorPairIntRect, obj_boxes,
                   MapIntVectorPairIntInt,obj_lumas);


STRUCT_SERIALIZE_6(de::arctern::MultiTrackResultImg,
                   int32_t, stream_id,
                   int64_t, pts,
                   int32_t, frm_id,
                   de::NDArray, img,
                   MapIntVectorPairIntRect, obj_boxes,
                   MapIntVectorPairIntInt,obj_lumas);


STRUCT_SERIALIZE_4(de::arctern::TrackMissedResult,
                   int32_t, stream_id,
                   int64_t, pts,
                   int32_t, frm_id,
                   MapIntVectorInt, obj_missed);


} // namespace serializer


} // namespace de



//---------------------------------------------------------------------
//
//             类型stream打印调试输出
//
//--------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const  deRect& rslt)
{
    os << "[(" << rslt.x << "," << rslt.y << ")," << rslt.w << "," << rslt.h << "]";
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const  de::arctern::ObjQualRsltImg& rslt)
{
    os << "frm=" << (int32_t)rslt.img_id << ", cat=" << rslt.cat_id << ", idnum=" << (int32_t)rslt.rects.size() << ", ";
    for(int32_t i = 0; i < (int32_t)rslt.rects.size(); i++)
    {
        auto track_id = rslt.rects[i].first;
        auto& rect = rslt.rects[i].second;
        auto qual = rslt.qual[i].second;
        os << "{id:" << track_id << "," << rect << "," << qual << "}";
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const  de::arctern::ObjQualRslt& rslt)
{
    os << "frm=" << (int32_t)rslt.img_id << ", cat=" << rslt.cat_id << ", idnum=" << (int32_t)rslt.rects.size() << ", ";
    for(int32_t i = 0; i < (int32_t)rslt.rects.size(); i++)
    {
        auto track_id = rslt.rects[i].first;
        auto& rect = rslt.rects[i].second;
        auto qual = rslt.qual[i].second;
        os << "{id:" << track_id << "," << rect << "," << qual << "}";
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const  de::arctern::ObjAttrRslt& rslt) {
        os << "stream="  << rslt.stream_id << ",img_id=" << rslt.img_id <<",attr_id=" << rslt.attr_id << ",content = {";
        for(auto it = rslt.attr.begin(); it != rslt.attr.end(); ++it) {
                os << "(id:" << it->first << ",val=" << it->second << "),";
        }
        os << "}";
        return os;
}

inline std::ostream& operator<<(std::ostream& os, const  de::arctern::ObjMultiAttrRslt& rslt) {
        os << "stream="  << rslt.stream_id << ",img_id=" << rslt.img_id <<",attr_id=" << rslt.attr_id << ", ";
        for(auto it = rslt.attrs.begin(); it != rslt.attrs.end(); ++it) {
            os << "{id:" << it->first << ",";
            for (auto& attr : it->second)
            {
                os << "{attr_type:" << attr.first << ",val=(";
                if (attr.first == de::arctern::HUMAN_COMPLETENESS_TRUNCATION) {
                    for(auto attr_ : attr.second) {
                        os <<*( (float*)&attr_.first) << ",";
                    }
                }
                else
                {
                    for (size_t i = 0; i < attr.second.size(); i++) {
                        os << "("<<attr.second[i].first << "," << attr.second[i].second<<"),";
                    }
                }
                os << ")},";
            }
            os << "},"; 
        }
        return os;
}

inline std::ostream& operator<<(std::ostream& os, const  de::arctern::ObjMultiAttrRsltString& rslt) {
        os << "stream="  << rslt.stream_id << ",img_id=" << rslt.img_id <<",attr_id=" << rslt.attr_id << ", ";
        for(auto it = rslt.attrs.begin(); it != rslt.attrs.end(); ++it) {
            os << "{id:" << it->first << ",";
            for (auto& attr : it->second)
            {
                os << "{attr_type:" << attr.first << ",val=(";
                for (size_t i = 0; i < attr.second.size(); i++)
                {
                    os << "("<<attr.second[i].first << "," << attr.second[i].second <<")";
                }
                os << ")},";
            }
            os << "},"; 
        }
        return os;
}


inline std::ostream& operator<<(std::ostream& os, const  de::arctern::ObjFeatRslt& rslt)
{
    os << "stream=" << rslt.stream_id << ",img_id=" << rslt.img_id << ",cat=" << rslt.cat_id << ",dtype=" << rslt.data_type << ",content = {";
    auto& feat = rslt.feature;
    for(auto it = feat.begin(); it != feat.end(); ++it)
    {
        os << "(id:" << it->first << ",len=" << it->second.size() << "),";
    }
    os << "}";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const  de::arctern::MultiDetRslt& rslt)
{
    os << "stream=" << (int32_t)rslt.stream_id << ", frm=" << rslt.id << ", rects num=" << (int32_t)rslt.rects.size() << ", ";
    for(int32_t i = 0; i < (int32_t)rslt.rects.size(); i++)
    {
        auto type = rslt.rects[i].type;
        auto& rect = rslt.rects[i].box;
        auto confidence = rslt.rects[i].confidence;
        os << "{type:" << type << "," << rect << ",confidence=" << confidence << "}";
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const  de::arctern::MultiDetRsltImg& rslt)
{
    os << "stream=" << (int32_t)rslt.stream_id << ", frm=" << rslt.id << ", rects num=" << (int32_t)rslt.rects.size() << ", ";
    for(int32_t i = 0; i < (int32_t)rslt.rects.size(); i++)
    {
        auto type = rslt.rects[i].type;
        auto& rect = rslt.rects[i].box;
        auto confidence = rslt.rects[i].confidence;
        os << "{type:" << type << "," << rect << ",confidence=" << confidence << "}";
    }
    return os;
}


#endif
