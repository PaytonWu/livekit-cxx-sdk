#include <future>
#include <string>

#include <stdexec/execution.hpp>
#include <stdexec/receiver.hpp>
#include <stdexec/sender.hpp>

class Room
{
public:
    // ...existing code...

    // Asynchronously retrieves the session ID (SID) of the room.
    auto sid() -> stdexec::sender<std::string>
    {
        return stdexec::let_value([this]() -> stdexec::sender<std::string> {
            if (!_info.sid.empty())
            {
                return stdexec::just(_info.sid);
            }
            return stdexec::when_all(stdexec::just(), _first_sid_future.get_sender());
        });
    }

private:
    struct RoomInfo
    {
        std::string sid;
        // ...existing code...
    };

    RoomInfo _info;
    std::promise<std::string> _first_sid_promise;
    stdexec::future<std::string> _first_sid_future = _first_sid_promise.get_future();

    // ...existing code...
};
