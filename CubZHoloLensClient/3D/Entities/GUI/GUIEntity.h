#pragma once

#include <3D\Entities\Common\InteractableEntity.h>

using namespace Windows::Foundation::Numerics;

namespace HoloLensClient
{
	class GUIEntity : public InteractableEntity
	{
	protected:
		float3				_size;
		float4				_color;
		std::wstring		_label;

	public:
		GUIEntity(std::shared_ptr<HolographicScene> scene,
			Spatial::SpatialGestureSettings settings = Spatial::SpatialGestureSettings::None)
			: InteractableEntity(scene, settings) {}
		virtual ~GUIEntity() = default;

	public:
		/// <summary>	Executes the label changed action. </summary>
		virtual void OnLabelChanged() {}
		
		bool OnGetFocus() override { return false; }
		bool OnLostFocus() override { return false; }

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets a size. </summary>
		///
		/// <param name="size">	The size. </param>
		///-------------------------------------------------------------------------------------------------
		void setSize(float3 size) { _size = size; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets a color. </summary>
		///
		/// <param name="color">	The color. </param>
		///-------------------------------------------------------------------------------------------------
		void setColor(float4 color) { _color = color; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets a label. </summary>
		///
		/// <param name="label">	The label. </param>
		///-------------------------------------------------------------------------------------------------
		void setLabel(std::wstring const &label) {
			_label = label;
			OnLabelChanged();
		}

	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the size. </summary>
		///
		/// <returns>	The size. </returns>
		///-------------------------------------------------------------------------------------------------
		float3 getSize() const { return _size; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the color. </summary>
		///
		/// <returns>	The color. </returns>
		///-------------------------------------------------------------------------------------------------
		float4 getColor() const { return _color; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the label. </summary>
		///
		/// <returns>	The label. </returns>
		///-------------------------------------------------------------------------------------------------
		std::wstring const &getLabel() const { return _label; }
	};
}