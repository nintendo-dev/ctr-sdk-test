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

#ifndef DEMO_RENDER_DATA_H_
#define DEMO_RENDER_DATA_H_

#include <nn/gx.h>
#include <nn/util.h>

namespace demo {

/*!
:private

@brief Gets the LCD width.

@param[in]    display    Target LCD (<tt>NN_GX_DISPLAY0</tt> or <tt>NN_GX_DISPLAY1</tt>)
@return       LCD width.
*/
f32 GetLcdWidth(const s32 display);

/*!
:private

@brief Gets the LCD height.

@param[in]    display    Target LCD (<tt>NN_GX_DISPLAY0</tt> or <tt>NN_GX_DISPLAY1</tt>)
@return       LCD height.
*/
f32 GetLcdHeight(const s32 display);

/*!
:private

@brief Base class for the rendering data.
*/
class RenderData : private nn::util::NonCopyable<RenderData>
{
public:
	/*!
	:private

	@brief Constructor.
	*/
	RenderData(void);
	/*!
	:private

	@brief Destructor.
	*/
	virtual ~RenderData(void);

public:
	/*!
	:private

	@brief Initializes the rendering data.
	*/
	virtual void Initialize(void);

	/*!
	:private

	@brief Initializes the rendering data.
	*/
	virtual void Finalize(void);

public:
	/*!
	:private

	@brief Performs rendering using the rendering data.
	*/
	virtual void Draw(void) = 0;

public:
	/*!
	:private

	@brief Sets the window size to convert from the normalized device coordinate system to the window coordinate system.

	@param[in] windowWidth  Specifies the window width.
	@param[in] windowHeight  Specifies the window height.
	*/
	virtual void SetWindowSize(const f32 windowWidth, const f32 windowHeight);

public:
	/*!
	:private

	@brief Converts the specified window coordinates to normalized device coordinates.

	@param[in] windowCoordinateX  Specifies the x-coordinate for the window coordinates.
	@param[in] windowCoordinateY  Specifies the y-coordinate for the window coordinates.
	@param[out] normalizedDeviceCoordinateX  Specifies the x-coordinate for the normalized device coordinates.
	@param[out] normalizedDeviceCoordinateY  Specifies the y-coordinate for the normalized device coordinates.
	*/
	virtual void GetNormalizedDeviceCoordinateXY(const f32 windowCoordinateX,
												 const f32 windowCoordinateY,
												 f32&	   normalizedDeviceCoordinateX,
												 f32&	   normalizedDeviceCoordinateY);

	/*!
	:private

	@brief Converts the y-coordinate for the provided window coordinates to an x-coordinate for the normalized device coordinates.

	@param[in] windowCoordinateY  Specifies the y-coordinate for the window coordinates.
	@return Returns the x-coordinate for the normalized device coordinates.
	*/
	virtual f32 GetNormalizedDeviceCoordinateX(const f32 windowCoordinateY);

	/*!
	:private

	@brief Converts the x-coordinate for the provided window coordinates to a y-coordinate for the normalized device coordinates.

	@param[in] windowCoordinateX  Specifies the x-coordinate for the window coordinates.
	@return Returns the y-coordinate for the normalized device coordinates.
	*/
	virtual f32 GetNormalizedDeviceCoordinateY(const f32 windowCoordinateX);

protected:
	virtual void CalculateInverseWindowSize(void);

protected:
	s32 m_RenderTarget;
	f32 m_WindowWidth;
	f32 m_WindowHeight;

	f32 m_InverseWindowWidth;
	f32 m_InverseWindowHeight;
};

}  // namespace demo

#endif
