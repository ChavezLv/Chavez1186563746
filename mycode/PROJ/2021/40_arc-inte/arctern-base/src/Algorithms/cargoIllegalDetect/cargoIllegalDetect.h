
#ifndef SRC_ALGORITHMS_CARGOILLEGALDETECT_CARGOILLEGALDETECT_H_
#define SRC_ALGORITHMS_CARGOILLEGALDETECT_CARGOILLEGALDETECT_H_
#include <vector>
#include "include/algorithminterface.h"
#include "include/arctern.h"

namespace arctern {

class AlgorithmBase;  ///< prior declaring

/// \brief cargo illegal 
enum CargoIllegalDetectType {
  CARGO_ILLEGAL_ABNORMAL = 0, ///<  abnormal
  CARGO_ILLEGAL_NORMAL = 1,    ///< normal
};

/// \brief cargo illegal detect result
typedef struct {
  arctern::ArcternRect rect;      ///< rectangle
  float score;  ///< 可信度
  CargoIllegalDetectType type;   ///< illegal type
}CARGOILLEGAL_DETECT_TINY_INFO;

using DETECT_INFOS = std::vector<CARGOILLEGAL_DETECT_TINY_INFO>;

/// \brief 井盖异常检测运行参数类
class ARCTERN_EXPORT_API CargoIllegalDetectRunParameter final : public RunParameter {
 public:
  /// \brief constuctor
  CargoIllegalDetectRunParameter();

  /// \brief destructor
  ~CargoIllegalDetectRunParameter() final;

  /// \brief checking func
  ErrorType CheckSize() const override;
};

/// \brief store detect result
class ARCTERN_EXPORT_API CargoIllegalDetectResult : public Result {
 public:
  /// \brief constuctor
  CargoIllegalDetectResult();

  /// \brief destructor
  ~CargoIllegalDetectResult() override;

  /// \brief release source
  void Release() override { detInfos_.clear(); }

  /// \brief resize, upper layer can ignore it.
  /// \param n - vector size
  void resize(int n) override { detInfos_.resize(n); }

 public:
  std::vector<DETECT_INFOS> detInfos_;   ///<  检测信息
};

/// \brief cargo illegal detect alg class
class ARCTERN_EXPORT_API CargoIllegalDetect : public AlgorithmInterface {
 public:
  /// \brief constuctor
  CargoIllegalDetect();

  /// \brief destuctor
  ~CargoIllegalDetect() override;

  /// \brief init alg
  /// \param p - initial param
  /// \return error type
  ErrorType Init(const InitParameter *p) override;

  /// \brief release source
  void Release() override;

  /// \brief ececute alg
  /// \param p - operating param
  /// \return alg result
  ErrorType Process(const RunParameter *p,Result *r) override;


 private:
  AlgorithmBase *impl_ = nullptr;    ///< alg base class
};
}  /// namespace arctern
#endif  // SRC_ALGORITHMS_CARGOILLEGALDETECT_CARGOILLEGALDETECT_H_
