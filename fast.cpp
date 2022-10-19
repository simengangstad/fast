#include "fast.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define Compare(X, Y) ((X) >= (Y))

namespace fast {

    static void make_offsets(uint32_t pixel[], const uint16_t row_stride) {
        pixel[0]  = 0 + row_stride * 3;
        pixel[1]  = 1 + row_stride * 3;
        pixel[2]  = 2 + row_stride * 2;
        pixel[3]  = 3 + row_stride * 1;
        pixel[4]  = 3 + row_stride * 0;
        pixel[5]  = 3 + row_stride * -1;
        pixel[6]  = 2 + row_stride * -2;
        pixel[7]  = 1 + row_stride * -3;
        pixel[8]  = 0 + row_stride * -3;
        pixel[9]  = -1 + row_stride * -3;
        pixel[10] = -2 + row_stride * -2;
        pixel[11] = -3 + row_stride * -1;
        pixel[12] = -3 + row_stride * 0;
        pixel[13] = -3 + row_stride * 1;
        pixel[14] = -2 + row_stride * 2;
        pixel[15] = -1 + row_stride * 3;
    }

    static uint8_t
    corner_score(const uint8_t* p, const uint32_t pixel[], int bstart) {
        int bmin      = bstart;
        int bmax      = 255;
        int threshold = (bmax + bmin) / 2;

        /*Compute the score using binary search*/
        for (;;) {
            int cb  = *p + threshold;
            int c_b = *p - threshold;

            if (p[pixel[0]] > cb)
                if (p[pixel[1]] > cb)
                    if (p[pixel[2]] > cb)
                        if (p[pixel[3]] > cb)
                            if (p[pixel[4]] > cb)
                                if (p[pixel[5]] > cb)
                                    if (p[pixel[6]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[7]] < c_b)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[14]] < c_b)
                                                if (p[pixel[8]] < c_b)
                                                    if (p[pixel[9]] < c_b)
                                                        if (p[pixel[10]] < c_b)
                                                            if (p[pixel[11]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [13]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [15]] <
                                                                            c_b)
                                                                            goto is_a_corner;
                                                                        else
                                                                            goto is_not_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[6]] < c_b)
                                        if (p[pixel[15]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[13]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [11]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [12]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [14]] <
                                                                            c_b)
                                                                            goto is_a_corner;
                                                                        else
                                                                            goto is_not_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[12]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [13]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [14]] <
                                                                        c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[13]] < c_b)
                                        if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[12]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [14]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [15]] <
                                                                        c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[5]] < c_b)
                                    if (p[pixel[14]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [11]] >
                                                                        cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[12]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [11]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [13]] <
                                                                        c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[14]] < c_b)
                                        if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[12]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [13]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [6]] <
                                                                        c_b)
                                                                        goto is_a_corner;
                                                                    else if (
                                                                        p[pixel
                                                                              [15]] <
                                                                        c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[6]] < c_b)
                                        if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[12]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [13]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[12]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [11]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[12]] < c_b)
                                    if (p[pixel[7]] < c_b)
                                        if (p[pixel[8]] < c_b)
                                            if (p[pixel[9]] < c_b)
                                                if (p[pixel[10]] < c_b)
                                                    if (p[pixel[11]] < c_b)
                                                        if (p[pixel[13]] < c_b)
                                                            if (p[pixel[14]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [6]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else if (
                                                                    p[pixel
                                                                          [15]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[4]] < c_b)
                                if (p[pixel[13]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[11]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[13]] < c_b)
                                    if (p[pixel[7]] < c_b)
                                        if (p[pixel[8]] < c_b)
                                            if (p[pixel[9]] < c_b)
                                                if (p[pixel[10]] < c_b)
                                                    if (p[pixel[11]] < c_b)
                                                        if (p[pixel[12]] < c_b)
                                                            if (p[pixel[6]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [5]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else if (
                                                                    p[pixel
                                                                          [14]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else if (
                                                                p[pixel[14]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [15]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[5]] < c_b)
                                    if (p[pixel[6]] < c_b)
                                        if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[12]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[11]] > cb)
                                if (p[pixel[12]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[11]] < c_b)
                                if (p[pixel[7]] < c_b)
                                    if (p[pixel[8]] < c_b)
                                        if (p[pixel[9]] < c_b)
                                            if (p[pixel[10]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        if (p[pixel[6]] < c_b)
                                                            if (p[pixel[5]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else if (
                                                                p[pixel[14]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else if (p[pixel[14]] <
                                                                 c_b)
                                                            if (p[pixel[15]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[3]] < c_b)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[10]] < c_b)
                                if (p[pixel[7]] < c_b)
                                    if (p[pixel[8]] < c_b)
                                        if (p[pixel[9]] < c_b)
                                            if (p[pixel[11]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[5]] < c_b)
                                                        if (p[pixel[4]] < c_b)
                                                            goto is_a_corner;
                                                        else if (p[pixel[12]] <
                                                                 c_b)
                                                            if (p[pixel[13]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] < c_b)
                                                            if (p[pixel[14]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] < c_b)
                                                            if (p[pixel[15]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[10]] > cb)
                            if (p[pixel[11]] > cb)
                                if (p[pixel[12]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[4]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[10]] < c_b)
                            if (p[pixel[7]] < c_b)
                                if (p[pixel[8]] < c_b)
                                    if (p[pixel[9]] < c_b)
                                        if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[5]] < c_b)
                                                        if (p[pixel[4]] < c_b)
                                                            goto is_a_corner;
                                                        else if (p[pixel[13]] <
                                                                 c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b)
                                                        if (p[pixel[15]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[2]] < c_b)
                        if (p[pixel[9]] > cb)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[3]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[9]] < c_b)
                            if (p[pixel[7]] < c_b)
                                if (p[pixel[8]] < c_b)
                                    if (p[pixel[10]] < c_b)
                                        if (p[pixel[6]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[4]] < c_b)
                                                    if (p[pixel[3]] < c_b)
                                                        goto is_a_corner;
                                                    else if (p[pixel[11]] < c_b)
                                                        if (p[pixel[12]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[11]] < c_b)
                                                    if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b)
                                                        if (p[pixel[15]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[9]] > cb)
                        if (p[pixel[10]] > cb)
                            if (p[pixel[11]] > cb)
                                if (p[pixel[12]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[4]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[3]] > cb)
                                    if (p[pixel[4]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[9]] < c_b)
                        if (p[pixel[7]] < c_b)
                            if (p[pixel[8]] < c_b)
                                if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[6]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[4]] < c_b)
                                                    if (p[pixel[3]] < c_b)
                                                        goto is_a_corner;
                                                    else if (p[pixel[12]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if (p[pixel[1]] < c_b)
                    if (p[pixel[8]] > cb)
                        if (p[pixel[9]] > cb)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[3]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[2]] > cb)
                                    if (p[pixel[3]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[8]] < c_b)
                        if (p[pixel[7]] < c_b)
                            if (p[pixel[9]] < c_b)
                                if (p[pixel[6]] < c_b)
                                    if (p[pixel[5]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[3]] < c_b)
                                                if (p[pixel[2]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[10]] < c_b)
                                                    if (p[pixel[11]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[10]] < c_b)
                                                if (p[pixel[11]] < c_b)
                                                    if (p[pixel[12]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[10]] < c_b)
                                            if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[10]] < c_b)
                                        if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if (p[pixel[8]] > cb)
                    if (p[pixel[9]] > cb)
                        if (p[pixel[10]] > cb)
                            if (p[pixel[11]] > cb)
                                if (p[pixel[12]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[4]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[3]] > cb)
                                    if (p[pixel[4]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[2]] > cb)
                                if (p[pixel[3]] > cb)
                                    if (p[pixel[4]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if (p[pixel[8]] < c_b)
                    if (p[pixel[7]] < c_b)
                        if (p[pixel[9]] < c_b)
                            if (p[pixel[10]] < c_b)
                                if (p[pixel[6]] < c_b)
                                    if (p[pixel[5]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[3]] < c_b)
                                                if (p[pixel[2]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[11]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else if (p[pixel[0]] < c_b)
                if (p[pixel[1]] > cb)
                    if (p[pixel[8]] > cb)
                        if (p[pixel[7]] > cb)
                            if (p[pixel[9]] > cb)
                                if (p[pixel[6]] > cb)
                                    if (p[pixel[5]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[3]] > cb)
                                                if (p[pixel[2]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[10]] > cb)
                                                    if (p[pixel[11]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[10]] > cb)
                                                if (p[pixel[11]] > cb)
                                                    if (p[pixel[12]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[10]] > cb)
                                            if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[10]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[8]] < c_b)
                        if (p[pixel[9]] < c_b)
                            if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[3]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[2]] < c_b)
                                    if (p[pixel[3]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if (p[pixel[1]] < c_b)
                    if (p[pixel[2]] > cb)
                        if (p[pixel[9]] > cb)
                            if (p[pixel[7]] > cb)
                                if (p[pixel[8]] > cb)
                                    if (p[pixel[10]] > cb)
                                        if (p[pixel[6]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[4]] > cb)
                                                    if (p[pixel[3]] > cb)
                                                        goto is_a_corner;
                                                    else if (p[pixel[11]] > cb)
                                                        if (p[pixel[12]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[11]] > cb)
                                                    if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb)
                                                        if (p[pixel[15]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[9]] < c_b)
                            if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[3]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[2]] < c_b)
                        if (p[pixel[3]] > cb)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[7]] > cb)
                                    if (p[pixel[8]] > cb)
                                        if (p[pixel[9]] > cb)
                                            if (p[pixel[11]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[5]] > cb)
                                                        if (p[pixel[4]] > cb)
                                                            goto is_a_corner;
                                                        else if (p[pixel[12]] >
                                                                 cb)
                                                            if (p[pixel[13]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb)
                                                            if (p[pixel[14]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb)
                                                            if (p[pixel[15]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[3]] < c_b)
                            if (p[pixel[4]] > cb)
                                if (p[pixel[13]] > cb)
                                    if (p[pixel[7]] > cb)
                                        if (p[pixel[8]] > cb)
                                            if (p[pixel[9]] > cb)
                                                if (p[pixel[10]] > cb)
                                                    if (p[pixel[11]] > cb)
                                                        if (p[pixel[12]] > cb)
                                                            if (p[pixel[6]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [5]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else if (
                                                                    p[pixel
                                                                          [14]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else if (
                                                                p[pixel[14]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [15]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[13]] < c_b)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[5]] > cb)
                                    if (p[pixel[6]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[12]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[4]] < c_b)
                                if (p[pixel[5]] > cb)
                                    if (p[pixel[14]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[12]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [13]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [6]] >
                                                                        cb)
                                                                        goto is_a_corner;
                                                                    else if (
                                                                        p[pixel
                                                                              [15]] >
                                                                        cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[14]] < c_b)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [11]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [13]] >
                                                                        cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [11]] <
                                                                        c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[6]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[12]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [13]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[5]] < c_b)
                                    if (p[pixel[6]] > cb)
                                        if (p[pixel[15]] < c_b)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[7]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [11]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [12]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [14]] >
                                                                            cb)
                                                                            goto is_a_corner;
                                                                        else
                                                                            goto is_not_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[12]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [13]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [14]] >
                                                                        cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[6]] < c_b)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[8]] > cb)
                                                    if (p[pixel[9]] > cb)
                                                        if (p[pixel[10]] > cb)
                                                            if (p[pixel[11]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [13]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [15]] >
                                                                            cb)
                                                                            goto is_a_corner;
                                                                        else
                                                                            goto is_not_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[13]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[12]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [14]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [15]] >
                                                                        cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[12]] > cb)
                                    if (p[pixel[7]] > cb)
                                        if (p[pixel[8]] > cb)
                                            if (p[pixel[9]] > cb)
                                                if (p[pixel[10]] > cb)
                                                    if (p[pixel[11]] > cb)
                                                        if (p[pixel[13]] > cb)
                                                            if (p[pixel[14]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [6]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else if (
                                                                    p[pixel
                                                                          [15]] >
                                                                    cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[12]] < c_b)
                                    if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [11]] <
                                                                    c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[11]] > cb)
                                if (p[pixel[7]] > cb)
                                    if (p[pixel[8]] > cb)
                                        if (p[pixel[9]] > cb)
                                            if (p[pixel[10]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        if (p[pixel[6]] > cb)
                                                            if (p[pixel[5]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else if (
                                                                p[pixel[14]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else if (p[pixel[14]] >
                                                                 cb)
                                                            if (p[pixel[15]] >
                                                                cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[11]] < c_b)
                                if (p[pixel[12]] < c_b)
                                    if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[10]] > cb)
                            if (p[pixel[7]] > cb)
                                if (p[pixel[8]] > cb)
                                    if (p[pixel[9]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[5]] > cb)
                                                        if (p[pixel[4]] > cb)
                                                            goto is_a_corner;
                                                        else if (p[pixel[13]] >
                                                                 cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb)
                                                        if (p[pixel[15]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[10]] < c_b)
                            if (p[pixel[11]] < c_b)
                                if (p[pixel[12]] < c_b)
                                    if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[4]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[9]] > cb)
                        if (p[pixel[7]] > cb)
                            if (p[pixel[8]] > cb)
                                if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[6]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[4]] > cb)
                                                    if (p[pixel[3]] > cb)
                                                        goto is_a_corner;
                                                    else if (p[pixel[12]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if (p[pixel[9]] < c_b)
                        if (p[pixel[10]] < c_b)
                            if (p[pixel[11]] < c_b)
                                if (p[pixel[12]] < c_b)
                                    if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[4]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[3]] < c_b)
                                    if (p[pixel[4]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if (p[pixel[8]] > cb)
                    if (p[pixel[7]] > cb)
                        if (p[pixel[9]] > cb)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[6]] > cb)
                                    if (p[pixel[5]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[3]] > cb)
                                                if (p[pixel[2]] > cb)
                                                    goto is_a_corner;
                                                else if (p[pixel[11]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if (p[pixel[8]] < c_b)
                    if (p[pixel[9]] < c_b)
                        if (p[pixel[10]] < c_b)
                            if (p[pixel[11]] < c_b)
                                if (p[pixel[12]] < c_b)
                                    if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[4]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[3]] < c_b)
                                    if (p[pixel[4]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[2]] < c_b)
                                if (p[pixel[3]] < c_b)
                                    if (p[pixel[4]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[7]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else if (p[pixel[7]] > cb)
                if (p[pixel[8]] > cb)
                    if (p[pixel[9]] > cb)
                        if (p[pixel[6]] > cb)
                            if (p[pixel[5]] > cb)
                                if (p[pixel[4]] > cb)
                                    if (p[pixel[3]] > cb)
                                        if (p[pixel[2]] > cb)
                                            if (p[pixel[1]] > cb)
                                                goto is_a_corner;
                                            else if (p[pixel[10]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[10]] > cb)
                                            if (p[pixel[11]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[10]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[10]] > cb)
                            if (p[pixel[11]] > cb)
                                if (p[pixel[12]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else if (p[pixel[7]] < c_b)
                if (p[pixel[8]] < c_b)
                    if (p[pixel[9]] < c_b)
                        if (p[pixel[6]] < c_b)
                            if (p[pixel[5]] < c_b)
                                if (p[pixel[4]] < c_b)
                                    if (p[pixel[3]] < c_b)
                                        if (p[pixel[2]] < c_b)
                                            if (p[pixel[1]] < c_b)
                                                goto is_a_corner;
                                            else if (p[pixel[10]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if (p[pixel[10]] < c_b)
                                            if (p[pixel[11]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if (p[pixel[10]] < c_b)
                                        if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if (p[pixel[10]] < c_b)
                            if (p[pixel[11]] < c_b)
                                if (p[pixel[12]] < c_b)
                                    if (p[pixel[13]] < c_b)
                                        if (p[pixel[14]] < c_b)
                                            if (p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else
                goto is_not_a_corner;

        is_a_corner:
            bmin = threshold;
            goto end_if;

        is_not_a_corner:
            bmax = threshold;
            goto end_if;

        end_if:

            if (bmin == bmax - 1 || bmin == bmax)
                return bmin;
            threshold = (bmin + bmax) / 2;
        }
    }

    /**
     * @brief Store the initial detected features not on the stack to prevent a
     * potential stack overflow.
     */
    static Feature detected_features[512];

    void detect_nonmax(const uint8_t* image_buffer,
                       const uint16_t width,
                       const uint16_t height,
                       const uint16_t stride,
                       const uint8_t threshold,
                       Feature* out_feature_buffer,
                       const size_t feature_buffer_length,
                       size_t* out_number_of_features) {

        // TODO: feature_buffer_length should probably be passed during the
        // detect here

        size_t number_of_features;

        fast::detect(image_buffer,
                     width,
                     height,
                     stride,
                     threshold,
                     detected_features,
                     sizeof(detected_features) / sizeof(Feature),
                     &number_of_features);

        int scores[number_of_features];
        fast::score(image_buffer,
                    stride,
                    detected_features,
                    number_of_features,
                    threshold,
                    scores);

        fast::nonmax_suppression(detected_features,
                                 scores,
                                 number_of_features,
                                 out_feature_buffer,
                                 out_number_of_features);
    }

    void detect(const uint8_t* image_buffer,
                const uint16_t width,
                const uint16_t height,
                const uint16_t stride,
                const uint8_t threshold,
                Feature* out_feature_buffer,
                const size_t feature_buffer_length,
                size_t* out_number_of_features) {

        size_t number_of_features = 0;

        uint32_t pixel[16];
        uint16_t x, y;

        make_offsets(pixel, stride);

        for (y = 3; y < height - 3; y++) {
            for (x = 3; x < width - 3; x++) {
                const uint8_t* p = image_buffer + y * stride + x;

                int cb  = *p + threshold;
                int c_b = *p - threshold;
                if (p[pixel[0]] > cb)
                    if (p[pixel[1]] > cb)
                        if (p[pixel[2]] > cb)
                            if (p[pixel[3]] > cb)
                                if (p[pixel[4]] > cb)
                                    if (p[pixel[5]] > cb)
                                        if (p[pixel[6]] > cb)
                                            if (p[pixel[7]] > cb)
                                                if (p[pixel[8]] > cb) {
                                                } else if (p[pixel[15]] > cb) {
                                                } else
                                                    continue;
                                            else if (p[pixel[7]] < c_b)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else
                                                        continue;
                                                else if (p[pixel[14]] < c_b)
                                                    if (p[pixel[8]] < c_b)
                                                        if (p[pixel[9]] < c_b)
                                                            if (p[pixel[10]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [11]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [12]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [13]] <
                                                                            c_b)
                                                                            if (p[pixel
                                                                                      [15]] <
                                                                                c_b) {
                                                                            } else
                                                                                continue;
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[6]] < c_b)
                                            if (p[pixel[15]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb) {
                                                    } else
                                                        continue;
                                                else if (p[pixel[13]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [11]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [12]] <
                                                                            c_b)
                                                                            if (p[pixel
                                                                                      [14]] <
                                                                                c_b) {
                                                                            } else
                                                                                continue;
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[7]] < c_b)
                                                if (p[pixel[8]] < c_b)
                                                    if (p[pixel[9]] < c_b)
                                                        if (p[pixel[10]] < c_b)
                                                            if (p[pixel[11]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [13]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [14]] <
                                                                            c_b) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[13]] < c_b)
                                            if (p[pixel[7]] < c_b)
                                                if (p[pixel[8]] < c_b)
                                                    if (p[pixel[9]] < c_b)
                                                        if (p[pixel[10]] < c_b)
                                                            if (p[pixel[11]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [14]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [15]] <
                                                                            c_b) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[5]] < c_b)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [9]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [10]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [11]] >
                                                                            cb) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[12]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [11]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [13]] <
                                                                            c_b) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[14]] < c_b)
                                            if (p[pixel[7]] < c_b)
                                                if (p[pixel[8]] < c_b)
                                                    if (p[pixel[9]] < c_b)
                                                        if (p[pixel[10]] < c_b)
                                                            if (p[pixel[11]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [13]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [6]] <
                                                                            c_b) {
                                                                        } else if (
                                                                            p[pixel
                                                                                  [15]] <
                                                                            c_b) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[6]] < c_b)
                                            if (p[pixel[7]] < c_b)
                                                if (p[pixel[8]] < c_b)
                                                    if (p[pixel[9]] < c_b)
                                                        if (p[pixel[10]] < c_b)
                                                            if (p[pixel[11]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [13]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [11]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[12]] < c_b)
                                        if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[13]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [14]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [6]] <
                                                                        c_b) {
                                                                    } else if (
                                                                        p[pixel
                                                                              [15]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[4]] < c_b)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [9]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [10]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [9]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [10]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[11]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [12]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[13]] < c_b)
                                        if (p[pixel[7]] < c_b)
                                            if (p[pixel[8]] < c_b)
                                                if (p[pixel[9]] < c_b)
                                                    if (p[pixel[10]] < c_b)
                                                        if (p[pixel[11]] < c_b)
                                                            if (p[pixel[12]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [6]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [5]] <
                                                                        c_b) {
                                                                    } else if (
                                                                        p[pixel
                                                                              [14]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else if (
                                                                    p[pixel
                                                                          [14]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [15]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[5]] < c_b)
                                        if (p[pixel[6]] < c_b)
                                            if (p[pixel[7]] < c_b)
                                                if (p[pixel[8]] < c_b)
                                                    if (p[pixel[9]] < c_b)
                                                        if (p[pixel[10]] < c_b)
                                                            if (p[pixel[11]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [12]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[11]] < c_b)
                                    if (p[pixel[7]] < c_b)
                                        if (p[pixel[8]] < c_b)
                                            if (p[pixel[9]] < c_b)
                                                if (p[pixel[10]] < c_b)
                                                    if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] < c_b)
                                                            if (p[pixel[6]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [5]] <
                                                                    c_b) {
                                                                } else if (
                                                                    p[pixel
                                                                          [14]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else if (
                                                                p[pixel[14]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [15]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[3]] < c_b)
                                if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [9]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [9]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[4]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [9]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[10]] < c_b)
                                    if (p[pixel[7]] < c_b)
                                        if (p[pixel[8]] < c_b)
                                            if (p[pixel[9]] < c_b)
                                                if (p[pixel[11]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[5]] < c_b)
                                                            if (p[pixel[4]] <
                                                                c_b) {
                                                            } else if (
                                                                p[pixel[12]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [13]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else if (p[pixel[12]] <
                                                                 c_b)
                                                            if (p[pixel[13]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [14]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] < c_b)
                                                            if (p[pixel[14]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [15]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[10]] < c_b)
                                if (p[pixel[7]] < c_b)
                                    if (p[pixel[8]] < c_b)
                                        if (p[pixel[9]] < c_b)
                                            if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[5]] < c_b)
                                                            if (p[pixel[4]] <
                                                                c_b) {
                                                            } else if (
                                                                p[pixel[13]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else if (p[pixel[13]] <
                                                                 c_b)
                                                            if (p[pixel[14]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] < c_b)
                                                            if (p[pixel[15]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[2]] < c_b)
                            if (p[pixel[9]] > cb)
                                if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[4]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[3]] > cb)
                                            if (p[pixel[4]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb)
                                                            if (p[pixel[8]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[9]] < c_b)
                                if (p[pixel[7]] < c_b)
                                    if (p[pixel[8]] < c_b)
                                        if (p[pixel[10]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[4]] < c_b)
                                                        if (p[pixel[3]] < c_b) {
                                                        } else if (p[pixel
                                                                         [11]] <
                                                                   c_b)
                                                            if (p[pixel[12]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if (p[pixel[11]] < c_b)
                                                        if (p[pixel[12]] < c_b)
                                                            if (p[pixel[13]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[11]] < c_b)
                                                    if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] < c_b)
                                                            if (p[pixel[14]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] < c_b)
                                                            if (p[pixel[15]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[9]] > cb)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[3]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[9]] < c_b)
                            if (p[pixel[7]] < c_b)
                                if (p[pixel[8]] < c_b)
                                    if (p[pixel[10]] < c_b)
                                        if (p[pixel[11]] < c_b)
                                            if (p[pixel[6]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[4]] < c_b)
                                                        if (p[pixel[3]] < c_b) {
                                                        } else if (p[pixel
                                                                         [12]] <
                                                                   c_b) {
                                                        } else
                                                            continue;
                                                    else if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b)
                                                        if (p[pixel[15]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if (p[pixel[1]] < c_b)
                        if (p[pixel[8]] > cb)
                            if (p[pixel[9]] > cb)
                                if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[4]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[3]] > cb)
                                            if (p[pixel[4]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[2]] > cb)
                                        if (p[pixel[3]] > cb)
                                            if (p[pixel[4]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[7]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[8]] < c_b)
                            if (p[pixel[7]] < c_b)
                                if (p[pixel[9]] < c_b)
                                    if (p[pixel[6]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[4]] < c_b)
                                                if (p[pixel[3]] < c_b)
                                                    if (p[pixel[2]] < c_b) {
                                                    } else if (p[pixel[10]] <
                                                               c_b)
                                                        if (p[pixel[11]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[10]] < c_b)
                                                    if (p[pixel[11]] < c_b)
                                                        if (p[pixel[12]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[10]] < c_b)
                                                if (p[pixel[11]] < c_b)
                                                    if (p[pixel[12]] < c_b)
                                                        if (p[pixel[13]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[10]] < c_b)
                                            if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b)
                                                        if (p[pixel[14]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[10]] < c_b)
                                        if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b)
                                                        if (p[pixel[15]] <
                                                            c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if (p[pixel[8]] > cb)
                        if (p[pixel[9]] > cb)
                            if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[3]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[2]] > cb)
                                    if (p[pixel[3]] > cb)
                                        if (p[pixel[4]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if (p[pixel[8]] < c_b)
                        if (p[pixel[7]] < c_b)
                            if (p[pixel[9]] < c_b)
                                if (p[pixel[10]] < c_b)
                                    if (p[pixel[6]] < c_b)
                                        if (p[pixel[5]] < c_b)
                                            if (p[pixel[4]] < c_b)
                                                if (p[pixel[3]] < c_b)
                                                    if (p[pixel[2]] < c_b) {
                                                    } else if (p[pixel[11]] <
                                                               c_b) {
                                                    } else
                                                        continue;
                                                else if (p[pixel[11]] < c_b)
                                                    if (p[pixel[12]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b)
                                                    if (p[pixel[13]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if (p[pixel[0]] < c_b)
                    if (p[pixel[1]] > cb)
                        if (p[pixel[8]] > cb)
                            if (p[pixel[7]] > cb)
                                if (p[pixel[9]] > cb)
                                    if (p[pixel[6]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[4]] > cb)
                                                if (p[pixel[3]] > cb)
                                                    if (p[pixel[2]] > cb) {
                                                    } else if (p[pixel[10]] >
                                                               cb)
                                                        if (p[pixel[11]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[10]] > cb)
                                                    if (p[pixel[11]] > cb)
                                                        if (p[pixel[12]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[10]] > cb)
                                                if (p[pixel[11]] > cb)
                                                    if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[10]] > cb)
                                            if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[10]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb)
                                                        if (p[pixel[15]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[8]] < c_b)
                            if (p[pixel[9]] < c_b)
                                if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else if (p[pixel[6]] <
                                                               c_b)
                                                        if (p[pixel[7]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[4]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[3]] < c_b)
                                            if (p[pixel[4]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[2]] < c_b)
                                        if (p[pixel[3]] < c_b)
                                            if (p[pixel[4]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if (p[pixel[1]] < c_b)
                        if (p[pixel[2]] > cb)
                            if (p[pixel[9]] > cb)
                                if (p[pixel[7]] > cb)
                                    if (p[pixel[8]] > cb)
                                        if (p[pixel[10]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[4]] > cb)
                                                        if (p[pixel[3]] > cb) {
                                                        } else if (p[pixel
                                                                         [11]] >
                                                                   cb)
                                                            if (p[pixel[12]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if (p[pixel[11]] > cb)
                                                        if (p[pixel[12]] > cb)
                                                            if (p[pixel[13]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[11]] > cb)
                                                    if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb)
                                                            if (p[pixel[14]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb)
                                                            if (p[pixel[15]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[9]] < c_b)
                                if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else if (p[pixel[6]] <
                                                               c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[4]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[3]] < c_b)
                                            if (p[pixel[4]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[2]] < c_b)
                            if (p[pixel[3]] > cb)
                                if (p[pixel[10]] > cb)
                                    if (p[pixel[7]] > cb)
                                        if (p[pixel[8]] > cb)
                                            if (p[pixel[9]] > cb)
                                                if (p[pixel[11]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[5]] > cb)
                                                            if (p[pixel[4]] >
                                                                cb) {
                                                            } else if (
                                                                p[pixel[12]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [13]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else if (p[pixel[12]] >
                                                                 cb)
                                                            if (p[pixel[13]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [14]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb)
                                                            if (p[pixel[14]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [15]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else if (p[pixel[6]] <
                                                               c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [9]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [9]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[4]] < c_b)
                                                if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [9]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[3]] < c_b)
                                if (p[pixel[4]] > cb)
                                    if (p[pixel[13]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[12]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [6]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [5]] >
                                                                        cb) {
                                                                    } else if (
                                                                        p[pixel
                                                                              [14]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else if (
                                                                    p[pixel
                                                                          [14]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [15]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[13]] < c_b)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[5]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [12]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else if (p[pixel[6]] <
                                                               c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [9]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [10]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[5]] < c_b)
                                                    if (p[pixel[6]] < c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [9]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [10]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[5]] > cb)
                                        if (p[pixel[6]] > cb)
                                            if (p[pixel[7]] > cb)
                                                if (p[pixel[8]] > cb)
                                                    if (p[pixel[9]] > cb)
                                                        if (p[pixel[10]] > cb)
                                                            if (p[pixel[11]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[4]] < c_b)
                                    if (p[pixel[5]] > cb)
                                        if (p[pixel[14]] > cb)
                                            if (p[pixel[7]] > cb)
                                                if (p[pixel[8]] > cb)
                                                    if (p[pixel[9]] > cb)
                                                        if (p[pixel[10]] > cb)
                                                            if (p[pixel[11]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [13]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [6]] >
                                                                            cb) {
                                                                        } else if (
                                                                            p[pixel
                                                                                  [15]] >
                                                                            cb) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[14]] < c_b)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[6]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [11]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [13]] >
                                                                            cb) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else if (p[pixel[6]] <
                                                               c_b)
                                                        if (p[pixel[7]] < c_b)
                                                            if (p[pixel[8]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [9]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [10]] <
                                                                        c_b)
                                                                        if (p[pixel
                                                                                  [11]] <
                                                                            c_b) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[6]] > cb)
                                            if (p[pixel[7]] > cb)
                                                if (p[pixel[8]] > cb)
                                                    if (p[pixel[9]] > cb)
                                                        if (p[pixel[10]] > cb)
                                                            if (p[pixel[11]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [13]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[5]] < c_b)
                                        if (p[pixel[6]] > cb)
                                            if (p[pixel[15]] < c_b)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[7]] > cb)
                                                        if (p[pixel[8]] > cb)
                                                            if (p[pixel[9]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [10]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [11]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [12]] >
                                                                            cb)
                                                                            if (p[pixel
                                                                                      [14]] >
                                                                                cb) {
                                                                            } else
                                                                                continue;
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[13]] < c_b)
                                                    if (p[pixel[14]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[7]] > cb)
                                                if (p[pixel[8]] > cb)
                                                    if (p[pixel[9]] > cb)
                                                        if (p[pixel[10]] > cb)
                                                            if (p[pixel[11]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [13]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [14]] >
                                                                            cb) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[6]] < c_b)
                                            if (p[pixel[7]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[8]] > cb)
                                                        if (p[pixel[9]] > cb)
                                                            if (p[pixel[10]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [11]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [12]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [13]] >
                                                                            cb)
                                                                            if (p[pixel
                                                                                      [15]] >
                                                                                cb) {
                                                                            } else
                                                                                continue;
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[14]] < c_b)
                                                    if (p[pixel[15]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[7]] < c_b)
                                                if (p[pixel[8]] < c_b) {
                                                } else if (p[pixel[15]] < c_b) {
                                                } else
                                                    continue;
                                            else if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[13]] > cb)
                                            if (p[pixel[7]] > cb)
                                                if (p[pixel[8]] > cb)
                                                    if (p[pixel[9]] > cb)
                                                        if (p[pixel[10]] > cb)
                                                            if (p[pixel[11]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [12]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [14]] >
                                                                        cb)
                                                                        if (p[pixel
                                                                                  [15]] >
                                                                            cb) {
                                                                        } else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[12]] > cb)
                                        if (p[pixel[7]] > cb)
                                            if (p[pixel[8]] > cb)
                                                if (p[pixel[9]] > cb)
                                                    if (p[pixel[10]] > cb)
                                                        if (p[pixel[11]] > cb)
                                                            if (p[pixel[13]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [14]] >
                                                                    cb)
                                                                    if (p[pixel
                                                                              [6]] >
                                                                        cb) {
                                                                    } else if (
                                                                        p[pixel
                                                                              [15]] >
                                                                        cb) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b)
                                                                    if (p[pixel
                                                                              [11]] <
                                                                        c_b) {
                                                                    } else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[11]] > cb)
                                    if (p[pixel[7]] > cb)
                                        if (p[pixel[8]] > cb)
                                            if (p[pixel[9]] > cb)
                                                if (p[pixel[10]] > cb)
                                                    if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb)
                                                            if (p[pixel[6]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [5]] >
                                                                    cb) {
                                                                } else if (
                                                                    p[pixel
                                                                          [14]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else if (
                                                                p[pixel[14]] >
                                                                cb)
                                                                if (p[pixel
                                                                          [15]] >
                                                                    cb) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b)
                                                                if (p[pixel
                                                                          [10]] <
                                                                    c_b) {
                                                                } else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[10]] > cb)
                                if (p[pixel[7]] > cb)
                                    if (p[pixel[8]] > cb)
                                        if (p[pixel[9]] > cb)
                                            if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[6]] > cb)
                                                        if (p[pixel[5]] > cb)
                                                            if (p[pixel[4]] >
                                                                cb) {
                                                            } else if (
                                                                p[pixel[13]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else if (p[pixel[13]] >
                                                                 cb)
                                                            if (p[pixel[14]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb)
                                                            if (p[pixel[15]] >
                                                                cb) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b)
                                                            if (p[pixel[9]] <
                                                                c_b) {
                                                            } else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[9]] > cb)
                            if (p[pixel[7]] > cb)
                                if (p[pixel[8]] > cb)
                                    if (p[pixel[10]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[6]] > cb)
                                                if (p[pixel[5]] > cb)
                                                    if (p[pixel[4]] > cb)
                                                        if (p[pixel[3]] > cb) {
                                                        } else if (p[pixel
                                                                         [12]] >
                                                                   cb) {
                                                        } else
                                                            continue;
                                                    else if (p[pixel[12]] > cb)
                                                        if (p[pixel[13]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb)
                                                        if (p[pixel[14]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb)
                                                        if (p[pixel[15]] > cb) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if (p[pixel[9]] < c_b)
                            if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[3]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b)
                                                        if (p[pixel[8]] < c_b) {
                                                        } else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if (p[pixel[8]] > cb)
                        if (p[pixel[7]] > cb)
                            if (p[pixel[9]] > cb)
                                if (p[pixel[10]] > cb)
                                    if (p[pixel[6]] > cb)
                                        if (p[pixel[5]] > cb)
                                            if (p[pixel[4]] > cb)
                                                if (p[pixel[3]] > cb)
                                                    if (p[pixel[2]] > cb) {
                                                    } else if (p[pixel[11]] >
                                                               cb) {
                                                    } else
                                                        continue;
                                                else if (p[pixel[11]] > cb)
                                                    if (p[pixel[12]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb)
                                                    if (p[pixel[13]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb)
                                                    if (p[pixel[14]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb)
                                                    if (p[pixel[15]] > cb) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if (p[pixel[8]] < c_b)
                        if (p[pixel[9]] < c_b)
                            if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[3]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[2]] < c_b)
                                    if (p[pixel[3]] < c_b)
                                        if (p[pixel[4]] < c_b)
                                            if (p[pixel[5]] < c_b)
                                                if (p[pixel[6]] < c_b)
                                                    if (p[pixel[7]] < c_b) {
                                                    } else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if (p[pixel[7]] > cb)
                    if (p[pixel[8]] > cb)
                        if (p[pixel[9]] > cb)
                            if (p[pixel[6]] > cb)
                                if (p[pixel[5]] > cb)
                                    if (p[pixel[4]] > cb)
                                        if (p[pixel[3]] > cb)
                                            if (p[pixel[2]] > cb)
                                                if (p[pixel[1]] > cb) {
                                                } else if (p[pixel[10]] > cb) {
                                                } else
                                                    continue;
                                            else if (p[pixel[10]] > cb)
                                                if (p[pixel[11]] > cb) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[10]] > cb)
                                            if (p[pixel[11]] > cb)
                                                if (p[pixel[12]] > cb) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[10]] > cb)
                                        if (p[pixel[11]] > cb)
                                            if (p[pixel[12]] > cb)
                                                if (p[pixel[13]] > cb) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[10]] > cb)
                                    if (p[pixel[11]] > cb)
                                        if (p[pixel[12]] > cb)
                                            if (p[pixel[13]] > cb)
                                                if (p[pixel[14]] > cb) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[10]] > cb)
                                if (p[pixel[11]] > cb)
                                    if (p[pixel[12]] > cb)
                                        if (p[pixel[13]] > cb)
                                            if (p[pixel[14]] > cb)
                                                if (p[pixel[15]] > cb) {
                                                } else
                                                    continue;
                                            else

                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if (p[pixel[7]] < c_b)
                    if (p[pixel[8]] < c_b)
                        if (p[pixel[9]] < c_b)
                            if (p[pixel[6]] < c_b)
                                if (p[pixel[5]] < c_b)
                                    if (p[pixel[4]] < c_b)
                                        if (p[pixel[3]] < c_b)
                                            if (p[pixel[2]] < c_b)
                                                if (p[pixel[1]] < c_b) {
                                                } else if (p[pixel[10]] < c_b) {
                                                } else
                                                    continue;
                                            else if (p[pixel[10]] < c_b)
                                                if (p[pixel[11]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else if (p[pixel[10]] < c_b)
                                            if (p[pixel[11]] < c_b)
                                                if (p[pixel[12]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if (p[pixel[10]] < c_b)
                                        if (p[pixel[11]] < c_b)
                                            if (p[pixel[12]] < c_b)
                                                if (p[pixel[13]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if (p[pixel[10]] < c_b)
                                    if (p[pixel[11]] < c_b)
                                        if (p[pixel[12]] < c_b)
                                            if (p[pixel[13]] < c_b)
                                                if (p[pixel[14]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if (p[pixel[10]] < c_b)
                                if (p[pixel[11]] < c_b)
                                    if (p[pixel[12]] < c_b)
                                        if (p[pixel[13]] < c_b)
                                            if (p[pixel[14]] < c_b)
                                                if (p[pixel[15]] < c_b) {
                                                } else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else
                    continue;
                if (number_of_features == feature_buffer_length) {
                    *out_number_of_features = number_of_features;
                    return;
                    // rsize *= 2;
                    // out_feature_buffer = (xy*)realloc(out_feature_buffer,
                    // sizeof(xy) * rsize);
                }
                out_feature_buffer[number_of_features].x = x;
                out_feature_buffer[number_of_features].y = y;
                number_of_features++;
            }
        }

        *out_number_of_features = number_of_features;
    }

    void score(const uint8_t* image_buffer,
               const uint16_t stride,
               const Feature* features,
               const size_t number_of_features,
               const uint8_t threshold,
               int* out_scores) {

        uint32_t pixel[16];
        make_offsets(pixel, stride);

        for (size_t n = 0; n < number_of_features; n++) {
            out_scores[n] = corner_score(image_buffer + features[n].y * stride +
                                             features[n].x,
                                         pixel,
                                         threshold);
        }
    }

    void nonmax_suppression(const Feature* features,
                            const int* scores,
                            const size_t number_of_features,
                            Feature* out_feature_buffer,
                            size_t* out_number_of_features) {
        int num_nonmax = 0;
        size_t last_row;
        size_t i, j;
        const size_t sz = number_of_features;

        /*Point above points (roughly) to the pixel above the one of interest,
    if there is a feature there.*/
        size_t point_above = 0;
        size_t point_below = 0;

        if (number_of_features < 1) {
            *out_number_of_features = 0;
            return;
        }

        /* Find where each row begins
           (the features are output in raster scan order). A beginning of -1
           signifies that there are no features on that row. */
        last_row = features[number_of_features - 1].y;
        int row_start[(last_row + 1)];

        for (i = 0; i < last_row + 1; i++) row_start[i] = -1;

        {
            int prev_row = -1;
            for (i = 0; i < number_of_features; i++)
                if (features[i].y != prev_row) {
                    row_start[features[i].y] = i;
                    prev_row                 = features[i].y;
                }
        }

        for (i = 0; i < sz; i++) {
            int score   = scores[i];
            Feature pos = features[i];

            /*Check left */
            if (i > 0)
                if (features[i - 1].x == pos.x - 1 &&
                    features[i - 1].y == pos.y && Compare(scores[i - 1], score))
                    continue;

            /*Check right*/
            if (i < (sz - 1))
                if (features[i + 1].x == pos.x + 1 &&
                    features[i + 1].y == pos.y && Compare(scores[i + 1], score))
                    continue;

            /*Check above (if there is a valid row above)*/
            if (pos.y != 0 && row_start[pos.y - 1] != -1) {
                /*Make sure that current point_above is one
                  row above.*/
                if (features[point_above].y < pos.y - 1)
                    point_above = row_start[pos.y - 1];

                /*Make point_above point to the first of the pixels above the
                  current point, if it exists.*/
                for (; features[point_above].y < pos.y &&
                       features[point_above].x < pos.x - 1;
                     point_above++) {}

                for (j = point_above;
                     features[j].y < pos.y && features[j].x <= pos.x + 1;
                     j++) {
                    int x = features[j].x;
                    if ((x == pos.x - 1 || x == pos.x || x == pos.x + 1) &&
                        Compare(scores[j], score))
                        goto cont;
                }
            }

            /*Check below (if there is anything below)*/
            if (pos.y != last_row && row_start[pos.y + 1] != -1 &&
                point_below < sz) /*Nothing below*/
            {
                if (features[point_below].y < pos.y + 1)
                    point_below = row_start[pos.y + 1];

                /* Make point below point to one of the pixels belowthe current
                   point, if it exists.*/
                for (;
                     point_below < sz && features[point_below].y == pos.y + 1 &&
                     features[point_below].x < pos.x - 1;
                     point_below++) {}

                for (j = point_below; j < sz && features[j].y == pos.y + 1 &&
                                      features[j].x <= pos.x + 1;
                     j++) {
                    int x = features[j].x;
                    if ((x == pos.x - 1 || x == pos.x || x == pos.x + 1) &&
                        Compare(scores[j], score))
                        goto cont;
                }
            }

            out_feature_buffer[num_nonmax++] = features[i];
        cont:;
        }

        *out_number_of_features = num_nonmax;
    }
}
