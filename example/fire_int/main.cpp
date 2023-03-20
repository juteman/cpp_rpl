#include <rpl/rpl.h>
#include <iostream>

int main () {
    rpl::event_stream<int> eventStream;
    int i = 0;
    rpl::lifetime lifetime;
    eventStream.events_starting_with(std::move(i)) | rpl::start_with_next([](int value) {
        std::cout << "rpl event stream value: " << value << std::endl;
    }, lifetime);

    eventStream.fire(3);
    eventStream.fire(7);
    eventStream.fire(9);

    return 0;
}