///-------------------------------------------------------------------------------------------------
// file:	3D\Objects\GUI\Interfaces\ITextObject.h
//
// summary:	Declares the ITextObject interface
///-------------------------------------------------------------------------------------------------
#pragma once

#include <3D\Objects\Common\IObject.h>


///-------------------------------------------------------------------------------------------------
// namespace: HoloLensClient
//
// summary:	.
///-------------------------------------------------------------------------------------------------
namespace HoloLensClient
{
	/// <summary>	A text object. </summary>
	class ITextObject : public virtual IObject
	{
	public:
		/// <summary>	Destructor. </summary>
		virtual ~ITextObject() = default;

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the text. </summary>
		///
		/// <returns>	The text. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual std::wstring const &getText() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets a text. </summary>
		///
		/// <param name="text">	The text. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void setText(std::wstring const &text) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets font size. </summary>
		///
		/// <returns>	The font size. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual float const getFontSize() const = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets font size. </summary>
		///
		/// <param name="font">	The font. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void setFontSize(float font) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets a color. </summary>
		///
		/// <param name="parameter1">	The first parameter. </param>
		///-------------------------------------------------------------------------------------------------
		virtual void setColor(Windows::Foundation::Numerics::float4) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the color. </summary>
		///
		/// <returns>	The color. </returns>
		///-------------------------------------------------------------------------------------------------
		virtual Windows::Foundation::Numerics::float4 getColor() const = 0;
	};
}