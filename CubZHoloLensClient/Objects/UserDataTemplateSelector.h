#pragma once

#include "pch.h"
#include "User.h"

namespace CubZHoloLensClient
{
	//https://docs.microsoft.com/en-us/cpp/cppcx/ref-classes-and-structs-c-cx
	//https://msdn.microsoft.com/en-us/library/es7h5kch.aspx
	public ref class UserDataTemplateSelector sealed : Windows::UI::Xaml::Controls::DataTemplateSelector
	{
	private:
		Windows::UI::Xaml::DataTemplate	^_adminTemplate;
		Windows::UI::Xaml::DataTemplate	^_guestTemplate;

	public:
		UserDataTemplateSelector(Windows::UI::Xaml::DataTemplate ^admin, Windows::UI::Xaml::DataTemplate ^guest)
			: _adminTemplate(admin), _guestTemplate(guest)
		{
		}

	public:
		property Windows::UI::Xaml::DataTemplate^ AdminTemplate
		{
			void set(Windows::UI::Xaml::DataTemplate ^adminTemplate)
			{
				_adminTemplate = adminTemplate;
			}
		}

		property Windows::UI::Xaml::DataTemplate^ GuestTemplate
		{
			void set(Windows::UI::Xaml::DataTemplate ^guestTemplate)
			{
				_guestTemplate = guestTemplate;
			}
		}

	public:
		virtual Windows::UI::Xaml::DataTemplate ^SelectTemplateCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container) override
		{
			User ^user = dynamic_cast<User^>(item);
			if (user->Rank == UserRank::ADMIN)
				return (_adminTemplate);
			return (_guestTemplate);
		}
	};
}