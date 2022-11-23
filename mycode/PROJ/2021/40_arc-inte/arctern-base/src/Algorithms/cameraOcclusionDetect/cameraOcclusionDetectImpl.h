/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.10
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_CAMERAOCCLUSIONDETECT_CAMERAOCCLUSIONDETECTIMPL_H_
#define SRC_ALGORITHMS_CAMERAOCCLUSIONDETECT_CAMERAOCCLUSIONDETECTIMPL_H_
#include <utility>
#include <vector>
#include <memory>
#include "src/Base/algorithmbase.h"
#include "cameraOcclusionDetect.h"
#include "opencv2/opencv.hpp"
namespace background_estimation{
     /* =====================================================================  *
     * metric function                                                         *
     * ====================================================================== */
    double psnr(cv::Mat &I1, cv::Mat &I2);
    double ssim(cv::Mat &i1, cv::Mat & i2);

    /* =====================================================================  *
     * FrameDifferenceC1L1                                                    *
     * ====================================================================== */

    class FrameDifferenceC1L1 {
      private:

        cv::Mat previous_frame;

      public:

        FrameDifferenceC1L1() {}

        virtual ~FrameDifferenceC1L1() {}

        void compute(const cv::Mat& current_frame, cv::Mat& motion_map);
    };
	
	class Utils {
      public:

        typedef std::vector<cv::Rect>                                     ROIs;

      public:

        static std::string getMethod(std::string method);

        static ROIs getROIs(size_t height, size_t width, size_t segments);

        static ROIs getROIs(size_t height, size_t width);

        static void normalize_mat(cv::Mat& input, double max = 1);

        static void normalize_mat(
          const cv::Mat& input,
          cv::Mat& output,
          double max = 1
        );
    };
	
    /* ====================================================================== *
     * HistoryMat                                                             *
     * ====================================================================== */

    class HistoryMat {
      private:
        friend bool operator< (const HistoryMat& lhs, const HistoryMat& rhs);
        friend bool operator<=(const HistoryMat& lhs, const HistoryMat& rhs);
        friend bool operator==(const HistoryMat& lhs, const HistoryMat& rhs);
        friend bool operator< (const HistoryMat& lhs, const uint32_t&   rhs);
        friend bool operator<=(const HistoryMat& lhs, const uint32_t&   rhs);
        friend bool operator==(const HistoryMat& lhs, const uint32_t&   rhs);
        friend bool operator< (const uint32_t&   lhs, const HistoryMat& rhs);
        friend bool operator<=(const uint32_t&   lhs, const HistoryMat& rhs);
        friend bool operator==(const uint32_t&   lhs, const HistoryMat& rhs);

      protected:

        uint8_t mat[3];
        uint32_t positives;

      public:

        HistoryMat(const unsigned char* mat, const uint32_t positives);

        HistoryMat(const HistoryMat& copy);

        HistoryMat& operator=(const HistoryMat& copy);

        uint8_t operator[](size_t i) const;
    };

    /* ====================================================================== *
     * History                                                                *
     * ====================================================================== */

    class History {
      public:

        typedef std::vector<HistoryMat>                             HistoryVec;

      protected:

        HistoryVec history;
        size_t buffer_size;

      public:

        explicit History(size_t buffer_size);

        HistoryVec& operator*();

        const HistoryVec& operator*() const;

        void insert(
          const int32_t* quantities_of_motion,
          const unsigned char* current_frame
        );

        void median(unsigned char* result, size_t size = ~0) const;

        bool empty() const;
    };

    /* ====================================================================== *
     * PatchesHistory                                                         *
     * ====================================================================== */

    class PatchesHistory {
      protected:

        typedef std::vector<History>                         PatchesHistoryVec;

      protected:

        PatchesHistoryVec p_history;
        Utils::ROIs rois;

      public:

        PatchesHistory(const Utils::ROIs& rois, size_t buffer_size);

        void insert(
          const cv::Mat& quantities_of_motion, const cv::Mat& current_frame
        );

        void median(cv::Mat& result, size_t size = ~0) const;

		void clear_history(int buffer_size);

        bool empty() const;
    };
	 
    /* ====================================================================== *
     * QuantitiesMotion                                                       *
     * ====================================================================== */

    class QuantitiesMotion {
      protected:

        typedef int32_t                               QuantitiesMotionEncoding;

      protected:

        int size;

      public:

        QuantitiesMotion(int size);

        void compute(cv::Mat& motion_map, cv::Mat& quantities_of_motion) const;

        int getOpenCVEncoding() const;
    };
    
    /* ====================================================================== *
     * SummedAreaTables                                                       *
     * ====================================================================== */

    /**
     * This class implements the method known as "Integral Image Representation"
     * or "Summed area tables" introduced by F. Crown at SIGGRAPH 1984.
     */
    template <typename Input, typename Output = Input>
    class SummedAreaTables {
      private :

        int w;
        int h;
        Output* sum;

      public :
        SummedAreaTables(const cv::Mat& mat);
		
        virtual ~SummedAreaTables();

        Output getIntegral(int row, int col) const;

        Output getIntegral(
          int min_row,
          int max_row,
          int min_col,
          int max_col
        ) const;
    };
	
    template<typename Input, typename Output> 
    SummedAreaTables<Input, Output>::SummedAreaTables(const cv::Mat& mat){
      w = mat.cols;
      h = mat.rows;
      sum = new Output[mat.total()];
      if (w == 0)
        throw std::logic_error("Image with zero width are not supported");

      if (h == 0)
        throw std::logic_error("Image with zero height are not supported");

      const Input* buffer = reinterpret_cast<const Input*>(mat.data);

      for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
          sum[row * w + col] =
            *(buffer++) +
            getIntegral(row - 1, col    ) +
            getIntegral(row    , col - 1) -
            getIntegral(row - 1, col - 1) ;
        }
      }
    }
    
    template<typename Input, typename Output>
    SummedAreaTables<Input, Output>::~SummedAreaTables(){
      delete[] sum;
    }
    
    template<typename Input, typename Output>
    inline Output SummedAreaTables<Input, Output>::getIntegral(int row, int col) const{
      if (row < 0)
        return Output();

      if (col < 0)
        return Output();

      return sum[std::min(row, h - 1) * w + std::min(col, w - 1)];
    }

    template<typename Input, typename Output>
    Output SummedAreaTables<Input, Output>::getIntegral(
      int min_row,
      int max_row,
      int min_col,
      int max_col
    ) const{
      if (min_row > max_row)
        return Output();

      if (min_col > max_col)
        return Output();

      return getIntegral(max_row    , max_col    ) -
             getIntegral(min_row - 1, max_col    ) -
             getIntegral(max_row    , min_col - 1) +
             getIntegral(min_row - 1, min_col - 1) ;
    }
    class LaBGen_P {
      protected:

        size_t height;
        size_t width;
        int32_t s;
        int32_t n;
        FrameDifferenceC1L1 f_diff;
        cv::Mat motion_map;
        cv::Mat quantities_of_motion;
        QuantitiesMotion filter;
        PatchesHistory history;
        bool first_frame;

      public:
        LaBGen_P(size_t height, size_t width, int32_t s, int32_t n);

        void reset_buffer(int buffer_size);

        void insert(const cv::Mat& current_frame);

        void generate_background(cv::Mat& background) const;

        size_t get_height() const;

        size_t get_width() const;

        int32_t get_s() const;

        int32_t get_n() const;

        const cv::Mat& get_motion_map() const;

        const cv::Mat& get_quantities_of_motion() const;
    };
} // namespace background_estimation

namespace arctern {
class CameraOcclusionDetectImpl {
 public:
  CameraOcclusionDetectImpl();

  ~CameraOcclusionDetectImpl();

  ErrorType Init(CameraOcclusionDetectInitParameter *p);

  void Release();

  ErrorType Process(const RunParameter *p,Result *r);
  const Result *GetRealBackground(const RunParameter *p);

 private:
  ErrorType Preprocess(const RunParameter *p);
  void reset();
  void CalcMotionInfo();
  std::pair<int, float> CheckingOcclusion(const std::vector<cv::Mat> former_background_sequence);

 protected:
  Result *result_ = nullptr;
 private:
  std::vector<cv::Mat> preprocessed_imgs_;
  std::vector<cv::Mat> preprocessed_refs_;
  std::shared_ptr<background_estimation::LaBGen_P> labgen_p_;
  int s_param_;
  int n_param_;
  int resize_width_;
  int resize_height_;
  bool first_frame_;
  int least_frame_count_;
  float motion_thr_;
  float regionmotion_thr_;
  float lessmotioncount_thr_;
  float static_thr_;
  float ssim_thr_;
  //motion vector
  std::vector<std::vector<float>> motion_region_vec_;
  std::vector<float> current_motion_buffer_;
  //motion blocks
  int x_blocks_;
  int y_blocks_;
  int region_height_;
  int region_width_;
  //motion map
  std::unique_ptr<cv::Mat> motion_map_8u_;
  cv::Mat background_;
  std::vector<cv::Mat> motion_regions_;
};

}  // namespace arctern

#endif  // SRC_ALGORITHMS_CAMERAOCCLUSIONDETECT_CAMERAOCCLUSIONDETECTIMPL_H_
