
if(PLATFORM_LINUX)
    message("------------------------------------------TVMModelStaticLib PLATFORM_LINUX")

    if(${AVX_TARGET} STREQUAL "avx2")
        set(TVM_MODELS_DEFINED_SYMBOL_PATH "${PROJECT_SOURCE_DIR}/tvmstaticlib/tvm0.7/avx2")
        set(TVM_MODELS_DEFINED_SYMBOL

                # face_gluon_3.8.0
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_3.8.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_quality_cpu_1.3.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_mask_cpu_0.2.1_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_landmark_cpu_1.3.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_retinaface_cpu_1.0.2_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_pose_cls_cpu_1.0.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_glass_cpu_1.0.3_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_expression_classify_cpu_0.0.2_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_hat_cpu_1.0.3_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_noise_motionblur-classifier_cpu_0.0.9_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_occlusion_detect_cpu_0.0.1_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_abnomal_light_classifier_cpu_0.0.1_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_abnomal_light_classifier_cpu_0.0.2_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_abnormal_light_classifier_cpu_0.0.3_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_abnormal_light_classifier_cpu_0.0.4_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gender_age_0.2.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_multi_attributes_cpu_0.1.1_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_det_video_cpu_0.0.3_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_3.9.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/person_feat_tiny_cpu_0.5.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/person_feat_f32_cpu_0.4.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/person_feat_f32_cpu_0.9.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gender_age_cpu_0.1.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_quality_pose_cpu_0.0.1_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_noise_motionblur_classifier_cpu_0.1.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/person_quality_cpu_1.0.3_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_3.7.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_3.6.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_2.5.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_3.5.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/face_gluon_cpu_7.2.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/person_completeness_f32_cpu_0.0.0_lib.a
                ${TVM_MODELS_DEFINED_SYMBOL_PATH}/person_feat_f32_cpu_1.0.0_lib.a
                )

    endif()
else()
    message("----TvmModelStaticLib Cmake Run Nothing")
endif()
