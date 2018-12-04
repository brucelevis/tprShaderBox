/*
 * ========================= tprBinary.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.17
 *                                        修改 -- 2018.10.17
 * ----------------------------------------------------------
 * 
 *   常用的 二进制 处理函数
 * 
 * ----------------------------
 * 依赖的其它库：
 *        tprDataType
 * ----------------------------
 */
#ifndef _TPR_BINARY_H_
#define _TPR_BINARY_H_

//------------ C -------------//


//------------ CPP -------------//
#include <string>
#include <vector> 
#include <iostream> 

//------------ self -------------//
#include "tprDataType.h"



namespace tpr { //--------------- namespace: tpr -------------------//

//------ tprbinary.cpp --------
const std::vector<u8> sum( const std::vector<u8> &_a, const std::vector<u8> &_b );
const std::vector<u8> sum( const u64 _a, const u64 _b );

const u32 sum_wrapping( const u32 _a, const u32 _b );
const u64 sum_wrapping( const u64 _a, const u64 _b );

const std::string u8_2_hexStr( u8 _v, bool _is_cap );

size_t get_num_digit( size_t _num, bool _is_hex = false );

const std::string memaddr_2_hexStr( u64 _memaddr, bool _is_cap = true );


//------ binary_2_charStr.cpp --------
const std::string xxd( void *_buf, size_t _len, 
                        bool _is_memaddr = false,
                        bool _is_cap = true, 
                        bool _is_line_bytes_16 = true
                        );





}//------------------- namespace: tpr ------------------------//
#endif
