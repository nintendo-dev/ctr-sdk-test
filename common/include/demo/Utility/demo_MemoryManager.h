/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C) Nintendo Co., Ltd. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 58466 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_MEMORY_MANAGER_H_
#define DEMO_MEMORY_MANAGER_H_

#include "demo/Utility/demo_Utility.h"

namespace demo {

namespace detail {
	/*!
		@brief Manages memory regions allocated for graphics.

			   This class allocates or frees regions of main memory or VRAM as needed.
	*/

	class MemoryManager : private nn::util::NonCopyable<MemoryManager>
	{
	public:
		/*!
			@brief Constructor.
		*/
		MemoryManager(void);

		/*!
		@brief Destructor.
		*/
		virtual ~MemoryManager();

		/*!
			@brief Initializes the memory manager class.

					  This function uses the memory region specified by the argument to allocate the buffers needed for DMPGL initialization
		   and rendering. It must be called once in advance.

					  Subsequent calls are ignored unless the class is destroyed by a call to <tt>Finalize</tt>.

			@param[in] fcramAddress  Specifies the start address of the memory region in FCRAM used for graphics.
			@param[in] memorySize  Specifies the size of the memory region used for graphics.
		*/
		void Initialize(const uptr fcramAddress, const size_t memorySize);

		/*!
			@brief Finalizes the memory manager class instance.
		*/
		void Finalize(void);

		/*!
			@brief Allocates the memory region managed by this class.

			@param[in] area  Specifies the memory region to allocate.
			@param[in] aim  Specifies the purpose of the allocated buffer.
			@param[in] id  Specifies the ID of the buffer to allocate.
			@param[in] size  Specifies the size of the buffer to allocate.

			@return Returns the address of the allocated buffer.
		*/
		void* Allocate(GLenum area, GLenum aim, GLuint id, GLsizei size);

		/*!
			@brief Frees the memory region allocated by this class.

					  Note that the region is not freed if <tt>area</tt> specifies VRAM.

			@param[in] area  Specifies the memory region that contains the buffer to free.
			@param[in] aim  Specifies the purpose of the buffer to free.
			@param[in] id  Specifies the ID of the buffer to free.
			@param[in] addr  Specifies the address of the buffer to free.
		*/
		void Deallocate(GLenum area, GLenum aim, GLuint id, void* addr);

		/*!
			@brief Sends the current free space and maximum space of main memory and VRAM to debug output.
		*/
		void PrintFreeMemorySize(void);

	private:
		bool			 m_Initialized;
		u8				 m_Pad[3];
		uptr			 m_pStartAddrFcram;
		uptr			 m_CurrentAddrVramA;
		uptr			 m_CurrentAddrVramB;
		nn::fnd::ExpHeap m_HeapOnFcram;
		size_t			 m_AllocatedBlockSize;
	};
}  // namespace detail

/*!
	@brief Initializes the memory manager class.

	@param[in] fcramAddress  Specifies the start address of the memory region in FCRAM used for graphics.
	@param[in] memorySize  Specifies the size of the memory region used for graphics.
*/
void InitializeMemoryManager(const uptr fcramAddress, const size_t memorySize);

/*!
	@brief Finalizes the memory manager.
*/
void FinalizeMemoryManager(void);

/*!
	@brief Sends the current free space and maximum space of main memory and VRAM to debug output.
*/
void PrintMemoryManagerInfo(void);

/*!
	@brief Gets the memory allocator specified as an argument to the <tt>nngxInitialize</tt> function

	@param[in] area  Specifies the memory region to allocate.
	@param[in] aim  Specifies the purpose of the allocated buffer.
	@param[in] id  Specifies the ID of the buffer to allocate.
	@param[in] size  Specifies the size of the buffer to allocate.

	@return Returns the address of the allocated buffer.
*/
void* GetAllocator(GLenum area, GLenum aim, GLuint id, GLsizei size);

/*!
	@brief Gets the memory deallocator specified as an argument to the <tt>nngxInitialize</tt> function.

	@param[in] area  Specifies the memory region that contains the buffer to free.
	@param[in] aim  Specifies the purpose of the buffer to free.
	@param[in] id  Specifies the ID of the buffer to free.
	@param[in] addr  Specifies the address of the buffer to free.
*/
void GetDeallocator(GLenum area, GLenum aim, GLuint id, void* addr);

/*!
	@brief Allocates a buffer from the heap in main memory managed by the memory manager.

	:private

	This is intended for use by the demo library.
	This allocator is called as a system application.

	@param[in] size  Specifies the size of the buffer to allocate.

	@return Returns the address of the allocated buffer.
*/
void* Alloc(size_t size);

/*!
	@brief Frees a buffer allocated from the heap in main memory.

	:private

	This is intended for use by the demo library.

	@param[in] ptr  Specifies the address of the buffer to free.
*/
void Free(void* ptr);
}  // namespace demo

#endif
