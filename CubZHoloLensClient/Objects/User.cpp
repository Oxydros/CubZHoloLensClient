#include "pch.h"
#include "User.h"

CubZHoloLensClient::User::User(Platform::String ^username, UserRank rank)
	: _username(username), _rank(rank)
{
}

Platform::String ^CubZHoloLensClient::User::Username::get()
{
	return (_username);
}

CubZHoloLensClient::UserRank CubZHoloLensClient::User::Rank::get()
{
	return (_rank);
}

Platform::String ^CubZHoloLensClient::User::RankText::get()
{
	return (_rank == UserRank::GUEST ? "Guest" : (_rank == UserRank::UNKNOWN ? "Unknown" : "Admin"));
}