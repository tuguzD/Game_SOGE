#ifndef SOGE_EVENT_EVENTHELPERS_HPP
#define SOGE_EVENT_EVENTHELPERS_HPP

#include "SOGE/Utils/PreprocessorHelpers.hpp"


#define SG_DERIVED_FROM_STATIC_EVENT_ASSERT(eventName)                                                                 \
    static_assert(::soge::DerivedFromStaticEvent<eventName>,                                                           \
                  UE_STRINGIZE(UE_JOIN(eventName, should have StaticEvent in parent hierarchy !)))

#endif // SOGE_EVENT_EVENTHELPERS_HPP
