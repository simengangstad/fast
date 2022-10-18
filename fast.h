#ifndef FAST_H
#define FAST_H

#include <stddef.h>
#include <stdint.h>

namespace fast {

    struct Feature {
        uint16_t x, y;
    };

    /**
     * @brief Detects, scores and suppresses non-max FAST corners.
     *
     * @param image_buffer [in] Image data.
     * @param width [in] Width of image.
     * @param height [in] Height of image.
     * @param stride [in] Column length of image (used with @p width to only run
     * the detection on a certain part of the image. If the whole image should
     * be run through the algorithm, this should be equal to the image width).
     * @param out_feature_buffer [out] Buffer for detected features.
     * @param feature_buffer_length [in] Length of the buffer for the detected
     * features.
     * @param out_number_of_corners [out] The number of features detected.
     */
    void detect_nonmax(const uint8_t* image_buffer,
                       const uint16_t width,
                       const uint16_t height,
                       const uint16_t stride,
                       const uint8_t threshold,
                       Feature* out_feature_buffer,
                       const size_t feature_buffer_length,
                       size_t* out_number_of_features);

    void detect(const uint8_t* image_buffer,
                const uint16_t width,
                const uint16_t height,
                const uint16_t stride,
                const uint8_t threshold,
                Feature* out_feature_buffer,
                const size_t feature_buffer_length,
                size_t* out_number_of_features);

    void score(const uint8_t* image_buffer,
               const uint16_t stride,
               const Feature* features,
               const size_t number_of_features,
               const uint8_t threshold,
               int* out_scores);

    void nonmax_suppression(const Feature* features,
                            const int* scores,
                            const size_t number_of_features,
                            Feature* out_feature_buffer,
                            size_t* out_number_of_features);

}
#endif
