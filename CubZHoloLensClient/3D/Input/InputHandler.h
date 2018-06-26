#pragma once

#include <boost/lockfree/queue.hpp>

namespace HoloLensClient
{
	/*
	*	Stucture representation of an input
	*/
	struct InputEvent
	{

	};

	/*
	*	Singleton handling user input in a FIFO queue
	*/
	class InputHandler
	{
	private:
		boost::lockfree::queue<InputEvent>	_eventQueue;

	public:
		InputHandler(InputHandler const &) = delete;
		InputHandler &operator=(InputHandler const &) = delete;

	private:
		InputHandler() = default;

	public:
		/*
		*	Return the instance of the InputHandler
		*/
		static InputHandler &Instance()
		{
			static InputHandler	instance{};

			return (instance);
		}

		/*
		*	Add a new InputEvent to the queue
		*/
		void pushEvent(InputEvent &event)
		{
			_eventQueue.push(event);
		}

		/*
		*	Retrieve the oldest input event
		*/
		InputEvent popEvent()
		{
			InputEvent result;
			_eventQueue.pop(result);

			return (result);
		}

		/*
		*	Check if the event queue is empty
		*/
		bool isEmpty()
		{
			return (_eventQueue.empty());
		}
	};
}