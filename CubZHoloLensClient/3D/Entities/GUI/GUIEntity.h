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
		GUIEntity(std::shared_ptr<HolographicScene> &scene) : InteractableEntity(scene) {}
		virtual ~GUIEntity() = default;

	public:
		virtual void OnLabelChanged() {}
		
		void OnGetFocus() override {}
		void OnLostFocus() override {}

	public:
		void setSize(float3 size) { _size = size; }
		void setColor(float4 color) { _color = color; }

		void setLabel(std::wstring const &label) {
			_label = label;
			OnLabelChanged();
		}

	public:
		float3 getSize() const { return _size; }
		float4 getColor() const { return _color; }
		std::wstring const &getLabel() const { return _label; }
	};
}