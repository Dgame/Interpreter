#include "Location.hpp"

Loc::Loc(const char* start, const char* end) : first(start), last(end), pos(start), tracker(start) { }