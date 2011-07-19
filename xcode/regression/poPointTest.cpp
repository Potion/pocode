#include <boost/test/unit_test.hpp>

#include "poPoint.h"

BOOST_AUTO_TEST_CASE( point_test_1 ) {
	poPoint p1(100,100);
    BOOST_CHECK_CLOSE(p1.x, 100.0, 0.0000001);
    BOOST_CHECK_CLOSE(p1.y, 100.0, 0.0000001);
}