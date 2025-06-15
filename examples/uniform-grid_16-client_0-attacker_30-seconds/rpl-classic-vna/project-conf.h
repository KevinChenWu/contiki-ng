/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#define ENERGEST_CONF_ON 1
#define ENERGEST_TYPE 2 // 1: Energest, 2: Simple Energest

#define UIP_CONF_BUFFER_SIZE 512

#define DIO_VERSION 1
#define SHIFT_DIO_VERSION 0
//#ifdef DIO_VERSION
//  #define DIO_VERSION_LEN 3
//  #define DIO_VERSION_SZ sizeof(char) * (DIO_VERSION_LEN + 1)
//  #define DIO_VERSION_IDX 6
//  #define DIO_VERSION_FLG_LEN 1
//  #define DIO_VERSION_FLG_SZ sizeof(char) * (DIO_VERSION_FLG_LEN + 1)
//  #define DIO_VERSION_FLG_IDX 0
//  #define DIO_VERSION_FLG_MSK 0x8
//#endif
#define DIO_VERSION_LEN 3
#define DIO_VERSION_FLG_MSK (0x80000 >> SHIFT_DIO_VERSION)

#define DIO_RANK 1
#define SHIFT_DIO_RANK (SHIFT_DIO_VERSION + DIO_RANK)
//#ifdef DIO_RANK
//  #define DIO_RANK_LEN 5
//  #define DIO_RANK_SZ sizeof(char) * (DIO_RANK_LEN + 1)
//  #define DIO_RANK_IDX 10
//  #define DIO_RANK_FLG_LEN 1
//  #define DIO_RANK_FLG_SZ sizeof(char) * (DIO_RANK_FLG_LEN + 1)
//  #define DIO_RANK_FLG_IDX 0
//  #define DIO_RANK_FLG_MSK 0x4
//#endif
#define DIO_RANK_LEN 5
#define DIO_RANK_FLG_MSK (0x80000 >> SHIFT_DIO_RANK)

#define FRAME_LEN 1
#define SHIFT_FRAME_LEN (SHIFT_DIO_RANK + FRAME_LEN)
//#ifdef FRAME_LEN
//  #define FRAME_LEN_LEN 3
//  #define FRAME_LEN_SZ sizeof(char) * (FRAME_LEN_LEN + 1)
//  #define FRAME_LEN_IDX 16
//  #define FRAME_LEN_FLG_LEN 1
//  #define FRAME_LEN_FLG_SZ sizeof(char) * (FRAME_LEN_FLG_LEN + 1)
//  #define FRAME_LEN_FLG_IDX 0
//  #define FRAME_LEN_FLG_MSK 0x2
//#endif
#define FRAME_LEN_LEN 3
#define FRAME_LEN_FLG_MSK (0x80000 >> SHIFT_FRAME_LEN)

#define SIXLOWPAN_SRC 1
#define SHIFT_SIXLOWPAN_SRC (SHIFT_FRAME_LEN + SIXLOWPAN_SRC)
//#ifdef SIXLOWPAN_SRC
//  #define SIXLOWPAN_SRC_LEN 17
//  #define SIXLOWPAN_SRC_SZ sizeof(char) * (SIXLOWPAN_SRC_LEN + 1)
//  #define SIXLOWPAN_SRC_IDX 20
//  #define SIXLOWPAN_SRC_FLG_LEN 1
//  #define SIXLOWPAN_SRC_FLG_SZ sizeof(char) * (SIXLOWPAN_SRC_FLG_LEN + 1)
//  #define SIXLOWPAN_SRC_FLG_IDX 0
//  #define SIXLOWPAN_SRC_FLG_MSK 0x1
//#endif
#define SIXLOWPAN_SRC_LEN 8
#define SIXLOWPAN_SRC_FLG_MSK (0x80000 >> SHIFT_SIXLOWPAN_SRC)

#define SIXLOWPAN_DST 1
#define SHIFT_SIXLOWPAN_DST (SHIFT_SIXLOWPAN_SRC + SIXLOWPAN_DST)
//#ifdef SIXLOWPAN_DST
//  #define SIXLOWPAN_DST_LEN 17
//  #define SIXLOWPAN_DST_SZ sizeof(char) * (SIXLOWPAN_DST_LEN + 1)
//  #define SIXLOWPAN_DST_IDX 38
//  #define SIXLOWPAN_DST_FLG_LEN 1
//  #define SIXLOWPAN_DST_FLG_SZ sizeof(char) * (SIXLOWPAN_DST_FLG_LEN + 1)
//  #define SIXLOWPAN_DST_FLG_IDX 1
//  #define SIXLOWPAN_DST_FLG_MSK 0x8
//#endif
#define SIXLOWPAN_DST_LEN 8
#define SIXLOWPAN_DST_FLG_MSK (0x80000 >> SHIFT_SIXLOWPAN_DST)

#define DIO_DTSN 1
#define SHIFT_DIO_DTSN (SHIFT_SIXLOWPAN_DST + DIO_DTSN)
//#ifdef DIO_DTSN
//  #define DIO_DTSN_LEN 3
//  #define DIO_DTSN_SZ sizeof(char) * (DIO_DTSN_LEN + 1)
//  #define DIO_DTSN_IDX 56
//  #define DIO_DTSN_FLG_LEN 1
//  #define DIO_DTSN_FLG_SZ sizeof(char) * (DIO_DTSN_FLG_LEN + 1)
//  #define DIO_DTSN_FLG_IDX 1
//  #define DIO_DTSN_FLG_MSK 0x4
//#endif
#define DIO_DTSN_LEN 3
#define DIO_DTSN_FLG_MSK (0x80000 >> SHIFT_DIO_DTSN)

#define DAO_SEQUENCE 1
#define SHIFT_DAO_SEQUENCE (SHIFT_DIO_DTSN + DAO_SEQUENCE)
//#ifdef DAO_SEQUENCE
//  #define DAO_SEQUENCE_LEN 3
//  #define DAO_SEQUENCE_SZ sizeof(char) * (DAO_SEQUENCE_LEN + 1)
//  #define DAO_SEQUENCE_IDX 60
//  #define DAO_SEQUENCE_FLG_LEN 1
//  #define DAO_SEQUENCE_FLG_SZ sizeof(char) * (DAO_SEQUENCE_FLG_LEN + 1)
//  #define DAO_SEQUENCE_FLG_IDX 1
//  #define DAO_SEQUENCE_FLG_MSK 0x2
//#endif
#define DAO_SEQUENCE_LEN 3
#define DAO_SEQUENCE_FLG_MSK (0x80000 >> SHIFT_DAO_SEQUENCE)

#define IPV6_HLIM 1
#define SHIFT_IPV6_HLIM (SHIFT_DAO_SEQUENCE + IPV6_HLIM)
//#ifdef IPV6_HLIM
//  #define IPV6_HLIM_LEN 3
//  #define IPV6_HLIM_SZ sizeof(char) * (IPV6_HLIM_LEN + 1)
//  #define IPV6_HLIM_IDX 64
//  #define IPV6_HLIM_FLG_LEN 1
//  #define IPV6_HLIM_FLG_SZ sizeof(char) * (IPV6_HLIM_FLG_LEN + 1)
//  #define IPV6_HLIM_FLG_IDX 1
//  #define IPV6_HLIM_FLG_MSK 0x1
//#endif
#define IPV6_HLIM_LEN 3
#define IPV6_HLIM_FLG_MSK (0x80000 >> SHIFT_IPV6_HLIM)

#define WPAN_SEQ_NO 1
#define SHIFT_WPAN_SEQ_NO (SHIFT_IPV6_HLIM + WPAN_SEQ_NO)
//#ifdef WPAN_SEQ_NO
//  #define WPAN_SEQ_NO_LEN 3
//  #define WPAN_SEQ_NO_SZ sizeof(char) * (WPAN_SEQ_NO_LEN + 1)
//  #define WPAN_SEQ_NO_IDX 68
//  #define WPAN_SEQ_NO_FLG_LEN 1
//  #define WPAN_SEQ_NO_FLG_SZ sizeof(char) * (WPAN_SEQ_NO_FLG_LEN + 1)
//  #define WPAN_SEQ_NO_FLG_IDX 2
//  #define WPAN_SEQ_NO_FLG_MSK 0x8
//#endif
#define WPAN_SEQ_NO_LEN 3
#define WPAN_SEQ_NO_FLG_MSK (0x80000 >> SHIFT_WPAN_SEQ_NO)

#define IPV6_PLEN 1
#define SHIFT_IPV6_PLEN (SHIFT_WPAN_SEQ_NO + IPV6_PLEN)
//#ifdef IPV6_PLEN
//  #define IPV6_PLEN_LEN 3
//  #define IPV6_PLEN_SZ sizeof(char) * (IPV6_PLEN_LEN + 1)
//  #define IPV6_PLEN_IDX 72
//  #define IPV6_PLEN_FLG_LEN 1
//  #define IPV6_PLEN_FLG_SZ sizeof(char) * (IPV6_PLEN_FLG_LEN + 1)
//  #define IPV6_PLEN_FLG_IDX 2
//  #define IPV6_PLEN_FLG_MSK 0x4
//#endif
#define IPV6_PLEN_LEN 3
#define IPV6_PLEN_FLG_MSK (0x80000 >> SHIFT_IPV6_PLEN)

#define ICMPV6_TYPE 0
#define SHIFT_ICMPV6_TYPE (SHIFT_IPV6_PLEN + ICMPV6_TYPE)
//#ifdef ICMPV6_TYPE
//  #define ICMPV6_TYPE_LEN 3
//  #define ICMPV6_TYPE_SZ sizeof(char) * (ICMPV6_TYPE_LEN + 1)
//  #define ICMPV6_TYPE_IDX 76
//  #define ICMPV6_TYPE_FLG_LEN 1
//  #define ICMPV6_TYPE_FLG_SZ sizeof(char) * (ICMPV6_TYPE_FLG_LEN + 1)
//  #define ICMPV6_TYPE_FLG_IDX 2
//  #define ICMPV6_TYPE_FLG_MSK 0x2
//#endif
#define ICMPV6_TYPE_LEN 3
#define ICMPV6_TYPE_FLG_MSK (0x80000 >> SHIFT_ICMPV6_TYPE)

#define ICMPV6_CODE 1
#define SHIFT_ICMPV6_CODE (SHIFT_ICMPV6_TYPE + ICMPV6_CODE)
//#ifdef ICMPV6_CODE
//  #define ICMPV6_CODE_LEN 3
//  #define ICMPV6_CODE_SZ sizeof(char) * (ICMPV6_CODE_LEN + 1)
//  #define ICMPV6_CODE_IDX 80
//  #define ICMPV6_CODE_FLG_LEN 1
//  #define ICMPV6_CODE_FLG_SZ sizeof(char) * (ICMPV6_CODE_FLG_LEN + 1)
//  #define ICMPV6_CODE_FLG_IDX 2
//  #define ICMPV6_CODE_FLG_MSK 0x1
//#endif
#define ICMPV6_CODE_LEN 3
#define ICMPV6_CODE_FLG_MSK (0x80000 >> SHIFT_ICMPV6_CODE)

#define WPAN_ACK_REQUEST 1
#define SHIFT_WPAN_ACK_REQUEST (SHIFT_ICMPV6_CODE + WPAN_ACK_REQUEST)
//#ifdef WPAN_ACK_REQUEST
//  #define WPAN_ACK_REQUEST_LEN 1
//  #define WPAN_ACK_REQUEST_SZ sizeof(char) * (WPAN_ACK_REQUEST_LEN + 1)
//  #define WPAN_ACK_REQUEST_IDX 84
//  #define WPAN_ACK_REQUEST_FLG_LEN 1
//  #define WPAN_ACK_REQUEST_FLG_SZ sizeof(char) * (WPAN_ACK_REQUEST_FLG_LEN + 1)
//  #define WPAN_ACK_REQUEST_FLG_IDX 3
//  #define WPAN_ACK_REQUEST_FLG_MSK 0x8
//#endif
#define WPAN_ACK_REQUEST_LEN 1
#define WPAN_ACK_REQUEST_FLG_MSK (0x80000 >> SHIFT_WPAN_ACK_REQUEST)


#endif /* PROJECT_CONF_H_ */
