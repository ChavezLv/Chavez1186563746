/**
 * @file hal_memory.h
 * @brief memory alloc/free interface
 * @author ma.dengyun@intellif.com
 * @version 1.0.0
 * @date 2019/12/25
 * @copyright Intellif
 */

#ifndef HAL_MEMORY_H_
#define HAL_MEMORY_H_

#include <stddef.h>
#include <core/de_c_type.h>

namespace de {
	namespace mem {
	
		/** 
		 * @brief Allocate memory in DDR (physical address continuous)
		 * @param size          required memory size
		 * @param alignment     required memory address alignment
		 * @return              virutal address of memory allocated
		 */
		DE_DLL void* Alloc(size_t size, size_t alignment = 16);

		/**
		 *  @brief Free memory
		 *	@param ptr virutal address of memory to be freed
		 */
		DE_DLL void Free(void* ptr);

		/**
		  *  @brief 获取空闲内存的大小
		  *  @return 空闲内存大小，单位为byte
		  */
		DE_DLL size_t GetFreeMemSize(void);

		/**
		  *  @brief 获取总内存的大小
		  *  @return 总内存大小，单位为byte
		  */
		DE_DLL size_t GetTotalMemSize(void);
		
		/** 
         * @brief Allocate memory in SRAM (physical address continuous)
         * @param size          required SRAM size
         * @param alignment     required SRAM address alignment
         * @return              virutal address of SRAM allocated
         */
        DE_DLL void* AllocSram(size_t size, size_t alignment = 16);

        /**
         *  @brief Free SRAM
         *  @param ptr virutal address of SRAM to be freed
        */
        DE_DLL void FreeSram(void* ptr);


		/**
		 * @brief Allocate shadow memory (physical address coninuous, no virtual address)
		 * @param size          required memory size
		 * @param alignment     required mdmory address alignment
		 * @return              physical address of shadow memory allocated
		*/
		DE_DLL void* AllocShadow(size_t size, size_t alignment = 16);


		/** 
		 * @brief Free shadow memory
		 * @param ptr physical address of shadow memory to be freed	
		*/
		DE_DLL void FreeShadow(void* ptr);

		/**
		 * @brief Get physical address by user virtual address
		 * @param ptr user virtual address
		 * @return physical address mapped with ptr
		 * @note ptr can be an address on DDR or SRAM
		*/
		DE_DLL void* GetPhyAddr(void* ptr);

        /**
		 * @brief Get user virtual address by physical address
		 * @param ptr physcial address
		 * @return user virtual address
		 * @note ptr can be an address on DDR or SRAM
		*/
		DE_DLL void* GetVirtualAddr(void* ptr);


		/**
		 * @brief Shrink buffer
		 * @param ptr       user virtual address
		 * @param size      required size after shrinking
		 * @return          shrinked buffer ptr (virtual address)
		 * @note carefule with efficiency,  includes memory allocation(new), copy and free(old)
		*/
		DE_DLL void* Shrink(void* ptr, size_t size); 
	}
}
#endif

