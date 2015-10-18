#include "Location.hpp"

Location::Location(const char* start, const char* end) : lineNr(1), first(start), last(end), pos(start), tracker(start) { }