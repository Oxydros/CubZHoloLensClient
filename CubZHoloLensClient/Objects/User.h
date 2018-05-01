#pragma once

#include "pch.h"

namespace CubZHoloLensClient
{
	public enum class UserRank
	{
		UNKNOWN = -1,
		GUEST,
		ADMIN,
	};

	//[WFM::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class User sealed
	{
	private:
		Platform::String	^_username;
		UserRank			_rank;

	public:
		User(Platform::String ^name, UserRank rank);

		property Platform::String ^Username
		{
			Platform::String ^get();
		}

		property UserRank Rank
		{
			UserRank get();
		}

		property Platform::String ^RankText
		{
			Platform::String ^get();
		}
	};
}