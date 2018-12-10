/*
 * ========================= config.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   配置 参数
 * ----------------------------
 */

#ifndef _TPR_CONFIG_H_
#define _TPR_CONFIG_H_

//-- window 显示尺寸（像素）--
/*
#define SCR_WIDTH  880
#define SCR_HEIGHT  660
*/

#define SCR_WIDTH  1024
#define SCR_HEIGHT 1024


//-- 游戏工作尺寸（像素）--
//- 由于是 pixel游戏，所以这个值会很小。
//- 这个值 也是 Camera 的尺寸
/*
#define PIX_TIMES 2
#define WORK_WIDTH  (160 * PIX_TIMES)
#define WORK_HEIGHT (120 * PIX_TIMES)
*/

#define PIX_TIMES 2
#define WORK_WIDTH  (128 * PIX_TIMES)
#define WORK_HEIGHT (128 * PIX_TIMES)




//-- canvas texture 是绑定现成 图片文件，还是绑定 代码生成的 png-data
//-- 0 -- 绑定 现成的 图片文件
//-- 1 -- 绑定 代码生成的 png data
#define IS_USE_SELF_PNG_DATA 0 




#endif






