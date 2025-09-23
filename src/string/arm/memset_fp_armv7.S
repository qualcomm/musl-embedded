/*
 * Copyright (c) 2020 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * Based on AOSP memset.
 * The Android Open Source Project license notifications and license are
 * retained for attribution purposes only.
 */

/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

    .text
    .syntax unified
    .global explicit_memset
    .type explicit_memset,%function

    .global memset
#ifdef VISIBILITY_HIDDEN
    .hidden memset
#endif
    .p2align 2
    .type memset,%function
#if defined(__thumb2__)
    .code 16
    .thumb_func
#else
    .code 32
#endif
explicit_memset:
memset:
        stmfd       sp!, {r0, lr}

        // Compute the offset to align the destination.
        // offset = (4-(src&3))&3 = -src & 3
        rsb         r3, r0, #0
        and         r3, r3, #3
        cmp         r3, r2
        it hi
        movhi       r3, r2

        // Splat r1.
        uxtb        r1, r1
        orr         r1, r1, r1, lsl #8
        orr         r1, r1, r1, lsl #16

        // Store one, two or three bytes.
        movs        r12, r3, lsl #31
        it mi
        strbmi      r1, [r0], #1
        it cs
        strhcs      r1, [r0], #2
        subs        r2, r2, r3

        // Set up d-registers for vstm.
        vmov d0, r1, r1
        vmov d1, r1, r1
        vmov.f64 d2, d0
        vmov.f64 d3, d1

        // Jump over the vector store loop if we have less than 64 bytes left.
        subs        r2, r2, #64
        bcc         2f

1:
        // Store 16 words.
        vstmia      r0!, {d0, d1, d2, d3}
        vstmia      r0!, {d0, d1, d2, d3}
        subs        r2, r2, #64
        bcs         1b

2:
        // Conditionally store 0 to 63 bytes.
        movs        r2, r2, lsl #27
        it cs
        vstmiacs    r0!, {d0, d1, d2, d3}
        it mi
        vstmiami    r0!, {d0, d1}

        movs        r2, r2, lsl #2
        it cs
        vstmiacs    r0!, {d0}
        it mi
        strmi       r1, [r0], #4

        movs        r2, r2, lsl #2
        it cs
        strhcs      r1, [r0], #2
        it mi
        strbmi      r1, [r0]

        pop         {r0, pc}
