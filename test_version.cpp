#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>

#include "libs/async.hpp"

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(Test) {
  Context* ctx = connect(3);

  receive(ctx, "cmd1");
  receive(ctx, "cmd2");
  receive(ctx, "cmd3");
  receive(ctx, "cmd4");
  receive(ctx, "cmd5");

  receive(ctx, "{");
  receive(ctx, "cmd6");
  receive(ctx, "cmd7");
  receive(ctx, "cmd8");
  receive(ctx, "}");

  disconnect(ctx);
}

BOOST_AUTO_TEST_CASE(TestContext) {
  Context* ctx1 = connect(2);
  receive(ctx1, "ctx1_cmd1");
  receive(ctx1, "ctx1_cmd2");

  Context* ctx2 = connect(4);
  receive(ctx2, "ctx2_cmd1");
  receive(ctx2, "ctx2_cmd2");
  receive(ctx2, "ctx2_cmd3");
  receive(ctx2, "ctx2_cmd4");

  disconnect(ctx1);
  disconnect(ctx2);
}

BOOST_AUTO_TEST_SUITE_END()
