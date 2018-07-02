#include "pch.h"
#include "Objects\HoloLensContext.h"

// The main function is only used to initialize our IFrameworkView class.
// Under most circumstances, you should not need to modify this function.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	try
	{
		auto callback = ref new ::Windows::UI::Xaml::ApplicationInitializationCallback(
			[](::Windows::UI::Xaml::ApplicationInitializationCallbackParams^) {

			CubZHoloLensClient::HoloLensContext::Instance();

			auto app = ref new CubZHoloLensClient::App();
		});
		//Launch the XAML App
		::Windows::UI::Xaml::Application::Start(callback);

		/*AppViewSource^ appViewSource = ref new ::AppViewSource();
		CoreApplication::Run(appViewSource);*/
	}
	catch (std::exception &e)
	{
		TRACE("GOT UNHANDLED EXCEPTION ON MAIN " << e.what() << std::endl);
	}
	return 0;
}