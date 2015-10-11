#include "Location.hpp"

Location::Location(const char* start, const char* end) : first(start), last(end), pos(start), tracker(start) { }