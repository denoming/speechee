#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speaker/AvailabilityObserver.hpp"

using namespace jar;
using namespace testing;

class MockAvailabilitySubject : public AvailabilitySubject {
public:
    explicit MockAvailabilitySubject(std::string name)
        : AvailabilitySubject{std::move(name)}
    {
    }

    using AvailabilitySubject::availability;
};

TEST(AvailabilityObserverTest, Status)
{
    MockAvailabilitySubject s1{"Subject1"};
    MockAvailabilitySubject s2{"Subject2"};
    s1.availability(AvailabilityStatus::Online);
    s2.availability(AvailabilityStatus::Offline);

    AvailabilityObserver o1{"Observer1"};
    EXPECT_EQ(o1.availability(), AvailabilityStatus::Offline);
    o1.add(s1);
    EXPECT_EQ(o1.availability(), AvailabilityStatus::Online);
    o1.add(s2);
    EXPECT_EQ(o1.availability(), AvailabilityStatus::Offline);

    o1.remove(s2);
    EXPECT_EQ(o1.availability(), AvailabilityStatus::Online);

    MockFunction<AvailabilitySubject::OnStateUpdate> callback;
    EXPECT_CALL(callback, Call(Not(IsEmpty()), AvailabilityStatus::Offline));
    auto c1 = o1.onAvailabilityUpdate(callback.AsStdFunction());
    s1.availability(AvailabilityStatus::Offline);
    EXPECT_EQ(o1.availability(), AvailabilityStatus::Offline);
    c1.disconnect();
}