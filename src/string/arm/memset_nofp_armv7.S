/*
 * Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * Not a Contribution.
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
#if !defined(__thumb2__) && defined(__thumb__)
// Thumb1 port of nofp memset.  It's suprisingly similar; mostly just
// replacing conditional execution with branches and switching around
// the registers a bit.
explicit_memset:
memset:
        push {r4, r5, lr}

        // Compute the offset to align the destination.
        // offset = (4-(src&3))&3 = -src & 3
        rsbs r3, r0, #0
        movs r4, #3
        ands r3, r3, r4
        // Limit stores if count is small.
        cmp r3, r2
        bls 1f
        subs r4, r2, #1
        bcc 10f
        ands r3, r3, r4
1:
        // Splat r1.
        movs r4, #255
        ands r1, r1, r4
        lsls r4, r1, #8
        orrs r1, r1, r4
        lsls r4, r1, #16
        orrs r1, r1, r4

        // Store one, two or three bytes.
        lsls r4, r3, #31
        bpl 2f
        strb r1, [r0]
        adds r0, #1
        lsls r4, r3, #31
2:
        bcc 3f
        strh r1, [r0]
        adds r0, #2
3:
        subs r2, r2, r3

        // Set up registers for stm
        movs r3, r1
        movs r4, r1
        movs r5, r1

        // Store eight words.
        subs r2, r2, #32
        bcc 5f
4:
        stmia r0!, {r1,r3,r4,r5}
        stmia r0!, {r1,r3,r4,r5}
        subs r2, r2, #32
        bcs 4b

5:
        // Conditionally store 0 to 31 bytes.
        lsls r2, r2, #28
        bcc 6f
        stm r0!, {r1,r3,r4,r5}
6:
        bpl 7f
        stm r0!, {r1,r3}
7:
        lsls r2, r2, #2
        bcc 8f
        stm r0!, {r1}
8:
        bpl 9f
        strh r1, [r0]
        adds r0, #2
9:
        lsls r2, r2, #2
        bcc 10f
        strb r1, [r0]
10:
        pop {r4, r5, pc}
#else
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
        bls         2f
        subs        r4, r2, #1
        bcc         3f
        and         r3, r3, r4
2:
        // Splat r1.
        and         r1, r1, #255
        orr         r1, r1, r1, lsl #8
        orr         r1, r1, r1, lsl #16

        // Store one, two or three bytes.
        movs        r12, r3, lsl #31
        it mi
        strbmi      r1, [r0], #1
        it cs
        strhcs      r1, [r0], #2
        subs        r2, r2, r3

        // We will use only r3, r12 and lr later on. So set them here.
        mov         r3, r1
        mov         r12, r1
        mov         lr, r1

        // Store eight words.
        subs        r2, r2, #32
1:      ittt cs
        stmiacs     r0!, {r1,r3,r12,lr}
        stmiacs     r0!, {r1,r3,r12,lr}
        subscs      r2, r2, #32
        bcs         1b

        // Conditionally store 0 to 31 bytes.
        movs        r2, r2, lsl #28
        it cs
        stmcs       r0!, {r1,r3,r12,lr}
        it mi
        stmmi       r0!, {r1, lr}
        movs        r2, r2, lsl #2
        it cs
        strcs       r1, [r0], #4
        it mi
        strhmi      r1, [r0], #2
        movs        r2, r2, lsl #2
        it cs
        strbcs      r1, [r0]
3:
        pop         {r0, pc}
#endif
